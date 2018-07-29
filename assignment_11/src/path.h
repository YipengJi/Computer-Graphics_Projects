#ifndef PATH_H
#define PATH_H

#include "gl.h"
#include "glmath.h"
#include <vector>

class Path {
public:
    Path(unsigned int resolution = 1000) : m_resolution(resolution), m_num_pts(0) { }

    void initialize() {
        // generate vertex array object
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // vertex positions -> attribute 0
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // line strip indices
        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

        glBindVertexArray(0);
    }

    void setPoints(const std::vector<vec3> &pts) {
        m_num_pts = pts.size();
        std::vector<GLfloat> positions(3 * m_num_pts);
        for (size_t i = 0; i < m_num_pts; ++i) {
            positions[3 * i    ] = pts[i][0];
            positions[3 * i + 1] = pts[i][1];
            positions[3 * i + 2] = pts[i][2];
        }
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::vector<GLuint> indices(m_num_pts);
        for (size_t i = 0; i < m_num_pts; ++i) indices[i] = i;

        glBindVertexArray(m_vao);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);
    }

    // Uniformly parametrized curve 'f' on the interval [0, 1]
    template<typename F>
    void sample(const F &f) {
        std::vector<vec3> pts;
        for (size_t i = 0; i < m_resolution; ++i)
            pts.push_back(f(i / float(m_resolution - 1)));
        setPoints(pts);
    }

    /// render the path as line segments
    void draw() {
        glBindVertexArray(m_vao);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1.0f); // Unfortunately, setting line widths > 1 is unsupported in modern OpenGL;
                           // we'll need to render polygons if we want thicker lines...
        glDrawElements(GL_LINE_STRIP, m_num_pts, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
    }

    ~Path() {
        if (m_vbo)  glDeleteBuffers(1, &m_vbo);
        if (m_ibo)  glDeleteBuffers(1, &m_ibo);
        if (m_vao)  glDeleteVertexArrays(1, &m_vao);
    }

private:
    /// tessellation resolution
    unsigned int m_resolution, m_num_pts;

    // vertex array object
    GLuint m_vao = 0;

    /// vertex buffer object
    GLuint m_vbo = 0;

    /// index buffer object
    GLuint m_ibo = 0;
};

#endif /* end of include guard: PATH_H */
