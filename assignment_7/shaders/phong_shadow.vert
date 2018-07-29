//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;

// Vertex shader computes eye-space vertex position and normals only.
out vec3 v2f_ec_vertex;
out vec3 v2f_normal;

uniform mat4 modelview_projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

void main()
{
    // vertex & light in eye coordinates
    v2f_ec_vertex = (modelview_matrix * v_position).xyz;

    // transform normal to eye coordinates, pass to fragment shader
    v2f_normal = normalize(normal_matrix * v_normal);

    // transform vertex by modelview and projection matrix
    gl_Position = modelview_projection_matrix * v_position;
}
