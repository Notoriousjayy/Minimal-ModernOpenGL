#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

/**
 * Minimal class to load, compile, link a GLSL program,
 * and set uniform variables. Requires GL 3.3+.
 */
class GLSLProgram
{
public:
    GLSLProgram();
    ~GLSLProgram();

    bool create(const char* vertFile, const char* fragFile);
    void use() const;
    static void stop(); // unbind any shader program

    GLuint getID() const { return programID_; }

    // Uniform setters:
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, const glm::vec3& vec);
    void setUniform(const std::string& name, const glm::mat4& mat);

private:
    GLuint programID_;
    bool   linked_;

    bool compileShader(GLuint shaderID, const char* filePath);
    std::string readFile(const char* filePath);
    GLint getUniformLocation(const std::string& name) const;
};

#endif // GLSLPROGRAM_H
