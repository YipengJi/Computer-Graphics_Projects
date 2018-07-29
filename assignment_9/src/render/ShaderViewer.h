
#pragma once

#include "gl.h"
#include "glfw_window.h"

#include "shader.h"
#include "Quad.h"
#include "../utils/vec.h"
#include "../utils/array2d.h"

//=============================================================================

/// OpenGL viewer that handles all the rendering for us
class ShaderViewer : public GLFW_window
{
public:

	/// default constructor
	/// \_title the window's title
	/// \_width the window's width
	/// \_height the window's height
	ShaderViewer(std::string const& _title = "Shader Viewer", int _width=640, int _height=640);

	void setShaderFiles(std::string const& vertex_shader_path, std::string const& fragment_shader_path);
	void setShaderFiles(std::vector<std::string> const& vertex_shader_paths, std::vector<std::string> const& fragment_shader_paths);

	virtual void saveScreenshot(std::string const& path) override;

	virtual void setViewerPosition(vec2 const& position);

	Array2D<vec4> extractFrame();

	/// resize function - called when the window size is changed
	virtual void resize(int width, int height) override;
	
protected:
	/// function that is called on the creation of the widget for the initialisation of OpenGL
	virtual void initialize() override;

	/// paint function - called when the window should be refreshed
	virtual void paint() override;

	virtual void paint_shader_to_texture();

	/// keyboard interaction
	virtual void keyboard(int key, int scancode, int action, int mods) override;

	virtual void scroll_wheel(double xoffset, double yoffset) override;

	virtual void timer(double dt) override;

	void save(std::string const& path);

private:
	Shader texture_shader;
	Shader shader_to_display;

	Quad fullscreen_quad;

	GLuint fbo;
	GLuint m_render_tex;
    uint32_t m_render_tex_width = 0, m_render_tex_height = 0;

	/// current viewport dimension
	uint32_t  width_, height_;
	
	// movement
	bool should_redraw;

	vec2 viewer_position;
	vec2 viewer_velocity;
	float viewer_scale;
};


//=============================================================================

