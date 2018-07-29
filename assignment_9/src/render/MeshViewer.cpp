//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "MeshViewer.h"
#include "../utils/vec.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <array>

//=============================================================================


MeshViewer::MeshViewer(std::string const& _title, int _width, int _height)
	: GLFW_window(_title.c_str(), _width, _height)
	, fovy_(90)
	, near_(0.01)
	, far_(1024)
	, x_angle_(50)
	, z_angle_(180)
	, camera_distance(0.85)
	, mesh(new Mesh)
	, actor(new StaticMeshActor(mesh))
{
	;
}

void MeshViewer::setMesh(std::shared_ptr<Mesh> new_mesh)
{
	mesh = new_mesh;
	actor->mesh = new_mesh;
}

//-----------------------------------------------------------------------------

void MeshViewer::scroll_wheel(double xoffset, double yoffset)
{
	float const scroll_amount = -yoffset * 0.1;
	camera_distance = std::min(std::max(camera_distance + scroll_amount, 0.1f), 30.0f);
}

void MeshViewer::keyboard(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		// Change view between the various bodies with keys 1..6
		switch (key)
		{
			case GLFW_KEY_LEFT:
			case GLFW_KEY_A:
			{
				z_angle_ -= 10.0;
				break;
			}

			case GLFW_KEY_RIGHT:
			case GLFW_KEY_D:
			{
				z_angle_ += 10.0;
				break;
			}

			case GLFW_KEY_DOWN:
			case GLFW_KEY_S:
			{
				x_angle_ += 10.0;
				break;
			}

			case GLFW_KEY_UP:
			case GLFW_KEY_W:
			{
				x_angle_ -= 10.0;
				break;
			}

			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
			{
				glfwSetWindowShouldClose(window_, GL_TRUE);
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------


void MeshViewer::resize(int _width, int _height)
{
	width_  = _width;
	height_ = _height;
	glViewport(0, 0, _width, _height);
}


//-----------------------------------------------------------------------------


void MeshViewer::initialize()
{
	// set initial state
	glClearColor(1,1,1,0);
	glEnable(GL_DEPTH_TEST);

	// setup shaders
	phong_shader_.load(SHADER_PATH "/terrain.vert", SHADER_PATH "/terrain.frag");
}
//-----------------------------------------------------------------------------


void MeshViewer::paint()
{
	// clear framebuffer and depth buffer first
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4  center(0, 0, 0, 1);
	mat4     rot = mat4::rotate_z(z_angle_) * mat4::rotate_x(x_angle_);
	vec4     eye = center + rot * vec4(0, 0, camera_distance, 0);
	vec4      up = rot * vec4(0, 1, 0, 0);
	mat4    view = mat4::look_at(vec3(eye), vec3(center), vec3(up));

	mat4 projection = mat4::perspective(fovy_, (float)width_/(float)height_, near_, far_);
	draw_scene(projection, view);
}

//-----------------------------------------------------------------------------


void MeshViewer::draw_scene(mat4& _projection, mat4& _view)
{
	if(!phong_shader_.is_valid()) {
		return;
	}

	vec4 light = vec4(0.6, 0.4, 2., 1.0); //in world coordinates
	light = _view * light;

	mat4 m_matrix = actor->model_matrix;
	mat4 mv_matrix  = _view * m_matrix;
	mat4 mvp_matrix = _projection * mv_matrix;
	// \todo Construct the matrix for transforming normals into eye coordinates
	//       You can paste in your solution from assignment 6.
	mat3 n_matrix    = n_matrix = transpose(inverse(mat3(mv_matrix)));

	phong_shader_.use();
	phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
	phong_shader_.set_uniform("modelview_matrix", mv_matrix);
	phong_shader_.set_uniform("normal_matrix", n_matrix);
	phong_shader_.set_uniform("light_position", vec3(light));

	actor->draw();

	phong_shader_.disable();

	// check for OpenGL errors
	glCheckError("MeshViewer::draw_scene");
}
