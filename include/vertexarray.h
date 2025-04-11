#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <vector>
#include <cstddef>
#include <glad/glad.h>

/**
 * Simple struct storing 3D position & RGB color.
 * You can expand to include normals, texcoords, etc.
 */
struct Vertex {
    float px, py, pz;  // position
    float cr, cg, cb;  // color
};

/**
 * Collects a list of vertices and uploads them to a VAO+VBO.
 * Then you can draw them with draw().
 */
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    // Start building geometry with a specified draw mode (GL_TRIANGLES, etc.)
    void begin(GLenum mode);

    // Add a single vertex
    void addVertex(float x, float y, float z,
                   float r, float g, float b);

    // Upload data to GPU
    void end();

    // Render the VAO
    void draw() const;

private:
    std::vector<Vertex> vertices_;
    GLenum mode_;
    GLuint vaoID_;
    GLuint vboID_;
    bool   built_;
};

#endif // VERTEXARRAY_H
