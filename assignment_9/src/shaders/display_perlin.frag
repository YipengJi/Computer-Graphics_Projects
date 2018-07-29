#version 140

in vec2 v2f_tex_coords;
out vec4 f_color;	// Final color output produced by fragment shader.
					// (Can name this anything you want...)

float perlin_noise(vec2 point); // Implemented in noise.frag

void main() {
	float freq = 23.15;
 	float noise_val = perlin_noise(v2f_tex_coords * freq) + 0.5;

	f_color = vec4(noise_val, noise_val, noise_val, 1.0);
} 
