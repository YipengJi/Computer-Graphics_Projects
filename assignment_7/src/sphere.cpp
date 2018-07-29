//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "sphere.h"
#include "glmath.h"
#include <vector>
#include <math.h>

//=============================================================================


Sphere::Sphere(unsigned int resolution)
    : resolution_(resolution)
{
}


//-----------------------------------------------------------------------------


Sphere::~Sphere()
{
    if (vbo_)  glDeleteBuffers(1, &vbo_);
    if (nbo_)  glDeleteBuffers(1, &nbo_);
    if (tan_bo_)  glDeleteBuffers(1, &tan_bo_);
    if (bitan_bo_)  glDeleteBuffers(1, &bitan_bo_);
    if (tbo_)  glDeleteBuffers(1, &tbo_);
    if (ibo_)  glDeleteBuffers(1, &ibo_);
    if (vao_)  glDeleteVertexArrays(1, &vao_);
}


//-----------------------------------------------------------------------------


void Sphere::initialize()
{
    const unsigned int v_resolution =     resolution_;
    const unsigned int u_resolution = 2 * resolution_;
    const unsigned int n_vertices   = (v_resolution) * u_resolution;
    const unsigned int n_triangles  = 2 * (v_resolution-1) * (u_resolution-1);

    std::vector<GLfloat> positions(3*n_vertices);
    std::vector<GLfloat>   normals(3*n_vertices);
    std::vector<GLfloat> texcoords(2*n_vertices);
    std::vector<GLuint >   indices(3*n_triangles);

    unsigned int p(0), n(0), t(0), i(0), tan(0), bitan(0);

    // generate vertices
    for (unsigned int iv=0; iv<v_resolution; ++iv)
    {
        for (unsigned int iu=0; iu<u_resolution; ++iu)
        {
            float u = (float) iu / (float) (u_resolution-1);
            float v = (float) iv / (float) (v_resolution-1);

            float theta = u * 2.0 * M_PI;
            float phi   = v * M_PI;

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            positions[p++] = x;
            positions[p++] = y;
            positions[p++] = z;

            normals[n++] = x;
            normals[n++] = y;
            normals[n++] = z;

            texcoords[t++] = 1.0-u;
            texcoords[t++] = 1.0-v;
        }
    }


    // generate triangles
    for (unsigned int v=0; v<v_resolution-1; ++v)
    {
        for (unsigned int u=0; u<u_resolution-1; ++u)
        {
            unsigned int i0 = (u  ) + (v  ) * u_resolution;
            unsigned int i1 = (u+1) + (v  ) * u_resolution;
            unsigned int i2 = (u+1) + (v+1) * u_resolution;
            unsigned int i3 = (u  ) + (v+1) * u_resolution;

            indices[i++] = i0;
            indices[i++] = i1;
            indices[i++] = i2;

            indices[i++] = i0;
            indices[i++] = i2;
            indices[i++] = i3;
        }
    }
    n_indices_ = 3*n_triangles;


    // generate vertex array object
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);


    // vertex positions -> attribute 0
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, 3*n_vertices*sizeof(float), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // normal vectors -> attribute 1
    glGenBuffers(1, &nbo_);
    glBindBuffer(GL_ARRAY_BUFFER, nbo_);
    glBufferData(GL_ARRAY_BUFFER, 3*n_vertices*sizeof(float), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // texture coordinates -> attribute 2
    glGenBuffers(1, &tbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tbo_);
    glBufferData(GL_ARRAY_BUFFER, 2*n_vertices*sizeof(float), &texcoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // triangle indices
    glGenBuffers(1, &ibo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*n_triangles*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}


//-----------------------------------------------------------------------------


void Sphere::draw(GLenum mode)
{
    if (n_indices_ == 0) initialize();

    glBindVertexArray(vao_);
    glDrawElements(mode, n_indices_, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}


//=============================================================================
