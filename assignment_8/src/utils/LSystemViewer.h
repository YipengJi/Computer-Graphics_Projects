#ifndef LSYSTEM_VIEWER_H
#define LSYSTEM_VIEWER_H

#include "gl.h"
#include "glfw_window.h"
#include <array>

#include "shader.h"
#include "../lsystem.h"
#include "Quad.h"

class LSystemViewer : public GLFW_window
{
public:
    LSystemViewer(LindenmayerSystem &lsys, const char *title, size_t width, size_t height, bool interactive=true)
        : GLFW_window(title, width, height, interactive)
		, m_lsys(lsys)
		, m_width(width)
		, m_height(height)
	{}

    virtual ~LSystemViewer() {
        glDeleteBuffers(bufferObjects.size(), bufferObjects.data());
        glDeleteVertexArrays(1, &segment_vao);
    }

    static constexpr double ANGLE_STEP = 5.0;
    static constexpr int MAX_ITERS = 16;
	static constexpr double MARGIN = 0.05;

protected:
    virtual void initialize() override;
    void renderSegmentsToFBO();
    virtual void paint() override;

    virtual void writeFramebufferImage(const std::string &outPath) {
        renderSegmentsToFBO();
        save(outPath);
    }

    void save(const std::string &path);

    virtual void keyboard(int key, int scancode, int action, int mods) override;
    virtual void scroll_wheel(double xoffset, double yoffset) override;


    void resize(int width, int height) {
        glViewport(0, 0, width, height);
        m_width = width;
        m_height = height;
    }

private:
    Shader line_shader, texture_shader;
    GLuint segment_vao, fbo;
    enum { MY_VTX_BUFFER = 0, MY_INDEX_BUFFER = 1 };
    std::array<GLuint, 2> bufferObjects;

    GLuint m_render_tex;
    int m_render_tex_width = 0, m_render_tex_height = 0;
    Quad m_fullscreenQuad;

	int m_width, m_height;

	LindenmayerSystem &m_lsys;

};

#endif /* end of include guard: DEMO_VIEWER_H */
