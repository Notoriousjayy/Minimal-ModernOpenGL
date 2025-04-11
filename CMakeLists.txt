cmake_minimum_required(VERSION 3.14)
project(ModernGLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------
# 1) Check if the user has OpenGL installed at the system level.
#    On Debian/Ubuntu/WSL, you typically need:
#      sudo apt-get install libgl1-mesa-dev
# ------------------------------------------------------------------------
find_package(OpenGL)

if(NOT OpenGL_FOUND)
    message(FATAL_ERROR
        "Could NOT find OpenGL.\n"
        "Please install the OpenGL development packages.\n"
        "Example (Debian/Ubuntu/WSL): sudo apt-get update && sudo apt-get install libgl1-mesa-dev\n"
    )
endif()

# ------------------------------------------------------------------------
# 2) Use FetchContent to automatically download/build third-party libs
#    (GLFW, GLAD, GLM), if not found system-wide.
# ------------------------------------------------------------------------
include(FetchContent)

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=3.3"  CACHE STRING "OpenGL version")
set(GLAD_GENERATOR "c" CACHE STRING "Language for GLAD")
FetchContent_MakeAvailable(glad)

# ------------------------------------------------------------------------
# 3) Build your application
# ------------------------------------------------------------------------
file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Where to find your own headers:
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Link with system OpenGL, plus fetched libraries: GLFW, GLAD, and GLM.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL       # from find_package(OpenGL)
        glfw             # from FetchContent (GLFW)
        glad             # from FetchContent (GLAD)
        glm::glm         # Link against the GLM CMake target
)
