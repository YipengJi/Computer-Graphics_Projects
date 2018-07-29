//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#version 140

uniform vec3 color;
out vec4 f_color;

void main()
{
    f_color = vec4(color, 1.0);
}
