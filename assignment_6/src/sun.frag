//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140

in  vec2 v2f_texcoord;
out vec4 f_color;

uniform sampler2D tex;
uniform bool greyscale;


void main()
{
    // fetch color (rgb and alpha) from texture
    vec4 color = texture(tex, v2f_texcoord.st);

    // For greyscale mode, convert RGB color to YUV and use only the luminance
    if (greyscale) color.rgb = vec3(0.299 * color.r
                                  + 0.587 * color.g
                                  + 0.114 * color.b);

    f_color = color;
}
