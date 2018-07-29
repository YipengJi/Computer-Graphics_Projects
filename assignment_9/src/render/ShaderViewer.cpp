//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "ShaderViewer.h"
#include "../utils/vec.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <array>
#include <cmath>
#include <lodepng.h>
#include <sstream>
#include <fstream>
#include <iomanip>
//=============================================================================

ShaderViewer::ShaderViewer(std::string const& _title, int _width, int _height)
	: GLFW_window(_title.c_str(), _width, _height)
	, viewer_position(0, 0)
	, viewer_velocity(0)
	, viewer_scale(1.0)
	, should_redraw(true)
{
}

void ShaderViewer::setShaderFiles(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	shader_to_display.load({vertex_shader_path}, {fragment_shader_path});
}

void ShaderViewer::setShaderFiles(const std::vector<std::string>& vertex_shader_paths, const std::vector<std::string>& fragment_shader_paths)
{
	shader_to_display.load(vertex_shader_paths, fragment_shader_paths);
}

void ShaderViewer::setViewerPosition(const vec2& position)
{
	should_redraw = true;
	viewer_position = position;
}


//-----------------------------------------------------------------------------

static std::string find_free_screenshot_name() {
	for(uint32_t num = 0;;num++) {
		std::stringstream ss;
		ss << "screenshot_" << std::setfill('0') << std::setw(3) << num << ".png";
		std::string path = ss.str();

		std::ifstream f(path);
		if(!f.good()) { // check file existence
			return path;
		}
	}
}

void ShaderViewer::keyboard(int key, int scancode, int action, int mods)
{
	// arrow keys
	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		float change = (action == GLFW_PRESS)?1:-1;
		change *= 0.5;

		switch (key)
		{
			case GLFW_KEY_LEFT:
			case GLFW_KEY_A:
			{
				viewer_velocity.x -= change;
				break;
			}

			case GLFW_KEY_RIGHT:
			case GLFW_KEY_D:
			{
				viewer_velocity.x += change;
				break;
			}

			case GLFW_KEY_DOWN:
			case GLFW_KEY_S:
			{
				viewer_velocity.y -= change;
				break;
			}

			case GLFW_KEY_UP:
			case GLFW_KEY_W:
			{
				viewer_velocity.y += change;
				break;
			}
		}
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		// Change view between the various bodies with keys 1..6
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
			{
				close();
				break;
			}

			case GLFW_KEY_PRINT_SCREEN:
			case GLFW_KEY_F12:
			case GLFW_KEY_P:
			{
				saveScreenshot(find_free_screenshot_name());
				break;
			}
		}
	}
}

void ShaderViewer::scroll_wheel(double xoffset, double yoffset)
{
	float const scroll_amount = -yoffset * 0.1;
	viewer_scale = std::min(std::max(viewer_scale + scroll_amount, 0.1f), 8.0f);
	should_redraw = true;
}

void ShaderViewer::timer(double dt)
{
	if(dot(viewer_velocity, viewer_velocity) > 0.01) {
		setViewerPosition(viewer_position + viewer_velocity*(viewer_scale*dt));
	}
}

//-----------------------------------------------------------------------------

void ShaderViewer::resize(int _width, int _height)
{
	width_  = _width;
	height_ = _height;
	should_redraw = true;
	glViewport(0, 0, _width, _height);
}

//-----------------------------------------------------------------------------


void ShaderViewer::initialize()
{
    // We'll render our procedural textures to an offscreen texture buffer
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &m_render_tex);

	fullscreen_quad.initialize();

	// set initial state
	glClearColor(1,1,1,0);
	glDisable(GL_DEPTH_TEST);

	texture_shader.load(SHADER_PATH "/textured.vert", SHADER_PATH "/textured.frag");
}
//-----------------------------------------------------------------------------

void ShaderViewer::paint_shader_to_texture()
{
	// If the shader failed to compile and we try to draw, we will get spam in the console output
	if(!shader_to_display.is_valid()) {
		return;
	}

	// No need to update since the parameters have not changed
	if(!should_redraw) {
		return;
	}

	glDisable(GL_DEPTH_TEST);

	////////////////////////////////////////////////////////////////////////////
	// Draw into our texture
	////////////////////////////////////////////////////////////////////////////
	// First, we need to resize the texture to match the screen resolution
	if ((width_ != m_render_tex_width) || (height_ != m_render_tex_height)) {
		glBindTexture(GL_TEXTURE_2D, m_render_tex);
		// using RGBD32F prevents clipping into [0, 1]
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width_, height_, 0, GL_RGBA, GL_FLOAT, NULL); //<- framebuffer type!

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);
		m_render_tex_width = width_;
		m_render_tex_height = height_;
	}

	glBindTexture(GL_TEXTURE_2D, m_render_tex);

	//fullscreen_quad.draw();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_render_tex, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0); // draw to the texture

	// No depth buffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Incomplete framebuffer; status: " + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
	glViewport(0, 0, width_, height_);

	// Use
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	vec2 aspect_ratio(width_, height_);
	aspect_ratio *= std::sqrt(2.0f) / norm(aspect_ratio);

	shader_to_display.use();
	shader_to_display.set_uniform("viewer_position", viewer_position);
	shader_to_display.set_uniform("viewer_scale", viewer_scale * aspect_ratio);

	fullscreen_quad.draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError("ShaderViewer::paint_shader_to_texture");

	should_redraw = false; // we have just redrawn, waiting for new changes
}

void ShaderViewer::paint()
{
	paint_shader_to_texture();

	glDisable(GL_DEPTH_TEST);

	// Draw fullscreen quad with the line drawing we just rendered.
	texture_shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_render_tex);
	texture_shader.set_uniform("tex", 0);
	fullscreen_quad.draw();

	glCheckError("ShaderViewer::paint");
}


Array2D<vec4> ShaderViewer::extractFrame()
{
	paint_shader_to_texture();

	Array2D<vec4> out_array({m_render_tex_width, m_render_tex_height});

	glBindTexture(GL_TEXTURE_2D, m_render_tex);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, reinterpret_cast<float*>(out_array.data_ptr())); // <- read type

	// the copy is maybe automatically optimized out
	// https://stackoverflow.com/questions/17473753/c11-return-value-optimization-or-move/17473874
	return out_array;
}

void ShaderViewer::saveScreenshot(const std::string& path)
{
	paint_shader_to_texture();
	save(path);
}

void ShaderViewer::save(const std::string& path)
{
	glBindTexture(GL_TEXTURE_2D, m_render_tex);
    std::vector<unsigned char> img(m_render_tex_width * m_render_tex_height * 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data()); // <- read type

	// OpenGL's pixel data convention is vertically flipped wrt. PNG's; flip the image
	for (unsigned int y = 0; y < m_render_tex_height / 2; ++y) {
		for (unsigned int x = 0; x < m_render_tex_width; ++x) {
			for (unsigned int c = 0; c < 4; ++c) {
				std::swap(
					img[(              y  * m_render_tex_width + x) * 4 + c],
					img[((m_render_tex_height - y - 1) * m_render_tex_width + x) * 4 + c]);
			}
		}
	}

	lodepng::encode(path, img, m_render_tex_width, m_render_tex_height);
	std::cout << "Wrote " << path << std::endl;
}
