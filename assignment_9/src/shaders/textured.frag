#version 140

uniform sampler2D tex;
in vec2 v2f_tex_coords;
out vec4 f_color;	// Final color output produced by fragment shader.
					// (Can name this anything you want...)

void main() {
	f_color = vec4(texture(tex, v2f_tex_coords).rgb, 1.0);
}
