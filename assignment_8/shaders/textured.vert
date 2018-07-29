#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 v_position;   // bind v_position to attribute 0
layout (location = 1) in vec2 v_tex_coords; // bind v_position to attribute 1

out vec2 v2f_tex_coords;

void main() {
    v2f_tex_coords = v_tex_coords;
    gl_Position = vec4(v_position, 1.0);
}
