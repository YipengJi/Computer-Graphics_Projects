//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef CYLINDER_H
#define CYLINDER_H


//== INCLUDES =================================================================

#include "Object.h"
#include "vec3.h"


//== CLASS DEFINITION =========================================================


/// \class Cylinder Cylinder.h
/// This class implements a cylinder object, which is specified by its center,
//  unit axis vector, radius, and height.
/// This class overrides the intersection method Object::intersect().
class Cylinder : public Object
{
public:
    /// Construct a cylinder by directly specifying its parameters
    Cylinder(const vec3 &_center = vec3(0,0,0),
             double _radius = 1,
             const vec3 &_axis = vec3(1,0,0),
             double _height = 1)
        :  center(_center), radius(_radius), axis(_axis), height(_height) { }

    /// Construct a cylinder with parameters parsed from an input stream.
    Cylinder(std::istream &is) { parse(is); }

    /// Compute the intersection of the cylinder with \c _ray. Return whether
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

    /// parse cylinder from an input stream
    virtual void parse(std::istream &is) override {
        is >> center >> radius >> axis >> height >> material;
        axis = normalize(axis);
    }

private:
	/// center position
    vec3 center;

	/// unit axis vector
    vec3 axis;

	/// radius
    double radius;

	/// height
    double height;
};

//=============================================================================
#endif // CYLINDER_H defined
//=============================================================================
