cmake_minimum_required(VERSION 3.14)  # Needed for FetchContent
project(ModernGLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------
# 1) Find OpenGL from the system (Mesa or equivalent).
#    If this fails, the user must install an OpenGL dev package manually.
# ------------------------------------------------------------------------
find_package(OpenGL REQUIRED)

# ------------------------------------------------------------------------
# 2) Use FetchContent to automatically download/build third-party libs
#    (GLFW, GLAD, GLM) if not found on the system.
# ------------------------------------------------------------------------
include(FetchContent)

#
# 2a) GLM
#
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8           # or a newer release tag
)
# Optionally set GLM as a header-only library
set(GLM_TEST_ENABLE OFF CACHE BOOL "Disable GLM unit tests" FORCE)
FetchContent_MakeAvailable(glm)

#
# 2b) GLFW
#
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8             # pick a known stable release
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "GLFW Docs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW Examples" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "GLFW Tests" FORCE)
FetchContent_MakeAvailable(glfw)

#
# 2c) GLAD
#
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36          # or a newer release
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=3.3"  CACHE STRING "OpenGL version")
set(GLAD_GENERATOR "c" CACHE STRING "Language for GLAD")
FetchContent_MakeAvailable(glad)

# ------------------------------------------------------------------------
# 3) Now create our main project
# ------------------------------------------------------------------------
# Note: we assume your project has a "src" folder with .cpp files, plus an "include" folder.
file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Where to find your own headers:
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Link the libraries we just fetched:
#   - OpenGL from the system
#   - GLFW (the target is usually "glfw" or "glfw3")
#   - GLAD (the target is "glad" in the official CMakeLists)
#   - GLM is header-only, so no need to link, just #include <glm/glm.hpp>
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL      # Provided by find_package(OpenGL)
        glfw           # Provided by FetchContent for GLFW
        glad           # Provided by FetchContent for GLAD
)

# On some Linux distros, you may also need X11, pthread, dl, etc.:
# target_link_libraries(${PROJECT_NAME} PRIVATE X11 pthread dl)
