//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;

out vec3 v2f_normal;
out vec3 v2f_light;
out vec3 v2f_view;
out vec2 v2f_texcoord;

uniform mat4 modelview_projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform vec4 light_position; //in eye space coordinates already



void main()
{

    // \todo Paste your Earth vertex shading code from assignment 6 here.
	vec3 translate = vec3(modelview_matrix[3][0],modelview_matrix[3][1],modelview_matrix[3][2]);
	
	v2f_texcoord = v_texcoord;
	v2f_normal	 = normalize(normal_matrix * v_normal);
	v2f_light	 = normalize(vec3(light_position) - translate);
	v2f_view 	 = normalize(-translate);
	
	gl_Position = modelview_projection_matrix * v_position;
}
