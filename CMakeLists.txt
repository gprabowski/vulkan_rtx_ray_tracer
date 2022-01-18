cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME vulkan_rtx_ray_tracer)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

function(add_shader TARGET SHADER)
    find_program(GLSLC glslc)

    set(current-shader-path ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER})
    set(current-output-path ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv)

    get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
    file(MAKE_DIRECTORY ${current-output-dir})

    add_custom_command(
           OUTPUT ${current-output-path}
           COMMAND ${GLSLC} --target-env=vulkan1.2 -o ${current-output-path} ${current-shader-path}
           DEPENDS ${current-shader-path}
           IMPLICIT_DEPENDS CXX ${current-shader-path}
           VERBATIM)

    set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_shader)

include_directories(3rdparty/)
include_directories(headers/)

set(SOURCES
    sources/main.cpp
    sources/ray_tracer.cpp
    sources/extension_functions.cpp
    sources/vertex.cpp
    sources/setup_utilities.cpp
    sources/lib_impl.cpp
    sources/vulkan_utilities.cpp
    sources/create_functions.cpp
    sources/asset_utilities.cpp

    headers/ray_tracer.h
    headers/constants.h
    headers/extension_functions.h
    headers/vertex.h
)

set(SHADERS
    shaders/shader.vert
    shaders/shader.frag
    shaders/raytrace.rgen
    shaders/raytrace.rchit
    shaders/raytrace.rmiss
    shaders/ao_helpers.h
)

source_group("shaders" FILES ${SHADERS})

add_executable(${PROJECT_NAME} ${SOURCES} ${SHADERS})


add_shader(${PROJECT_NAME} shader.vert)
add_shader(${PROJECT_NAME} shader.frag)
add_shader(${PROJECT_NAME} raytrace.rgen)
add_shader(${PROJECT_NAME} raytrace.rchit)
add_shader(${PROJECT_NAME} raytrace.rmiss)

target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/assets/
                ${CMAKE_CURRENT_BINARY_DIR}/assets/)
