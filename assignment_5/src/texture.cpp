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

    for (int col = 0; col < width; ++col) {
        for (int row = 0; row < height; ++row) {
            img[(row * width + col) * 4 + 0] = 255; // R
            img[(row * width + col) * 4 + 1] = 255; // G
            img[(row * width + col) * 4 + 2] = 255; // B
            img[(row * width + col) * 4 + 3] = 255; // A
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
