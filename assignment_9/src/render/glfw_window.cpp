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

GLFW_window::GLFW_window(const char* _title, int _width, int _height)
{
	// initialize glfw window
	if (!glfwInit())
	{
		std::cerr << "Cannot initialize GLFW!\n";
		//exit(EXIT_FAILURE);
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

	// Pointer to <this> will be used in callbacks
	glfwSetWindowUserPointer(window_, this);

	// make this window the current one
	glfwMakeContextCurrent(window_);

	// enable vsync
	glfwSwapInterval(1);

	// register glfw callbacks
	glfwSetErrorCallback([](int error_code, char const* error_msg) {
		std::cerr << "GLFW error(" << error_code << "): " << error_msg << std::endl;
	});

	// Input
	glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		getHandle(window)->keyboard(key, scancode, action, mods);
	});
	glfwSetScrollCallback(window_, [](GLFWwindow* window, double xoffset, double yoffset) {
		// unfortunately when i capture [this], the type of the lambda is no longer matching the API
		getHandle(window)->scroll_wheel(xoffset, yoffset);
	});
	
	// Window Resize
	glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height)
	{
		auto win = getHandle(window);
		win->resize(width, height);
		win->paint();
		glfwSwapBuffers(window);
	});

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
	glGetError();
}


//-----------------------------------------------------------------------------


GLFW_window::~GLFW_window()
{
	glfwTerminate();
}


//-----------------------------------------------------------------------------

//int GLFW_window::run(std::string const& screenshot_path)
int GLFW_window::run(std::function<bool()> func_after_init)
{
	// initialize OpenGL
	initialize();

	// query framebuffer width and height
	// call resize to initialize viewport
	int width, height;
	glfwGetFramebufferSize(window_, &width, &height);
	resize(width, height);

	bool should_loop = func_after_init();

	if(should_loop) {
		glfwSetTime(0);

		// now run the event loop
		while (!glfwWindowShouldClose(window_))
		{
			// call timer function

			double dt = glfwGetTime();
			timer(dt);
			glfwSetTime(0);

			// draw scene
			paint();

			// swap buffers
			glfwSwapBuffers(window_);

			// handle events
			glfwPollEvents();
		}
	}

	glfwDestroyWindow(window_);

	return EXIT_SUCCESS;
}

void GLFW_window::close()
{
	glfwSetWindowShouldClose(window_, GL_TRUE);
}

GLFW_window* GLFW_window::getHandle(GLFWwindow* w)
{
	return static_cast<GLFW_window*>(glfwGetWindowUserPointer(w));
}


//-----------------------------------------------------------------------------
