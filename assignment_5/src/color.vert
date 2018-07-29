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


void main() {
    // pass through texture coordinate
    v2f_texcoord = v_texcoord;

    // Compute vertices' normalized device coordinates
    gl_Position = modelview_projection_matrix * v_position;
}
