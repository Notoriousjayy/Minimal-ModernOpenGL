cmake_minimum_required(VERSION 3.14)
project(ModernGLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

include(FetchContent)

#
# GLM
#
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)
set(GLM_TEST_ENABLE OFF CACHE BOOL "Disable GLM unit tests" FORCE)
FetchContent_MakeAvailable(glm)

#
# GLFW
#
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW Docs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW Examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW Tests" FORCE)
FetchContent_MakeAvailable(glfw)

#
# GLAD
#
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=3.3" CACHE STRING "OpenGL version")
set(GLAD_GENERATOR "c" CACHE STRING "Language for GLAD")
FetchContent_MakeAvailable(glad)

file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

**target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        glfw
        glad
        glm::glm
)**
