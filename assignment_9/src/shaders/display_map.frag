#version 140
#pragma optionNV unroll all

in vec2 v2f_tex_coords;
out vec4 f_color;	// Final color output produced by fragment shader.
					// (Can name this anything you want...)

vec3 tex_map(vec2 point); // Implemented in noise.frag

void main() {
	vec3 color = tex_map(v2f_tex_coords);
		
	f_color = vec4(color, 1.0);
} 
