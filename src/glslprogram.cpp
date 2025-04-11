#include "glslprogram.h"
#include <iostream>
#include <fstream>
#include <sstream>

GLSLProgram::GLSLProgram()
: programID_(0),
  linked_(false)
{
}

GLSLProgram::~GLSLProgram()
{
    if(programID_ != 0) {
        glDeleteProgram(programID_);
    }
}

bool GLSLProgram::create(const char* vertFile, const char* fragFile)
{
    if(programID_ == 0) {
        programID_ = glCreateProgram();
    }

    // Vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    if(!compileShader(vs, vertFile)) {
        glDeleteShader(vs);
        return false;
    }
    glAttachShader(programID_, vs);

    // Fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    if(!compileShader(fs, fragFile)) {
        glDeleteShader(fs);
        return false;
    }
    glAttachShader(programID_, fs);

    // Link program
    glLinkProgram(programID_);
    GLint status;
    glGetProgramiv(programID_, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        char log[1024];
        glGetProgramInfoLog(programID_, 1024, nullptr, log);
        std::cerr << "Program link error:\n" << log << std::endl;
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }

    // Detach & delete shaders
    glDetachShader(programID_, vs);
    glDetachShader(programID_, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    linked_ = true;
    return true;
}

bool GLSLProgram::compileShader(GLuint shaderID, const char* filePath)
{
    std::string source = readFile(filePath);
    if(source.empty()) {
        std::cerr << "Could not load shader file: " << filePath << std::endl;
        return false;
    }

    const char* srcPtr = source.c_str();
    glShaderSource(shaderID, 1, &srcPtr, nullptr);
    glCompileShader(shaderID);

    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        char log[1024];
        glGetShaderInfoLog(shaderID, 1024, nullptr, log);
        std::cerr << "Shader compile error (" << filePath << "):\n" << log << std::endl;
        return false;
    }
    return true;
}

std::string GLSLProgram::readFile(const char* filePath)
{
    std::ifstream ifs(filePath);
    if(!ifs) {
        return std::string();
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

void GLSLProgram::use() const
{
    if(linked_) {
        glUseProgram(programID_);
    }
}

void GLSLProgram::stop()
{
    glUseProgram(0);
}

GLint GLSLProgram::getUniformLocation(const std::string& name) const
{
    GLint loc = glGetUniformLocation(programID_, name.c_str());
    // Optional: you can warn if loc < 0
    return loc;
}

void GLSLProgram::setUniform(const std::string& name, float value)
{
    GLint loc = getUniformLocation(name);
    if(loc >= 0) {
        glUniform1f(loc, value);
    }
}

void GLSLProgram::setUniform(const std::string& name, int value)
{
    GLint loc = getUniformLocation(name);
    if(loc >= 0) {
        glUniform1i(loc, value);
    }
}

void GLSLProgram::setUniform(const std::string& name, const glm::vec3& vec)
{
    GLint loc = getUniformLocation(name);
    if(loc >= 0) {
        glUniform3f(loc, vec.x, vec.y, vec.z);
    }
}

void GLSLProgram::setUniform(const std::string& name, const glm::mat4& mat)
{
    GLint loc = getUniformLocation(name);
    if(loc >= 0) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
    }
}
