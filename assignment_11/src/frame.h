#ifndef FRAME_H
#define FRAME_H

#include "shader.h"

// Represents and visualizes a 3D reference frame adapted to a curve:
//      t:      unit vector tangent to the curve
//      up:     some unit vector perpendicular to t
//      left:   up.cross(t)
struct Frame {
    Frame() : t(0.0, 0.0, 1.0), up(0.0, 1.0, 0.), left(1.0, 0.0, 0.0) { }

    vec3 t, up, left;
    // Align the frame with the tangent vector "tnew" either using parallel transport
    // from the current tangent vector or setting the vector as close to +y as possible.
    void alignTo(vec3 tnew) {
        tnew = normalize(tnew);
        if (m_useParallelTransport) {
            vec3 sinThetaAxis = cross(t, tnew);
            float cosTheta = dot(t, tnew);
            t    = tnew;
            up   = (dot(sinThetaAxis, up  ) / (1 + cosTheta)) * sinThetaAxis + cross(sinThetaAxis, up  ) + cosTheta * up;
            left = (dot(sinThetaAxis, left) / (1 + cosTheta)) * sinThetaAxis + cross(sinThetaAxis, left) + cosTheta * left;
        }
        else {
            t    = tnew;
            left = normalize(cross(vec3(0.0, 1.0, 0.0), t));
            up   = normalize(cross(t, left));
        }
    }

    // Initialize the OpenGL arrays/buffers with the arrow geometry for frame
    // visualization
    void initialize() {
        // generate vertex array object
        glGenVertexArrays(1, &m_arrowVAO);
        glBindVertexArray(m_arrowVAO);

        std::vector<GLfloat> coords;
        std::vector<GLint> idx;

        // Arrow geometry along the x axis.
        // Shaft cylinder
        const size_t nsubdiv = 20;
        const float radius = 0.08;
        for (size_t i = 0; i < nsubdiv; ++i) {
            float theta = (2 * M_PI * i) / nsubdiv;
            coords.push_back(0);
            coords.push_back( radius * cos(theta));
            coords.push_back(-radius * sin(theta));
            coords.push_back(0.75);
            coords.push_back( radius * cos(theta));
            coords.push_back(-radius * sin(theta));
            idx.push_back(2 * i                   + 0);
            idx.push_back(2 * i                   + 1);
            idx.push_back(2 * ((i + 1) % nsubdiv) + 1);
            idx.push_back(2 * i                   + 0);
            idx.push_back(2 * ((i + 1) % nsubdiv) + 1);
            idx.push_back(2 * ((i + 1) % nsubdiv) + 0);
        }
        // Arrow head cone
        size_t tipIdx = coords.size()  / 3;
        coords.push_back(1.0); coords.push_back(0); coords.push_back(0);

        size_t offset = tipIdx + 1;
        for (size_t i = 0; i < nsubdiv; ++i) {
            float theta = (2 * M_PI * i) / nsubdiv;
            coords.push_back(0.75);
            coords.push_back( radius * 1.5 * cos(theta));
            coords.push_back(-radius * 1.5 * sin(theta));

            idx.push_back(offset + i);
            idx.push_back(tipIdx);
            idx.push_back(offset + (i + 1) % nsubdiv);
        }

        // vertex positions -> attribute 0
        glGenBuffers(1, &m_arrowVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_arrowVBO);
        glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(GLfloat), coords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // triangle indices
        glGenBuffers(1, &m_arrowIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_arrowIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

        m_nidx = idx.size();

        glBindVertexArray(0);
    }

    void draw(Shader &s, const mat4 &vp, const vec3 &pos) {
        glBindVertexArray(m_arrowVAO);

        auto model = mat4::translate(pos) * mat4(xyzToFrame());

        s.use();
        s.set_uniform("modelview_projection_matrix", vp * model * mat4::scale(0.25));
        s.set_uniform("color", vec4(1.0, 0.0, 0.0, 1.0));
        glDrawElements(GL_TRIANGLES, m_nidx, GL_UNSIGNED_INT, NULL);

        s.set_uniform("color", vec4(0.0, 1.0, 0.0, 1.0));
        s.set_uniform("modelview_projection_matrix", vp * model * mat4::rotate_z(90) * mat4::scale(0.25));
        glDrawElements(GL_TRIANGLES, m_nidx, GL_UNSIGNED_INT, NULL);

        s.set_uniform("color", vec4(0.0, 0.0, 1.0, 1.0));
        s.set_uniform("modelview_projection_matrix", vp * model * mat4::rotate_y(-90) * mat4::scale(0.25));
        glDrawElements(GL_TRIANGLES, m_nidx, GL_UNSIGNED_INT, NULL);

        glBindVertexArray(0);
    }

    void toggleParallelTransport() { m_useParallelTransport = !m_useParallelTransport; }
    bool usingParallelTransport() { return m_useParallelTransport; }

    // Note: in its object coordinates, the spaceship points in the +z direction, with +y up, +x left.
    // We display the proper (x, y, z) axes for orienting the ship,
    // which should point in the +t direction and have the proper up vector.
    // Construct the rotation matrix mapping the (x, y, z) axis vectors to (left, up, t).
    mat3 xyzToFrame() const {
        mat3 m;
        m(0, 0) = left[0]; m(0, 1) = up[0]; m(0, 2) = t[0];
        m(1, 0) = left[1]; m(1, 1) = up[1]; m(1, 2) = t[1];
        m(2, 0) = left[2]; m(2, 1) = up[2]; m(2, 2) = t[2];
        return m;
    }

    ~Frame() {
        if (m_arrowVAO != 0) glDeleteVertexArrays(1, &m_arrowVAO);
        if (m_arrowVBO != 0) glDeleteBuffers(1, &m_arrowVBO);
        if (m_arrowIBO != 0) glDeleteBuffers(1, &m_arrowIBO);
    }
private:
    GLuint m_arrowVAO = 0,
           m_arrowVBO = 0,
           m_arrowIBO = 0;
    size_t m_nidx = 0;
    bool m_useParallelTransport = false;
};

#endif /* end of include guard: FRAME_H */
