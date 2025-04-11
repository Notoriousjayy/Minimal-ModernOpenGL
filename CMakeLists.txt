cmake_minimum_required(VERSION 3.14)  # Needed for FetchContent

# If you want to set the new policy for timestamps globally:
# cmake_policy(SET CMP0135 NEW)

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
# 3) Build and install GNU GSL and gnuplot from source via ExternalProject
# ------------------------------------------------------------------------
include(ExternalProject)

#
# GSL
#
ExternalProject_Add(
    gsl_ext
    URL "https://ftp.gnu.org/gnu/gsl/gsl-2.7.1.tar.gz"   # Adjust version as needed
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE                      # Quiet CMP0135 warnings
    PREFIX         "${CMAKE_CURRENT_BINARY_DIR}/gsl"
    INSTALL_DIR    "${CMAKE_CURRENT_BINARY_DIR}/gsl/install"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/gsl/install
    BUILD_COMMAND    make
    INSTALL_COMMAND  make install
)

#
# gnuplot
#
ExternalProject_Add(
    gnuplot_ext
    URL "https://sourceforge.net/projects/gnuplot/files/gnuplot/5.4.8/gnuplot-5.4.8.tar.gz"  # Adjust version as needed
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    PREFIX         "${CMAKE_CURRENT_BINARY_DIR}/gnuplot"
    INSTALL_DIR    "${CMAKE_CURRENT_BINARY_DIR}/gnuplot/install"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/gnuplot/install
    BUILD_COMMAND    make
    INSTALL_COMMAND  make install
)

# Mark that our main target depends on these external projects
# so they will be built/installed first.
add_custom_target(third_party_deps ALL
    DEPENDS gsl_ext gnuplot_ext
)

# ------------------------------------------------------------------------
# 4) Now create our main project
# ------------------------------------------------------------------------
file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Ensure our main target depends on the external projects
# so GSL and gnuplot are built before we build/link.
add_dependencies(${PROJECT_NAME} third_party_deps)

# Add GSL include directory for your code if you're calling GSL APIs
ExternalProject_Get_Property(gsl_ext INSTALL_DIR)
set(GSL_INCLUDE_DIR "${INSTALL_DIR}/include")
set(GSL_LIB_DIR     "${INSTALL_DIR}/lib")

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${GSL_INCLUDE_DIR}
)

# ------------------------------------------------------------------------
# Create IMPORTED libraries for GSL and link them to our main target
# ------------------------------------------------------------------------
add_library(GSL STATIC IMPORTED)
set_target_properties(GSL PROPERTIES
    IMPORTED_LOCATION "${GSL_LIB_DIR}/libgsl.a"
)
add_dependencies(GSL gsl_ext)

add_library(GSLCBLAS STATIC IMPORTED)
set_target_properties(GSLCBLAS PROPERTIES
    IMPORTED_LOCATION "${GSL_LIB_DIR}/libgslcblas.a"
)
add_dependencies(GSLCBLAS gsl_ext)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL      # Provided by find_package(OpenGL)
        glfw            # Provided by FetchContent for GLFW
        glad            # Provided by FetchContent for GLAD
        glm::glm        # GLM
        GSL
        GSLCBLAS
)

# On some Linux distros, you may also need X11, pthread, dl, etc.:
# target_link_libraries(${PROJECT_NAME} PRIVATE X11 pthread dl)

# ------------------------------------------------------------------------
# Notes on using gnuplot:
# ------------------------------------------------------------------------
# gnuplot is a standalone program, not a library you link.
# After building, the gnuplot executable will reside at:
#   ${CMAKE_CURRENT_BINARY_DIR}/gnuplot/install/bin/gnuplot
#
# You can invoke this from your code with system() calls or from scripts.
#
# If you need an actual find script:
# find_program(GNUPLOT_EXE gnuplot HINTS "${CMAKE_CURRENT_BINARY_DIR}/gnuplot/install/bin")
# if(NOT GNUPLOT_EXE)
#     message(FATAL_ERROR "gnuplot not found!")
# endif()
# message(STATUS "Using gnuplot at: ${GNUPLOT_EXE}")
