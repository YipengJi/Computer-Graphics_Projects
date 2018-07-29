//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#version 140

// Eye-space fragment position
in vec3 v2f_scene_eye_vertex;
in vec3 v2f_untransformed_vertex;

uniform vec3 light_position_scene_eye;  // In scene eye space coordinates already
uniform bool demo_texture_mode;
uniform samplerCube cube_map;

out vec4 f_color;

void main() {
    vec4 color = texture(cube_map, v2f_scene_eye_vertex - light_position_scene_eye);
    if (!demo_texture_mode) color.rgb = 0.25 * vec3(color.r);

    if (max(abs(v2f_untransformed_vertex.x), abs(v2f_untransformed_vertex.y)) > 0.98)
        color = vec4(1.0, 0.5, 0.0, 1.0);
    f_color = color;
}
