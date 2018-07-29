//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================


//== INCLUDES =================================================================

#include "Plane.h"
#include <limits>


//== CLASS DEFINITION =========================================================



Plane::Plane(const vec3& _center, const vec3& _normal)
: center(_center), normal(_normal)
{
}


//-----------------------------------------------------------------------------


bool
Plane::
intersect(const Ray& _ray,
          vec3&      _intersection_point,
          vec3&      _intersection_normal,
          double&    _intersection_t ) const
{

    // \todo Copy your assignment 1 solution here.
    
    //(o+td) * n = 0
	double dot_no = dot((center - _ray.origin) , normal);
	double dot_nd = dot(normal, _ray.direction);

	if (dot_nd < 1e-7 && dot_nd > -1e-7) 
		return false;
	
	_intersection_t 		= 	dot_no/dot_nd;
	_intersection_point 	= 	_ray(_intersection_t);
	_intersection_normal	= 	normal;

	return (_intersection_t > 0) ? true : false;

    return false;
}


//=============================================================================
