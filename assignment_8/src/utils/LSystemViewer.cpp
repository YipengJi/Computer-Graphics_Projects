#include "LSystemViewer.h"
#include <lodepng.h>
#include <algorithm>

void LSystemViewer::initialize() {
    // Generate vertex array object and set up pointers
    // (But don't actually allocate the buffers... those are reallocated in each drawing loop)
    glGenVertexArrays(1, &segment_vao);
    glBindVertexArray(segment_vao); 
    glGenBuffers(bufferObjects.size(), bufferObjects.data());
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[MY_VTX_BUFFER]); // Work with vertex buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);  // 2 floats per vertex
    glEnableVertexAttribArray(0);

    // To support rendering without displaying the window, and to render the
    // segments only (without help text), we render the segments to a texture.
    // This requires setting up a framebuffer.
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &m_render_tex);

    m_fullscreenQuad.initialize();

    line_shader.load(SHADER_PATH "/line.vert", SHADER_PATH "/line.frag");
    texture_shader.load(SHADER_PATH "/textured.vert", SHADER_PATH "/textured.frag");
}

void LSystemViewer::renderSegmentsToFBO() {
    glDisable(GL_DEPTH_TEST);

    ////////////////////////////////////////////////////////////////////////////
    // Draw into our texture
    ////////////////////////////////////////////////////////////////////////////
    // First, we need to resize the texture to match the screen resolution
    if ((m_width != m_render_tex_width) || (m_height != m_render_tex_height)) {
        glBindTexture(GL_TEXTURE_2D, m_render_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);
        m_render_tex_width = m_width;
        m_render_tex_height = m_height;
    }

    glBindTexture(GL_TEXTURE_2D, m_render_tex);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_render_tex, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0); // draw to the texture
    // No depth buffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Incomplete framebuffer; status: " + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
    glViewport(0, 0, m_width, m_height);

    ////////////////////////////////////////////////////////////////////////////
    // Copy segment data to OpenGL
    ////////////////////////////////////////////////////////////////////////////
    auto segments = m_lsys.getSegments();
    auto bbox = segment_group_bounding_box(segments);
	vec2 const center = (bbox.first + bbox.second)*0.5;
    vec2 const dim = bbox.second - bbox.first;

	double const aspect_ratio = (double)m_width / (double)m_height;

	// We are rendering in NDC coordinates; need to map
    // bbox to [-1, 1]^2
	double const scale = 2*std::min(
		(aspect_ratio-MARGIN) / dim.x,
		(1-MARGIN) / dim.y
	);

    const size_t npts = segments.size() * 2;
    std::vector<float> coords;
    coords.reserve(npts * 2);

	for (const auto &s : segments) {
		vec2 first_rel = (s.first - center)*scale;
		vec2 second_rel = (s.second - center)*scale;
		coords.push_back(first_rel.x / aspect_ratio);
		coords.push_back(first_rel.y);
		coords.push_back(second_rel.x / aspect_ratio);
		coords.push_back(second_rel.y);
    }

    glBindVertexArray(segment_vao);
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[MY_VTX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_DYNAMIC_DRAW);

    std::vector<float> colors(npts * 4, 1.0);

    std::vector<GLuint> indices(npts);
    for (size_t i = 0; i < npts; ++i) indices[i] = i;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[MY_INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);
    
    ////////////////////////////////////////////////////////////////////////////
    // Render segments
    ////////////////////////////////////////////////////////////////////////////
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f); // Unfortunately, setting line widths > 1 is unsupported in modern OpenGL;
                       // we'll need to render polygons if we want thicker lines...

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    line_shader.use();
    glDrawElements(GL_LINES, npts, GL_UNSIGNED_INT, NULL);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LSystemViewer::save(const std::string &path) {
    glBindTexture(GL_TEXTURE_2D, m_render_tex);
    std::vector<unsigned char> img(m_width * m_height * 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());

    // OpenGL's pixel data convention is vertically flipped wrt. PNG's; flip the image
    for (unsigned int y = 0; y < m_height / 2; ++y) {
        for (unsigned int x = 0; x < m_width; ++x) {
            for (unsigned int c = 0; c < 4; ++c) {
                std::swap(img[(              y  * m_width + x) * 4 + c],
                          img[((m_height - y - 1) * m_width + x) * 4 + c]);
            }
        }
    }

    lodepng::encode(path, img, m_width, m_height);
    std::cout << "Wrote " << path << std::endl;
}

void LSystemViewer::paint() {
    renderSegmentsToFBO();

    glDisable(GL_DEPTH_TEST);

    // Draw fullscreen quad with the line drawing we just rendered.
    texture_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_render_tex);
    texture_shader.set_uniform("tex", 0);
    m_fullscreenQuad.draw();

    glCheckError();
}

void LSystemViewer::keyboard(int key, int scancode, int action, int mods) {
	setNeedsDisplay();
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
			case GLFW_KEY_Q:
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window_, GL_TRUE);
			case GLFW_KEY_LEFT:
				m_lsys.rotation_angle_deg -= ANGLE_STEP;
				std::cout << "angle: " << m_lsys.rotation_angle_deg << " deg" << std::endl;
				break;
			case GLFW_KEY_RIGHT:
				m_lsys.rotation_angle_deg += ANGLE_STEP;
				std::cout << "angle: " << m_lsys.rotation_angle_deg << " deg" << std::endl;
				break;
			case GLFW_KEY_UP:
				m_lsys.num_iters = std::min(m_lsys.num_iters + 1, int(MAX_ITERS));
				std::cout << "iters: " << m_lsys.num_iters << std::endl;
				break;
			case GLFW_KEY_DOWN:
				m_lsys.num_iters = std::max(m_lsys.num_iters - 1, 0);
				std::cout << "iters: " << m_lsys.num_iters << std::endl;
				break;
			case GLFW_KEY_P: {
				static size_t i = 0;
				save("screenshot_" + std::to_string(++i) + ".png");
				break;
			}
		}
	}
}

void LSystemViewer::scroll_wheel(double xoffset, double yoffset)
{
	int const scroll_amount = yoffset;
	int const new_num_iters = std::min(std::max(m_lsys.num_iters + scroll_amount, 0), int(MAX_ITERS));
	if(new_num_iters != m_lsys.num_iters) {
		m_lsys.num_iters = new_num_iters;
		std::cout << "iters: " << m_lsys.num_iters << std::endl;
		setNeedsDisplay();
	}

}
