//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#include "glfw_window.h"
#include <iostream>
//=============================================================================


GLFW_window *GLFW_window::instance__ = NULL;


//-----------------------------------------------------------------------------


GLFW_window::GLFW_window(const char* _title, int _width, int _height)
{
    // initialize glfw window
    if (!glfwInit())
    {
        std::cerr << "Cannot initialize GLFW!\n";
        exit(EXIT_FAILURE);
    }


    // request core profile and OpenGL version 3.2
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // try to create window
    window_ = glfwCreateWindow(_width, _height, _title, NULL, NULL);
    if (!window_) {
        std::cerr << "Window creation failed!\n";
        std::cerr << "Attempting fall-back (for INF03 machines)" << std::endl;

        // Request OpenGL version 3.1
        // Note: below version 3.2, we must request GLFW_OPENGL_ANY_PROFILE
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        window_ = glfwCreateWindow(_width, _height, _title, NULL, NULL);

	if (!window_) {
            glfwTerminate();
            std::cerr << "Window creation failed!\n";
            exit(EXIT_FAILURE);
	}
    }


    // make this window the current one
    glfwMakeContextCurrent(window_);


    // enable vsync
    glfwSwapInterval(1);


    // register glfw callbacks
    glfwSetKeyCallback(window_, keyboard__);
    glfwSetFramebufferSizeCallback(window_, resize__);


    // now that we have a GL context, initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }


    // debug: print GL and GLSL version
    std::cout << "GLEW   " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "GL     " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


    // call glGetError once to clear error queue
    GLenum error = glGetError();


    instance__ = this;
}


//-----------------------------------------------------------------------------


GLFW_window::~GLFW_window()
{
    glfwTerminate();
}


//-----------------------------------------------------------------------------


int GLFW_window::run()
{
    // initialize OpenGL
    initialize();

    // query framebuffer width and height
    // call resize to initialize viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    resize(width, height);

    // now run the event loop
    while (!glfwWindowShouldClose(window_))
    {
        // call timer function
        timer();

        // draw scene
        paint();

        // swap buffers
        glfwSwapBuffers(window_);

        // handle events
        glfwPollEvents();
    }

    glfwDestroyWindow(window_);

    return EXIT_SUCCESS;
}


//-----------------------------------------------------------------------------


void GLFW_window::error__(int error, const char *description)
{
    fputs(description, stderr);
}


void GLFW_window::keyboard__(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    instance__->keyboard(key, scancode, action, mods);
}


void GLFW_window::resize__(GLFWwindow* window, int width, int height)
{
    instance__->resize(width, height);
    instance__->paint();
    glfwSwapBuffers(instance__->window_);
}


//=============================================================================
