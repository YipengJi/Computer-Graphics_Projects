//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;

out vec2 v2f_texcoord;

uniform mat4 modelview_projection_matrix;
uniform float t;    // Simulation time (for animating the sun)

void main()
{
    // pass through texture coordinate
    v2f_texcoord = v_texcoord;

    /** \todo (optional) Make your sun surface look like a fluid.
    *   - compute, where your incoming vertex is on the sphere by converting its position in spherical angles
    *   - add a new uniform `time` parameter to your shader
    *   - change the position of the vertex depending on its angles and `time` with a combination of sin and cos
    *   - experiment with different combinations of amplitude and frequency until you have a nice flow-like effect
    **/
    gl_Position = modelview_projection_matrix * v_position;

}
