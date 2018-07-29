#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "gl.h"
#include "texture.h"

class Billboard
{
public:
    /// render the billboard
    void draw();

    // Texture for billboard.
    Texture tex_;

private:
    void initialize();

    /// indices of the triangle vertices
    unsigned int n_indices_ = 0;

    // vertex array object
    GLuint vao_;
    /// vertex buffer object
    GLuint vbo_;
    GLuint nbo_;
    /// texture coordinates buffer object
    GLuint tbo_;
    /// index buffer object
    GLuint ibo_;
};

#endif

