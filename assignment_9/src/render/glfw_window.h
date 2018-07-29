//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H
//=============================================================================


#include "gl.h"

#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <string>
#include <functional>

//== CLASS DEFINITION =========================================================


/// \addtogroup glfw_window glfw_window
/// @{


/// Simple GLFW window
class GLFW_window
{

public: //------------------------------------------------------ public methods

	/// constructor
	GLFW_window(const char* _title="", int _width=0, int _height=0);

	/// destructor
	virtual ~GLFW_window();

	/// main window loop
	//int run(std::string const& screenshot_path="");

	/// main window loop
	// The function is called after initialization, and it can save screenshots or extract data
	// The value returned by the function determines whether the viewer should loop or terminate now.
	int run(std::function<bool()> func_after_init = [](){return true;});

	void close();

    virtual void saveScreenshot(const std::string &path) {};

	/// must overload: handle window resizing
	virtual void resize(int width, int height) = 0;

protected: //----------------------------------- callbacks as member functions

	/// must overload: initialize all OpenGL states
	virtual void initialize() = 0;

	/// must overload: render the scene
	virtual void paint() = 0;

	/// may overload: handle keyboard events
	virtual void keyboard(int key, int scancode, int action, int mods) {}

	// may overload: scroll wheel input
    virtual void scroll_wheel(double xoffset, double yoffset) {}
	
	/// may overload: handle idle timer
	virtual void timer(double dt) {}



protected: //----------------------------------------------------- protected data

	static GLFW_window* getHandle(GLFWwindow* w);
	
	/// GLFW window pointer
	GLFWwindow *window_;
};


/// @}


//=============================================================================
#endif
//=============================================================================

