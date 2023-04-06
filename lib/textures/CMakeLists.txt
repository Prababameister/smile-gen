cmake_minimum_required(VERSION 3.25)
project(TextureUtils)

add_library(texture_utils INTERFACE)

target_include_directories(
    texture_utils
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/textures>
        $<INSTALL_INTERFACE:lib/textures>
)
