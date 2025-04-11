#include <iostream>

// 1) Prevent GLFW from including its own GL headers.
#define GLFW_INCLUDE_NONE

// 2) Include GLAD first:
#include <glad/glad.h>

// 3) Then include GLFW:
#include <GLFW/glfw3.h>

// Now include GLM (no conflict with GL):
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Finally, include your own headers:
#include "glslprogram.h"
#include "vertexarray.h"

// Window dimensions
static const int WIDTH = 800;
static const int HEIGHT = 600;

static void errorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error [" << error << "]: " << description << std::endl;
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int, char**)
{
    // Setup GLFW
    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n";
        return -1;
    }

    // Request OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For macOS compatibility, uncomment:
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
                                          "Modern OpenGL Example", nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Load OpenGL functions via GLAD (must be done after context is current).
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD.\n";
        return -1;
    }

    // Print some info
    std::cout << "OpenGL Vendor:   " << glGetString(GL_VENDOR)   << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version:  " << glGetString(GL_VERSION)  << std::endl;

    // Compile and link our shader
    GLSLProgram shader;
    if(!shader.create("../shaders/simple.vert", "../shaders/simple.frag")) {
        std::cerr << "Failed to create shader.\n";
        return -1;
    }

    // Build a colorful triangle
    VertexArray triangle;
    triangle.begin(GL_TRIANGLES);

    triangle.addVertex(-0.5f, -0.5f, 0.0f,  1.0f, 0.f, 0.f);
    triangle.addVertex( 0.5f, -0.5f, 0.0f,  0.f, 1.0f, 0.f);
    triangle.addVertex( 0.0f,  0.5f, 0.0f,  0.f, 0.f, 1.0f);

    triangle.end();

    // Enable depth testing if needed
    glEnable(GL_DEPTH_TEST);

    // Main render loop
    while(!glfwWindowShouldClose(window)) {
        // Clear
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        shader.use();

        // Create an MVP matrix
        float aspect = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view  = glm::lookAt(glm::vec3(0,0,2),
                                      glm::vec3(0,0,0),
                                      glm::vec3(0,1,0));
        glm::mat4 proj  = glm::perspective(glm::radians(60.0f),
                                           aspect, 0.1f, 100.0f);

        glm::mat4 mvp   = proj * view * model;

        shader.setUniform("uMVP", mvp);

        // Draw
        triangle.draw();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
