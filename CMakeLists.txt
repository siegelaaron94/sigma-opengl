cmake_minimum_required(VERSION 3.10)
project(sigma-opengl)

add_library(sigma-opengl STATIC
    #src/sigma/graphics/opengl/cubemap_manager.cpp
    #src/sigma/graphics/opengl/geometry_buffer.cpp
    #src/sigma/graphics/opengl/shader_manager.cpp
    #src/sigma/graphics/opengl/static_mesh_manager.cpp
    #src/sigma/graphics/opengl/technique_manager.cpp
    #src/sigma/graphics/opengl/texture_manager.cpp
    include/sigma/graphics/opengl/cubemap_manager.hpp
    include/sigma/graphics/opengl/debug_draw_renderer.hpp
    include/sigma/graphics/opengl/geometry_buffer.hpp
    include/sigma/graphics/opengl/render_uniforms.hpp
    include/sigma/graphics/opengl/renderer.hpp
    include/sigma/graphics/opengl/shader_manager.hpp
    include/sigma/graphics/opengl/static_mesh_manager.hpp
    include/sigma/graphics/opengl/technique_manager.hpp
    include/sigma/graphics/opengl/texture_manager.hpp
    include/sigma/graphics/opengl/uniform_buffer.hpp
    include/sigma/graphics/opengl/util.hpp
    src/sigma/graphics/opengl/debug_draw_renderer.cpp
    src/sigma/graphics/opengl/renderer.cpp
)

target_include_directories(sigma-opengl PUBLIC include)
target_compile_definitions(sigma-opengl PRIVATE -DSIGMA_EXPORT)
target_link_libraries(sigma-opengl PUBLIC sigma-core debug-draw::debug-draw glad::glad)

if(COTIRE_CMAKE_MODULE_VERSION)
	cotire(sigma-opengl)
endif()

add_package(sigma-opengl-resources
    "default_basic.frag"
    "default_dielectric.frag"
    "default_matalic.frag"
    "default.frag"
    "default.vert"
    "fullscreen_quad.vert"
    "math.glsl"
    "null.frag"
    "pbr/brdf.glsl"
    "pbr/deffered/gamma_conversion.eff"
    "pbr/deffered/gamma_conversion.frag"
    "pbr/deffered/geometry_buffer.glsl"
    "pbr/deffered/lights/directional.eff"
    "pbr/deffered/lights/directional.frag"
    "pbr/deffered/lights/directional.vert"
    "pbr/deffered/lights/image_based.eff"
    "pbr/deffered/lights/image_based.frag"
    "pbr/deffered/lights/point.eff"
    "pbr/deffered/lights/point.frag"
    "pbr/deffered/lights/point.vert"
    "pbr/deffered/lights/spot.eff"
    "pbr/deffered/lights/spot.frag"
    "pbr/deffered/lights/spot.vert"
    "pbr/deffered/post_process_effect.glsl"
    "pbr/deffered/texture_blit.eff"
    "pbr/deffered/texture_blit.frag"
    "shadow_sampling.glsl"
    "shadow.frag"
    "shadow.stech"
    "sigma/graphics/shadow_block.glsl"
    "sigma/graphics/standard_block.glsl"
    #"sigma/shapes/cube.obj"
    #"sigma/shapes/sphere.obj"
    "uniforms.glsl"
    "vertex.glsl"
    "vignette.eff"
    "vignette.frag"
    PACKAGE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/resources"
)

add_dependencies(sigma-opengl sigma-opengl-resources)
