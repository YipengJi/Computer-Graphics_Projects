#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 v_position;		// bind v_position to attribute 0
layout (location = 1) in vec2 v_tex_coords;		// bind v_position to attribute 1

uniform vec2 viewer_position;
uniform vec2 viewer_scale;

out vec2 v2f_tex_coords;

const vec2 texture_coord_center = vec2(0.5, 0.5);

void main() {
	vec2 local_coord = (v_tex_coords - texture_coord_center) * viewer_scale + texture_coord_center;
	v2f_tex_coords = viewer_position + local_coord;

	gl_Position = vec4(v_position, 1.0);
}
