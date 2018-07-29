//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef PLANE_H
#define PLANE_H

//== INCLUDES =================================================================

#include "Object.h"
#include "vec3.h"


//== CLASS DEFINITION =========================================================


/// \class Plane Plane.h
/// This class implements a simple plane object.
/// A plane is specified by a center point and a normal vector.
/// This class overrides the intersection method Object::intersect().
class Plane : public Object
{
public:
    /// constructor
    Plane(const vec3& _center=vec3(0,0,0), const vec3& _normal=vec3(0,1,0));

    /// Construct a plane with parameters parsed from an input stream.
    Plane(std::istream &is) { parse(is); }

    /// Compute the intersection of the plane with \c _ray. Return whether
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

    /// parse plane from an input stream
    virtual void parse(std::istream &is) override {
        is >> center >> normal >> material;
    }

private:
	/// one (arbitrary) point on the plane
    vec3 center;
	/// normal vector of the plane
	vec3 normal;
};

//=============================================================================
#endif // SPHERE_H defined
//=============================================================================
