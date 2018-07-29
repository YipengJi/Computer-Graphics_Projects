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


    return false;
}


//=============================================================================
