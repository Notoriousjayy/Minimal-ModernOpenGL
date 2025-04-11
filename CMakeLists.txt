cmake_minimum_required(VERSION 3.14)

#
# --- IMPORTANT: Retain compatibility for subprojects requiring CMake <3.5. ---
#
# Newer CMake (>=3.27) removes backward-compat for <3.5 unless we set CMP0144 to OLD.
# This ensures older libraries like GLM 0.9.9.8 (which do cmake_minimum_required(VERSION 2.8))
# still work.
#
if (POLICY CMP0144)
    cmake_policy(SET CMP0144 OLD)
endif()

project(ModernGLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------
# 1) Find OpenGL from the system. If not found, user must install it.
# ------------------------------------------------------------------------
find_package(OpenGL REQUIRED)

# ------------------------------------------------------------------------
# 2) Use FetchContent to download/build third-party libs (GLFW, GLAD, GLM).
# ------------------------------------------------------------------------
include(FetchContent)

# 2a) GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8  # older release that sometimes has cmake_minimum_required(VERSION 2.8)
)
set(GLM_TEST_ENABLE OFF CACHE BOOL "Disable GLM unit tests" FORCE)
FetchContent_MakeAvailable(glm)

# 2b) GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "GLFW Docs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW Examples" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "GLFW Tests" FORCE)
FetchContent_MakeAvailable(glfw)

# 2c) GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API     "gl=3.3"  CACHE STRING "OpenGL version")
set(GLAD_GENERATOR "c"     CACHE STRING "Language for GLAD")
FetchContent_MakeAvailable(glad)

# ------------------------------------------------------------------------
# 3) Build your application
# ------------------------------------------------------------------------
# Assuming you have ./src/*.cpp and ./include for headers:
file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL  # from find_package(OpenGL)
        glfw        # from FetchContent (GLFW)
        glad        # from FetchContent (GLAD)
        # GLM is header-only, so no actual library to link:
        # but we can add "glm::glm" if we want, i.e. target_link_libraries(... glm::glm)
)
