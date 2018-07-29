//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#ifndef DUMPOPENGLBUFFER_H
#define DUMPOPENGLBUFFER_H

#include <lodepng.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "gl.h"

// Dump the currently active OpenGL frame buffer to a PNG file
inline void dumpOpenGLBuffer(const std::string &path, size_t width, size_t height) {
    std::vector<unsigned char> image(width * height * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    // OpenGL's pixel data convention is vertically flipped wrt. PNG's; flip the image
    for (unsigned int y = 0; y < height / 2; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int c = 0; c < 4; ++c) {
                std::swap(image[(              y  * width + x) * 4 + c],
                          image[((height - y - 1) * width + x) * 4 + c]);
            }
        }
    }

    lodepng::encode(path, image, width, height);
    std::cout << "Wrote " << path << std::endl;
}

// Dump the currently active OpenGL frame buffer to a PNG file
inline void dumpOpenGLDepthBuffer(const std::string &path, size_t width, size_t height) {
    std::vector<unsigned char> img_grayscale(width * height);
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, img_grayscale.data());

    // OpenGL's pixel data convention is vertically flipped wrt. PNG's; flip the image
    // also convert to RGBA (so we don't have to figure out how to get lodepng to
    // write grayscale...)
    std::vector<unsigned char> img_rgba(width * height * 4);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int c = 0; c < 4; ++c)
                img_rgba[(y  * width + x) * 4 + c] = img_grayscale[(height - y - 1) * width + x];
        }
    }

    lodepng::encode(path, img_rgba, width, height);
    std::cout << "Wrote " << path << std::endl;
}

#endif /* end of include guard: DUMPOPENGLBUFFER_H */
