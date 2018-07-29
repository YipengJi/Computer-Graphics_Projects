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

#include "Object.h"
#include <vector>
#include <string>

//== CLASS DEFINITION =========================================================


/// \class Mesh Mesh.h
/// This class represents a simple triangle mesh, stored as an indexed face set,
/// i.e., as an array of vertices and an array of triangles.
class Mesh : public Object
{
public:

    /// This type is used to choose between flat shading and Phong shading
    enum Draw_mode {FLAT, PHONG};

    /// Construct a mesh by parsing its path and properties from an input
    /// stream. The mesh path read from the file is relative to the 
    /// scene file's path "scenePath".
    Mesh(std::istream &is, const std::string &scenePath);

    /// Intersect mesh with ray (calls ray-triangle intersection)
    /// If \c _ray intersects a face of the mesh, it provides the following results:
    /// \param[in] _ray the ray to intersect the mesh with
    /// \param[out] _intersection_point the point of intersection
    /// \param[out] _intersection_normal the surface normal at intersection point
    /// \param[out] _intersection_t ray parameter at the intersection point
    virtual bool intersect(const Ray& _ray,
                           vec3&      _intersection_point,
                           vec3&      _intersection_normal,
                           double&    _intersection_t) const override;

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

public:
    /// Read mesh from an OFF file
    bool read(const std::string &_filename);

    /// Compute normal vectors for triangles and vertices
    void compute_normals();

    /// Compute the axis-aligned bounding box, store minimum and maximum point in bb_min_ and bb_max_
    void compute_bounding_box();

    /// Does \c _ray intersect the bounding box of the mesh?
    bool intersect_bounding_box(const Ray& _ray) const;

    /// Intersect a triangle with a ray. Return whether there is an intersection.
    /// If there is an intersection, store intersection data.
    /// This function overrides Object::intersect().
    /// \param[in] _triangle the triangle to be intersected
    /// \param[in] _ray the ray to intersect the triangle with
    /// \param[out] _intersection_point the point of intersection
    /// \param[out] _intersection_normal the surface normal at intersection point
    /// \param[out] _intersection_t ray parameter at the intersection point
    bool intersect_triangle(const Triangle&  _triangle,
                            const Ray&       _ray,
                            vec3&            _intersection_point,
                            vec3&            _intersection_normal,
                            double&          _intersection_t) const;

private:
    /// Does this mesh use flat or Phong shading?
    Draw_mode draw_mode_;

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
