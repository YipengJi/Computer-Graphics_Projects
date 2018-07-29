//=============================================================================
//                                                                            
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//                                                                            
//=============================================================================

#ifndef RAY_H
#define RAY_H


//== INCLUDES =================================================================

#include "vec3.h"


//== CLASS DEFINITION =========================================================


/// \class Ray Ray.h
/// This class implements a ray, specified by its origin and direction.
/// It provides a convenient function to compute the point ray(t) at a specific
/// ray paramter t.
class Ray
{
public:

	/// Constructor with origin and direction. Direction will be normalized.
    Ray(const vec3& _origin    = vec3(0,0,0),
        const vec3& _direction = vec3(0,0,1))
    {
        origin    = _origin;
        direction = normalize(_direction); // normalize direction
    }

	/// Compute the point on the ray at the parameter \c _t, which is
    /// origin + _t*direction.
    vec3 operator()(double _t) const
    {
        return origin + _t*direction;
    }
    
    
public:
    
    /// origin of the ray
    vec3 origin;
    /// direction of the ray (should be normalized)
    vec3 direction;
};


//-----------------------------------------------------------------------------


/// read ray from stream
inline std::istream& operator>>(std::istream& is, Ray& r)
{
    is >> r.origin >> r.direction;
    return is;
}


//=============================================================================
#endif // RAY_H defined
//=============================================================================
