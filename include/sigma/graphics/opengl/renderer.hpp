#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/render_queue.hpp>
#include <sigma/graphics/renderer.hpp>

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

        virtual graphics::render_queue* queue() override;

        virtual void resize(glm::uvec2 size) override;

        virtual void render() override;

    private:
        int loader_status_;
        glm::vec2 size_;

        graphics::render_queue queue_;

        /*
        void begin_pass(const graphics::pass& p);
        void end_pass(const graphics::pass& p);
        GLenum texture_format(const graphics::texture_format& fmt);
        GLuint frame_buffer_for(const graphics::pass& p);
        GLuint render_texture_for(const graphics::render_texture& tex);
        std::unordered_map<std::vector<uint32_t>, GLuint> frame_buffers_;
        std::unordered_map<uint32_t, GLuint> render_textures_;
        */
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
