//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef OBJECT_H
#define OBJECT_H

//== INCLUDES =================================================================

#include "Ray.h"
#include "vec3.h"
#include "Material.h"

#include <stdexcept>
#include <limits>


//== CLASS DEFINITION =========================================================


/// \class Object Object.h
/// This class implements an abstract class for an object.
/// Every derived object type will inherit the material property, and it
/// will have to override the virtual function Object::intersect().
struct Object
{
public:
    /// Default (empty) constructor
    Object() {}

    /// Destructor (if a class has virtual functions, then its destructor
    /// has to be virtual as well).
    virtual ~Object() {}

    /// Intersect the object with \c _ray, return whether there is an intersection.
    /// If \c _ray intersects the object, provide the following results:
    /// \param[in] _ray the ray to intersect the object with
    /// \param[out] _intersection_point the point of intersection
    /// \param[out] _intersection_normal the surface normal at intersection point
    /// \param[out] _intersection_t ray parameter at intersection point
    virtual bool intersect(const Ray&  _ray,
                           vec3&       _intersection_point,
                           vec3&       _intersection_normal,
                           double&     _intersection_t) const = 0;

    /// parse object properties from an input stream
    virtual void parse(std::istream &is) { throw std::logic_error("Unimplemented"); }

    /// The material of this object
    Material material;

    static constexpr double NO_INTERSECTION = std::numeric_limits<double>::max();
};

/// read object from stream
inline std::istream& operator>>(std::istream& is, Object& s)
{
    s.parse(is);
    return is;
}


/// This typedef is for convencience only
typedef Object* Object_ptr;


//=============================================================================
#endif // OBJECT_H defined
//=============================================================================
