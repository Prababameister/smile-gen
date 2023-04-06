cmake_minimum_required(VERSION 3.25)
project(ShaderUtils)

add_library(shader_utils INTERFACE)

target_include_directories(
    shader_utils
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/shaders>
        $<INSTALL_INTERFACE:lib/shaders>
)
