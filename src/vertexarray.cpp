#include "vertexarray.h"

VertexArray::VertexArray()
: mode_(GL_TRIANGLES)
, vaoID_(0)
, vboID_(0)
, built_(false)
{
}

VertexArray::~VertexArray()
{
    if(vboID_) glDeleteBuffers(1, &vboID_);
    if(vaoID_) glDeleteVertexArrays(1, &vaoID_);
}

void VertexArray::begin(GLenum mode)
{
    mode_ = mode;
    vertices_.clear();
    built_ = false;
}

void VertexArray::addVertex(float x, float y, float z,
                            float r, float g, float b)
{
    Vertex v { x, y, z, r, g, b };
    vertices_.push_back(v);
}

void VertexArray::end()
{
    if(vertices_.empty()) {
        built_ = false;
        return;
    }

    built_ = true;

    // Create VAO
    glGenVertexArrays(1, &vaoID_);
    glBindVertexArray(vaoID_);

    // Create VBO
    glGenBuffers(1, &vboID_);
    glBindBuffer(GL_ARRAY_BUFFER, vboID_);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices_.size() * sizeof(Vertex),
                 vertices_.data(),
                 GL_STATIC_DRAW);

    // Enable and set up vertex attribute arrays
    // layout(location=0) -> position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, px));

    // layout(location=1) -> color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, cr));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::draw() const
{
    if(!built_ || vaoID_ == 0) return;

    glBindVertexArray(vaoID_);
    glDrawArrays(mode_, 0, (GLsizei)vertices_.size());
    glBindVertexArray(0);
}
