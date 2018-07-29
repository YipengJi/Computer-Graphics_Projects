//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef MESH_H
#define MESH_H


//== INCLUDES =================================================================

#include <vector>
#include <string>
#include <array>
#include "glmath.h"
#include "gl.h"

//== CLASS DEFINITION =========================================================


/// \class Mesh Mesh.h
/// This class represents a simple triangle mesh, stored as an indexed face set,
/// i.e., as an array of vertices and an array of triangles.
class Mesh
{
public:

    /// This type is used to choose between flat shading and Phong shading
    enum Draw_mode {FLAT, PHONG};

    // Construct a mesh by reading an OFF file from offPath
    Mesh(const std::string &offPath) { read(offPath); }

    void clean() {
        for (GLuint &b : bufferObjects) {
            if(b) glDeleteBuffers(1, &b);
            b = 0;
        }
        if (vao) glDeleteVertexArrays(1, &vao);
    }

    void draw() {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, NULL);
    }

    // A model matrix that scales and translates the mesh to fit within
    // the bounding box (-1, 0, -1), (1, 2, 1) (to position it on a floor at "y = 0")
    mat4 modelMatrix() const {
        float scaleFactor = 2.0 / linf_norm(bb_max_ - bb_min_);

        vec3 translation(-0.5 * (bb_min_[0] + bb_max_[0]),  // Center x component around origin
                         -bb_min_[1],                       // Place object on the floor (y = 0)
                         -0.5 * (bb_min_[2] + bb_max_[2])); // Center y component around origin

        return mat4::scale(scaleFactor) * mat4::translate(translation);
    }

    ~Mesh() { clean(); }

private:
    /// a vertex consists of a position and a normal
    struct Vertex
    {
        /// vertex position
        vec3 position;
        /// vertex normal
        vec3 normal;
    };

    /// a triangle is specified by three indices and a normal
    struct Triangle
    {
        /// index of first vertex (for array Mesh::vertices_)
        int i0;
        /// index of second vertex (for array Mesh::vertices_)
        int i1;
        /// index of third vertex (for array Mesh::vertices_)
        int i2;
        /// triangle normal
        vec3 normal;
    };

    // OpenGL buffers
    enum { VTX_BUFFER = 0, NORMAL_BUFFER = 1, INDEX_BUFFER = 2 };
    GLuint vao = 0;
    size_t n_indices;
    std::array<GLuint, 3> bufferObjects{{0, 0, 0}};

    void m_generate_vao();

public:
    /// Read mesh from an OFF file
    void read(const std::string &_filename);

    /// Compute normal vectors for triangles and vertices
    void compute_normals();

    /// Compute the axis-aligned bounding box, store minimum and maximum point in bb_min_ and bb_max_
    void compute_bounding_box();

    /// Array of vertices
    std::vector<Vertex> vertices_;

    /// Array of triangles
    std::vector<Triangle> triangles_;

    /// Minimum point of the bounding box
    vec3 bb_min_;
    /// Maximum point of the bounding box
    vec3 bb_max_;
};


//=============================================================================
#endif // MESH_H defined
//=============================================================================
