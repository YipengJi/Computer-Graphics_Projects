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
    int run();



private: //----------------------------- static wrapper functions for callbacks

    static void error__(int error, const char *description);
    static void keyboard__(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void resize__(GLFWwindow* window, int width, int height);

    static GLFW_window *instance__;



protected: //----------------------------------- callbacks as member functions

    /// must overload: initialize all OpenGL states
    virtual void initialize() = 0;

    /// must overload: handle window resizing
    virtual void resize(int width, int height) = 0;

    /// must overload: render the scene
    virtual void paint() = 0;

    /// may overload: handle keyboard events
    virtual void keyboard(int key, int scancode, int action, int mods) {}

    /// may overload: handle idle timer
    virtual void timer() {}



protected: //----------------------------------------------------- protected data

    /// GLFW window pointer
    GLFWwindow *window_;
};


/// @}


//=============================================================================
#endif
//=============================================================================

