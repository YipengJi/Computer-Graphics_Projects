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


Shader::Shader() : pid_(0), valid(false)
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
    for (GLint id : shaderIds_)
        glDeleteShader(id);
    shaderIds_.clear();

	valid = false;
}


//-----------------------------------------------------------------------------

static void print_file_list(std::vector<std::string> const& files) {
	if(files.size() > 1) {
		std::cerr << "[\n";
		for(auto const& fn : files) std::cerr << "	" << fn << "\n";
		std::cerr << "]\n" << std::endl;
	} else {
		std::cerr << '"' << files[0] <<  "\"\n";
	}
}

bool Shader::load(Shader::ListOfStringCRef vfiles, Shader::ListOfStringCRef ffiles, Shader::ListOfStringCRef gfiles) {
	// cleanup existing shaders first
    cleanup();
    glCheckError("Shader::load");

    // create program
    pid_ = glCreateProgram();
    glCheckError("Shader::load create program");

    try {
        // vertex shader
        for (const auto &vs : vfiles) {
            GLint id = load_and_compile(vs.c_str(), GL_VERTEX_SHADER);
            glAttachShader(pid_, id);
            shaderIds_.push_back(id);
            glCheckError();
        }

        // fragment shader
        for (const auto &fs : ffiles) {
            GLint id = load_and_compile(fs.c_str(), GL_FRAGMENT_SHADER);
            glAttachShader(pid_, id);
            shaderIds_.push_back(id);
            glCheckError();
        }

        // geometry shader
        for (const auto &gs : gfiles) {
            GLint id = load_and_compile(gs.c_str(), GL_GEOMETRY_SHADER);
            glAttachShader(pid_, id);
            shaderIds_.push_back(id);
            glCheckError();
        }
        // link program
        glLinkProgram(pid_);
        GLint status;
        glGetProgramiv(pid_, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length;
            glGetProgramiv(pid_, GL_INFO_LOG_LENGTH, &length);

            std::string info(length + 1, ' ');
            glGetProgramInfoLog(pid_, length, NULL, &info[0]);
            throw std::runtime_error("Shader: Cannot link program:\n" + info + "\n");
        }

        glCheckError("Shader::load link program");

        valid = true;
        return true;
    }
    catch (const std::runtime_error &e) {
        std::cerr << e.what();
        cleanup();
        return false;
    }
}


bool Shader::load(const char* vfile, const char* ffile, const char* gfile)
{
	std::vector<std::string> gfiles;
	if(gfile) gfiles.push_back(gfile);
	
	return load({vfile}, {ffile}, gfiles);
}

//-----------------------------------------------------------------------------



GLint Shader::load_and_compile(const std::string &filename, GLenum type)
{
    // read file to string
    std::ifstream  ifs(filename);
    if (!ifs) throw std::runtime_error("Shader: Cannot open file " + filename + "\n");

    std::stringstream  ss;
    ss << ifs.rdbuf();
    std::string const str = ss.str();
    const char* source = str.c_str();
    ifs.close();

    // create shader
    GLint id = glCreateShader(type);
    if (!id) throw std::runtime_error("Shader: Cannot create shader object for " + filename + "\n");

    // compile vertex shader
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);


    // check compile status
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string info(length + 1, ' ');
        glGetShaderInfoLog(id, length, NULL, &info[0]);

        glDeleteShader(id);

        throw std::runtime_error("Shader: Cannot compile shader " + filename + "\n" + info + "\n");
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
