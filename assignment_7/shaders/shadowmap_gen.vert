//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 v_position;

uniform mat4 modelview_matrix;
uniform mat4 modelview_projection_matrix;

out vec3 v2f_lc_vertex; // vertex position in "eye-coordinates" for the light

void main() {
    v2f_lc_vertex = (modelview_matrix * vec4(v_position, 1)).xyz;
    gl_Position = modelview_projection_matrix * vec4(v_position, 1);
    gl_Position.x = -gl_Position.x; // Flip texture writes horizontally since we're drawing from the inside of the cube, and the +u texture axis actually points left
    gl_Position.y = -gl_Position.y; // Flip texture writes vertically since cube map textures are accessed upside down; see https://stackoverflow.com/a/12825633/122710
}
