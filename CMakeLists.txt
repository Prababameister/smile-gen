cmake_minimum_required (VERSION 3.25)

project(smile-gen)

set(PROJECT_HEAD ${PROJECT_SOURCE_DIR})
add_definitions( -DPROJECT_HEAD="${PROJECT_HEAD}" )

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.7
  )
FetchContent_GetProperties(glfw)

if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)

  set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
  set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
  set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
  set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

find_package(OpenGL REQUIRED)

set(SOURCES
  main.cc
)

add_executable(smile-gen-run ${SOURCES})
target_link_libraries(smile-gen-run glfw OpenGL::GL)

add_subdirectory(lib/foreign/)
add_subdirectory(lib/foreign/glm)
add_subdirectory(lib/shaders/)
add_subdirectory(lib/textures/)
target_link_libraries(smile-gen-run glad shader_utils texture_utils glm)
