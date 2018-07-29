#include "billboard.h"
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <cassert>
#include <algorithm>

Billboard::~Billboard() {
    if (vbo_)  glDeleteBuffers(1, &vbo_);
    if (nbo_)  glDeleteBuffers(1, &nbo_);
    if (tbo_)  glDeleteBuffers(1, &tbo_);
    if (ibo_)  glDeleteBuffers(1, &ibo_);
    if (vao_)  glDeleteVertexArrays(1, &vao_);
}

void Billboard::draw()
{
    if (n_indices_ == 0) initialize();

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, n_indices_, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void Billboard::initialize()
{
    std::array<GLfloat, 3 * 4>  positions;
    std::array<GLuint , 3 * 2>    indices;
    std::array<GLfloat, 2 * 4> tex_coords;

    unsigned int p(0), n(0), t(0), i(0);

    // generate vertices
    positions[0] = -1.0f;
    positions[1] = 1.0f;
    positions[2] = 0.0f;

    tex_coords[0] = 0.0f;
    tex_coords[1] = 1.0f;

    positions[3 + 0] = 1.0f;
    positions[3 + 1] = 1.0f;
    positions[3 + 2] = 0.0f;

    tex_coords[2 + 0] = 1.0f;
    tex_coords[2 + 1] = 1.0f;

    positions[6 + 0] = -1.0f;
    positions[6 + 1] = -1.0f;
    positions[6 + 2] = 0.0f;

    tex_coords[4 + 0] = 0.0f;
    tex_coords[4 + 1] = 0.0f;

    positions[9 + 0] = 1.0f;
    positions[9 + 1] = -1.0f;
    positions[9 + 2] = 0.0f;

    tex_coords[6 + 0] = 1.0f;
    tex_coords[6 + 1] = 0.0f;

    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;

    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;

    n_indices_ = 6;

    // generate vertex array object
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // vertex positions -> attribute 0
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4*sizeof(float), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // normal vectors -> attribute 1
    glGenBuffers(1, &nbo_);
    glBindBuffer(GL_ARRAY_BUFFER, nbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4*sizeof(float), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // texture coordinates -> attribute 2
    glGenBuffers(1, &tbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tbo_);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4*sizeof(float), &tex_coords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // triangle indices
    glGenBuffers(1, &ibo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}
