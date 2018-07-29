//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#ifndef CUBEMAPVISUALIZATION_H
#define CUBEMAPVISUALIZATION_H

#include "Quad.h"
#include "gl.h"
#include "glmath.h"
#include "shader.h"
#include <lodepng.h>

// Visualization to help debug your shadow cube mapping code
struct CubeMapVisualization {
    void initialize() {
        m_quad.initialize();
        m_shader.load(SHADER_PATH "/cube_map_visualization.vert", SHADER_PATH "/cube_map_visualization.frag");

        glGenTextures(1, &m_demoTex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_demoTex);

        // Load the 6 cube face demo textures
        for (size_t i = 0; i < 6; ++i) {
            std::vector<unsigned char> img;
            unsigned width, height;
            unsigned error = lodepng::decode(img, width, height, TEXTURE_PATH "/f" + std::to_string(i) + ".png");

            // Note: we don't need to vertically flip cubemap textures; OpenGL uses a flipped sampling convention for these.
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);
        }

        // Use the same texture sampling settings as for the shadow maps, except for smoothing
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    // scene_view_matrix: matrix for the view for which the lighting map was computed.
    void draw(const mat4 &scene_view_matrix,   const mat4 &view_matrix, const mat4 &projection_matrix,
              const vec3 &light_pos_scene_eye, const PointLightShadowMap &sm) {
        m_shader.use();
        glActiveTexture(GL_TEXTURE0);
        if (demoTextures) glBindTexture(GL_TEXTURE_CUBE_MAP, m_demoTex);
        else              glBindTexture(GL_TEXTURE_CUBE_MAP, sm.tex);

        // Note: the cube is positioned in the scene's eye coordinates
        // Draw cube faces [-1, 1]^3 using quad [-1, 1]^2
        std::array<mat4, 6> model_matrices = {{
            mat4::rotate_y( 90) * mat4::translate(vec3(0, 0,  1)), // +x
            mat4::rotate_y(270) * mat4::translate(vec3(0, 0,  1)), // -x
            mat4::rotate_x(-90) * mat4::translate(vec3(0, 0,  1)), // +y
            mat4::rotate_x( 90) * mat4::translate(vec3(0, 0,  1)), // -y
            mat4::rotate_y(  0) * mat4::translate(vec3(0, 0,  1)), // +z
            mat4::rotate_y(180) * mat4::translate(vec3(0, 0,  1))  // -z
        }};

        m_shader.set_uniform("light_position_scene_eye", light_pos_scene_eye);

        mat4 scene_eye_to_custom_eye = view_matrix * affineInverse(scene_view_matrix);

        m_shader.set_uniform("cube_map", 0);
        for (const mat4 &m : model_matrices) {
            m_shader.set_uniform("scene_modelview_matrix",                                                    mat4::translate(light_pos_scene_eye) * mat4::scale(0.33) * m); // scene eye
            m_shader.set_uniform("modelview_projection_matrix", projection_matrix * scene_eye_to_custom_eye * mat4::translate(light_pos_scene_eye) * mat4::scale(0.33) * m);
            m_shader.set_uniform("demo_texture_mode", demoTextures);
            m_quad.draw();
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    };

    ~CubeMapVisualization() {
        glDeleteTextures(1, &m_demoTex);
    }

    bool demoTextures = true; // Whether to visualize the shadow map or the demo textures that indicate the coordinate system

private:
    Quad m_quad;
    GLuint m_demoTex;
    Shader m_shader;
};

#endif /* end of include guard: CUBEMAPVISUALIZATION_H */
