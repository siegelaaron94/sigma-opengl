#include <sigma/graphics/opengl/renderer.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <iostream>

namespace sigma {
namespace opengl {
    renderer::renderer(glm::ivec2 size, std::shared_ptr<sigma::context> ctx)
        : graphics::renderer(size, ctx)
        , loader_status_(gladLoadGL())
        , size_(size.x, size.y)
    {
        if (!loader_status_)
            throw std::runtime_error("error: could not load OpenGL");

        // TODO disable this in production.
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debug_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, true);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, true);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, true);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);
    }

    renderer::~renderer()
    {
    }

    void renderer::resize(glm::uvec2 size)
    {
        glViewport(0, 0, size.x, size.y);
    }

    void renderer::render(const graphics::renderer::world_view_type& world)
    {
    }

    void renderer::begin_pass(const graphics::pass& p)
    {
        auto framebuffer = frame_buffer_for(p);
    }

    /*void renderer::geomery_pass(const graphics::pass& p, const graphics::renderer::world_view_type& world)
    {
        std::vector<render_token> tokens;
        world.for_each([](static_mesh& mesh, transform& txform) {
            for (const auto& part : mesh.parts()) {
                render_token token;

                token.group = p.group();

                token.framebuffer = frame_buffer_for(p);

                token.program = program_for(part.material());

                token.mesh = mesh_for(part);

                token.matrix = txform.matrix;

                uint32_t buffer_index = 0;
                token.buffer[buffer_index++] = constant_buffer();

                token.buffer[buffer_index++] = object_buffer();

                for (const auto& buffer : p.buffers())
                    token.buffer[buffer_index++] = buffer_for(buffer);

                for (const auto& buffer : part.material().buffers())
                    token.buffer[buffer_index++] = buffer_for(buffer);

                uint32_t texture_index = 0;
                for (const auto& texture : p.in_textures)
                    token.textures[texture_index++] = texture_for(texture);

                for (const auto& texture : part.material().textures())
                    token.textures[texture_index++] = texture_for(texture);

                tokens.push_back(t);
            }
        });
    }*/

    void renderer::end_pass(const graphics::pass& p)
    {
    }

    GLenum renderer::texture_format(const graphics::texture_format& fmt)
    {
        switch (fmt) {
        case sigma::graphics::texture_format::RGB8:
            return GL_RGB8;
            break;
        case sigma::graphics::texture_format::RGBA8:
            return GL_RGBA8;
            break;
        case sigma::graphics::texture_format::RGB16F:
            return GL_RGB16F;
            break;
        case sigma::graphics::texture_format::RGBA16F:
            return GL_RGBA16F;
            break;
        case sigma::graphics::texture_format::RGB32F:
            return GL_RGB32F;
            break;
        case sigma::graphics::texture_format::DEPTH32F_STENCIL8:
            return GL_DEPTH32F_STENCIL8;
            break;
        }
        return GL_NONE;
    }

    GLuint renderer::frame_buffer_for(const graphics::pass& p)
    {
        auto it = frame_buffers_.find(p.out_textures);
        if (it != frame_buffers_.end())
            return it->second;

        // We must create the frame buffer for this pass
        GLuint fbo;
        glCreateFramebuffers(1, &fbo);
        assert(glGetError() == GL_NO_ERROR);

        GLenum next_color_att = GL_COLOR_ATTACHMENT0;
        std::vector<GLenum> draw_buffers;
        for (auto texid : p.out_textures) {
            const auto& tex = context_->pipeline()->get_render_texture(texid);
            auto tid = render_texture_for(tex);
            GLenum att;
            switch (tex.format) {
            case sigma::graphics::texture_format::RGB8:
            case sigma::graphics::texture_format::RGBA8:
            case sigma::graphics::texture_format::RGB16F:
            case sigma::graphics::texture_format::RGBA16F:
            case sigma::graphics::texture_format::RGB32F:
                att = next_color_att;
                draw_buffers.push_back(att);
                next_color_att++;
                break;
            case sigma::graphics::texture_format::DEPTH32F_STENCIL8:
                att = GL_DEPTH_STENCIL_ATTACHMENT;
                break;
            }
            glNamedFramebufferTexture(fbo, att, tid, 0);
            assert(glGetError() == GL_NO_ERROR);
        }

        glNamedFramebufferDrawBuffers(fbo, draw_buffers.size(), draw_buffers.data());
        assert(glGetError() == GL_NO_ERROR);

        frame_buffers_[p.out_textures] = fbo;

        return fbo;
    }

    GLuint renderer::render_texture_for(const graphics::render_texture& tex)
    {
        auto it = render_textures_.find(tex.id);
        if (it != render_textures_.end())
            return it->second;

        GLenum fmt = texture_format(tex.format);

        GLuint tid;
        glCreateTextures(GL_TEXTURE_2D, 1, &tid);
        assert(glGetError() == GL_NO_ERROR);

        glTextureStorage2D(tid, 1, fmt, tex.size.x, tex.size.y);
        assert(glGetError() == GL_NO_ERROR);

        render_textures_[tex.id] = tid;
        return tid;
    }

    void debug_callback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        switch (severity) {
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "severity: LOW\n";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "severity: MEDIUM\n";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "severity: HIGH\n";
            break;
        default:
            return;
        }

        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "type: ERROR\n";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "type: DEPRECATED_BEHAVIOR\n";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "type: UNDEFINED_BEHAVIOR\n";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "type: PORTABILITY\n";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "type: PERFORMANCE\n";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "type: MARKER\n";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "type: PUSH_GROUP\n";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "type: POP_GROUP\n";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "type: OTHER\n";
            break;
        }
        std::cout << "message: " << message << '\n';
        std::cout << "id: " << id << '\n';
        switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "source: API\n";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "source: WINDOW_SYSTEM\n";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "source: SHADER_COMPILER\n";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "source: THIRD_PARTY\n";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "source: APPLICATION\n";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "source: OTHER\n";
            break;
        }
    }
}
}
