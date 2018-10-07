cmake_minimum_required(VERSION 3.10)
project(sigma-opengl)

add_library(sigma-opengl STATIC
    src/sigma/graphics/opengl/debug_draw_renderer.cpp
    #src/sigma/graphics/opengl/cubemap_manager.cpp
    #src/sigma/graphics/opengl/geometry_buffer.cpp
    src/sigma/graphics/opengl/renderer.cpp
    #src/sigma/graphics/opengl/shader_manager.cpp
    #src/sigma/graphics/opengl/static_mesh_manager.cpp
    #src/sigma/graphics/opengl/technique_manager.cpp
    #src/sigma/graphics/opengl/texture_manager.cpp
)

target_include_directories(sigma-opengl PUBLIC include)
target_compile_definitions(sigma-opengl PRIVATE -DSIGMA_EXPORT)
target_link_libraries(sigma-opengl PUBLIC sigma-core stb::stb debug-draw glad)
target_package_resources(sigma-opengl resources)
if(COTIRE_CMAKE_MODULE_VERSION)
	cotire(sigma-opengl)
endif()
