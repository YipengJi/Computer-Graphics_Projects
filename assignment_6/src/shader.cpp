//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>


//=============================================================================


Shader::Shader() :
    pid_(0), vid_(0), fid_(0), gid_(0)
{
}


//-----------------------------------------------------------------------------


Shader::~Shader()
{
    cleanup();
}


//-----------------------------------------------------------------------------


void Shader::cleanup()
{
    if (pid_) glDeleteProgram(pid_);
    if (vid_) glDeleteShader(vid_);
    if (fid_) glDeleteShader(fid_);
    if (fid_) glDeleteShader(fid_);

    pid_ = vid_ = fid_ = gid_ = 0;
}


//-----------------------------------------------------------------------------


bool Shader::load(const char* vfile, const char* ffile, const char* gfile)
{
    // cleanup existing shaders first
    cleanup();
    glCheckError();


    // create program
    pid_ = glCreateProgram();
    glCheckError();

    // vertex shader
    vid_ = load_and_compile(vfile, GL_VERTEX_SHADER);
    glCheckError();
    if (vid_)  glAttachShader(pid_, vid_);
    glCheckError();

    // fragment shader
    fid_ = load_and_compile(ffile, GL_FRAGMENT_SHADER);
    if (fid_)  glAttachShader(pid_, fid_);

    glCheckError();
    // geometry shader
    if (gfile)
    {
        gid_ = load_and_compile(gfile, GL_GEOMETRY_SHADER);
        if (gid_)  glAttachShader(pid_, gid_);
    }


    // link program
    glLinkProgram(pid_);
    GLint status;
    glGetProgramiv(pid_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(pid_, GL_INFO_LOG_LENGTH, &length);

        std::string info(length + 1, ' ');
        glGetProgramInfoLog(pid_, length, NULL, &info[0]);
        std::cerr << "Shader: Cannot link program:\n" << info << std::endl;

        cleanup();

        return false;
    }
    glCheckError();

    return true;
}


//-----------------------------------------------------------------------------


GLint Shader::load_and_compile(const char* filename, GLenum type)
{
    // read file to string
    std::ifstream  ifs(filename);
    if (!ifs)
    {
        std::cerr << "Shader: Cannot open file \""  << filename << "\"\n";
        return 0;
    }
    std::stringstream  ss;
    ss << ifs.rdbuf();
    std::string str = ss.str();
    const char* source = str.c_str();
    ifs.close();


    // create shader
    GLint id = glCreateShader(type);
    if (!id)
    {
        std::cerr << "Shader: Cannot create shader object for \""  << filename << "\"\n";
        return 0;
    }


    // compile vertex shader
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);


    // check compile status
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string info(length + 1, ' ');
        glGetShaderInfoLog(id, length, NULL, &info[0]);

        std::cerr << "Shader: Cannot compile shader \""  << filename << "\"\n" << info << std::endl;

        glDeleteShader(id);

        return 0;
    }


    return id;
}


//-----------------------------------------------------------------------------


void Shader::use()
{
    if (pid_) glUseProgram(pid_);
}


//-----------------------------------------------------------------------------


void Shader::disable()
{
    glUseProgram(0);
}

//-----------------------------------------------------------------------------
