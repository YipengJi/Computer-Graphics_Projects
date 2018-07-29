//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#ifndef POINTLIGHTSHADOWMAP_HH
#define POINTLIGHTSHADOWMAP_HH

#include "gl.h"

// We need to use a cube map as the shadow map for omni-directional lights.
struct PointLightShadowMap {
    PointLightShadowMap(int size = 1024) : size(size) {
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

        // Allocate the 6 cube face textures
        for (size_t i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F,
                         size, size, 0, GL_RED, GL_FLOAT, NULL);
        }

        // Don't antialias the depth sampling
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // Generate and attach our depth buffer
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Done modifying our cube map texture.
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void selectCubeFaceTextureAsRenderTarget(size_t cube_face) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + cube_face, tex, 0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0); // draw to the texture
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Incomplete framebuffer; status: " + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));

        glViewport(0, 0, size, size);
    }

    void bind() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
    }

    void unbind() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    ~PointLightShadowMap() {
        glDeleteTextures(1, &tex);
        glDeleteRenderbuffers(1, &depthBuffer);
        glDeleteFramebuffers(1, &fbo);
    }

    GLuint fbo, tex, depthBuffer;
    int size;
};

#endif /* end of include guard: POINTLIGHTSHADOWMAP_HH */
