#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 v_position; // bind v_position to attribute 0

void main() {
    gl_Position = vec4(v_position, 1.0);
}
