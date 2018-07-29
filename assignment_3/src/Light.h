//=============================================================================
//                                                                            
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//                                                                            
//=============================================================================

#ifndef LIGHT_H
#define LIGHT_H


//== INCLUDES =================================================================

#include "vec3.h"


//== CLASS DEFINITION =========================================================

/// \class Light Light.h
/// The class represents a light source and stores position and color.
struct Light
{
    Light(std::istream &is) { is >> position >> color; }

    /// position of the light source
    vec3 position;

    /// color of the light source
    vec3 color;
};


//-----------------------------------------------------------------------------


/// read light data from stream
inline std::istream& operator>>(std::istream& is, Light& l)
{
    l = Light(is);
    return is;
}


//=============================================================================
#endif // LIGHT_H defined
//=============================================================================
