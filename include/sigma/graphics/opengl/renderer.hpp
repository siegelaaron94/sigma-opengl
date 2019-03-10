#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/pipeline.hpp>
#include <sigma/graphics/renderer.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <glad/glad.h>

#include <sigma/util/hash.hpp>

namespace std {
template <>
class hash<std::vector<uint32_t>> {
public:
    std::size_t operator()(const std::vector<uint32_t>& vec) const
    {
        std::size_t seed = 0;
        for (auto val : vec)
            seed = sigma::util::hash_combine(seed, val);

        return seed;
    }
};
}

namespace sigma {

namespace opengl {
    class SIGMA_API renderer : public graphics::renderer {
    public:
        renderer(glm::ivec2 size, std::shared_ptr<sigma::context> ctx);

        renderer(const renderer&) = delete;

        renderer(renderer&&) = default;

        virtual ~renderer();

        renderer& operator=(const renderer&) = delete;

        renderer& operator=(renderer&&) = default;

        virtual void resize(glm::uvec2 size) override;

        virtual void render(const entt::registry<>& registry) override;

    private:
        void begin_pass(const graphics::pass& p);

        void end_pass(const graphics::pass& p);

        GLenum texture_format(const graphics::texture_format& fmt);

        GLuint frame_buffer_for(const graphics::pass& p);

        GLuint render_texture_for(const graphics::render_texture& tex);

        int loader_status_;
        glm::vec2 size_;

        std::unordered_map<std::vector<uint32_t>, GLuint> frame_buffers_;
        std::unordered_map<uint32_t, GLuint> render_textures_;
    };

    void debug_callback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
