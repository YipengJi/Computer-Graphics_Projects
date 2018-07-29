//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "texture.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include "lodepng.h"

//=============================================================================


Texture::Texture() :
    id_(0)
{
}


//-----------------------------------------------------------------------------


Texture::~Texture()
{
    if (id_) glDeleteTextures(1, &id_);
}


//-----------------------------------------------------------------------------

bool Texture::loadPNG(const char* filename)
{
    std::cout << "Load texture " << filename << "\n" << std::flush;

    std::vector<unsigned char> img;
    unsigned width, height;

    unsigned error = lodepng::decode(img, width, height, filename);
    if (error) {
        std::cout << "read error: " << lodepng_error_text(error) << std::endl;
        return false;
    }

    return uploadImage(img, width, height);
}

//-----------------------------------------------------------------------------


bool Texture::uploadImage(std::vector<unsigned char> &img, unsigned width, unsigned height)
{
    if (!id_) {
        std::cerr << "Texture: initialize before loading!\n";
        return false;
    }

    // flip vertically in order to adhere to how OpenGL interpretes image data
    if (height % 2) throw std::runtime_error("Image must be of even height");
    assert(height % 2 == 0);
    for (unsigned int y = 0; y < height/2; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int c = 0; c < 4; ++c) {
                std::swap(img[(              y  * width + x) * 4 + c],
                          img[((height - y - 1) * width + x) * 4 + c]);
            }
        }
    }

    // upload texture data
    glActiveTexture(unit_);
    glBindTexture(type_, id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage2D(type_, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);


    if(minfilter_==GL_LINEAR_MIPMAP_LINEAR)
    {
        // comment out to disable mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return true;
}


//-----------------------------------------------------------------------------

bool Texture::createSunBillboardTexture()
{
    std::cout << "creating sun billboard " << "\n" << std::flush;

    std::vector<unsigned char> img;
    int width = 900;
    int height = 900;
    img.resize(width*height * 4);
    double opacity_speed = 4.0;
    double opacity_ratio = 0.6;

    /** \todo Set up the texture for the sun billboard.
    *   - Draw an opaque circle with a 150 pixel radius in its middle
    *   - Outside that circle the texture should become more and more transparent to mimic a nice glow effect
    *   - Make sure that your texture is fully transparent at its borders to avoid seeing visible edges
    *   - Experiment with the color and with how fast you change the transparency until the effect satisfies you
    **/

    for (int col = 0; col < width; ++col) {
        for (int row = 0; row < height; ++row) {
            img[(row * width + col) * 4 + 0] = 224; // R
            img[(row * width + col) * 4 + 1] = 123; // G
            img[(row * width + col) * 4 + 2] = 15; // B
            double dist = sqrt(pow(col - 449.5, 2) + pow(row - 449.5, 2));
            img[(row * width + col) * 4 + 3] = (dist <= 150) ? 255 : std::max(255*(1 - opacity_speed * pow((dist-150)/299.5, opacity_ratio)), 0.0); // A
        }
    }

    return uploadImage(img, width, height);
}


//-----------------------------------------------------------------------------


void Texture::init(GLenum unit, GLenum type, GLint minfilter, GLint magfilter, GLint wrap)
{
    // remember this
    unit_ = unit;
    type_ = type;
    minfilter_ = minfilter;

    // activate texture unit
    glActiveTexture(unit_);

    // create texture object
    glGenTextures(1, &id_);
    glBindTexture(type_, id_);

    // set texture parameters
    glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, magfilter);
    glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, minfilter);
    glTexParameteri(type_, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(type_, GL_TEXTURE_WRAP_T, wrap);
}


//-----------------------------------------------------------------------------


void Texture::bind()
{
    assert(id_);
    glActiveTexture(unit_);
    glBindTexture(type_, id_);
}



//=============================================================================
