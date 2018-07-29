//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef SPHERE_H
#define SPHERE_H


//== INCLUDES =================================================================

#include "Object.h"
#include "vec3.h"


//== CLASS DEFINITION =========================================================


/// \class Sphere Sphere.h
/// This class implements a sphere object, which is specified by its center
/// and its radius. This class overrides the intersection method Object::intersect().
class Sphere : public Object
{
public:
    /// Construct a sphere by specifying center and radius
    Sphere(const vec3& _center=vec3(0,0,0), double _radius=1);

    /// Construct a sphere with parameters parsed from an input stream.
    Sphere(std::istream &is) { parse(is); }

    /// Compute the intersection of the sphere with \c _ray. Return whether
    /// there is an intersection. If there is one, return the intersection data.
    /// This function overrides Object::intersect().
	/// \param[in] _ray the ray to intersect the plane with
	/// \param[out] _intersection_point position of the intersection
	/// \param[out] _intersection_normal normal vector at the intersection point
	/// \param[out] _intersection_t ray parameter at the intesection point
    virtual bool intersect(const Ray&  _ray,
                           vec3&       _intersection_point,
                           vec3&       _intersection_normal,
                           double&     _intersection_t) const override;

    /// parse sphere from an input stream
    virtual void parse(std::istream &is) override {
        is >> center >> radius >> material;
    }

private:
	/// center position of the sphere
    vec3   center;

	/// radius of the sphere
    double radius;
};

//=============================================================================
#endif // SPHERE_H defined
//=============================================================================
