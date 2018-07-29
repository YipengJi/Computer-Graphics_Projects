//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef SPHERE_H
#define SPHERE_H
//=============================================================================

#include "gl.h"

//=============================================================================

/// class that creates a sphere with a desired tessellation degree and renders it
class Sphere
{
public:

    /// default constructor
    /// \param resolution the degree of the tessellation of the sphere
    Sphere(unsigned int resolution=10);

    /// destructor
    ~Sphere();

    /// render mesh of the sphere
    void draw(GLenum mode=GL_TRIANGLES);


private:

    /// generate sphere vertices/triangles and OpenGL buffers
    void initialize();


private:

    /// tessellation resolution
    unsigned int resolution_;
    /// indices of the triangle vertices
    unsigned int n_indices_;

    // vertex array object
    GLuint vao_;
    /// vertex buffer object
    GLuint vbo_;
    /// normals buffer object
    GLuint nbo_;
    /// tangents buffer object
    GLuint tan_bo_;
    /// bitangents buffer object
    GLuint bitan_bo_;
    /// texture coordinates buffer object
    GLuint tbo_;
    /// index buffer object
    GLuint ibo_;
};


//=============================================================================
#endif
//=============================================================================
