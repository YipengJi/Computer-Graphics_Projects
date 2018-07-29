//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 v_position;

uniform mat4 scene_modelview_matrix;
uniform mat4 modelview_projection_matrix;

out vec3 v2f_scene_eye_vertex;
out vec3 v2f_untransformed_vertex;

void main() {
    v2f_untransformed_vertex = v_position;
    v2f_scene_eye_vertex     = (scene_modelview_matrix * vec4(v_position, 1)).xyz;
    gl_Position = modelview_projection_matrix * vec4(v_position, 1);
}
