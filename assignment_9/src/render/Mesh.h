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
#include <limits>
#include "gl.h"
#include "../utils/vec.h"

//== CLASS DEFINITION =========================================================


/// \class Mesh Mesh.h
/// This class represents a simple triangle mesh, stored as an indexed face set,
/// i.e., as an array of vertices and an array of triangles.
class Mesh
{
public:
    /// a vertex consists of a position and a normal
    struct Vertex {

        /// vertex position
        vec3 position;
        /// vertex normal
        vec3 normal;
    };

    /// a triangle is specified by three indices and a normal
    struct Triangle
    {
        Triangle() { }
        Triangle(int i0, int i1, int i2) : i0(i0), i1(i1), i2(i2) { }

        /// index of first vertex (for array Mesh::vertices_)
        int i0;
        /// index of second vertex (for array Mesh::vertices_)
        int i1;
        /// index of third vertex (for array Mesh::vertices_)
        int i2;
        /// triangle normal
        vec3 normal;
    };
    using Face = Triangle;

    /// This type is used to choose between flat shading and Phong shading
    enum Draw_mode {FLAT, PHONG};

    Mesh() { }
    Mesh(std::vector<vec3> const& new_pts,  std::vector<Face> const& new_faces) { set(new_pts, new_faces); }

    // Construct a mesh by reading an OFF file from offPath
    Mesh(const std::string &offPath) { read(offPath); }

    void set(std::vector<vec3> const& new_pts,  std::vector<Face> const& new_faces);

    void clean() {
        for (GLuint &b : bufferObjects) {
            if(b) glDeleteBuffers(1, &b);
            b = 0;
        }
        if (vao) glDeleteVertexArrays(1, &vao);
    }

    void draw(GLenum mode) {
        glBindVertexArray(vao);
        glDrawElements(mode, n_indices, GL_UNSIGNED_INT, NULL);
    }

    std::pair<vec3, vec3> axis_aligned_bounding_box() const {
        vec3 top_left (std::numeric_limits<double>::max());
        vec3 bot_right(std::numeric_limits<double>::lowest());

        for (const Vertex &v: vertices_) {
            top_left  = min(top_left , v.position);
            bot_right = max(bot_right, v.position);
        }

        return {top_left, bot_right};
    }

    void print_info() const {
        auto bbox = axis_aligned_bounding_box();
        std::cout << "Mesh: " << vertices_.size() << " vertices, " << triangles_.size() << " triangles\n";
        std::cout << "Bounding box:\n   " << bbox.first << "\n  " << bbox.second << std::endl;
    }

    ~Mesh() { clean(); }

private:
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

    /// Array of vertices
    std::vector<Vertex> vertices_;

    /// Array of triangles
    std::vector<Triangle> triangles_;
};


//=============================================================================
#endif // MESH_H defined
//=============================================================================
