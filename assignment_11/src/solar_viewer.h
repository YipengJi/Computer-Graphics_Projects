//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef SOLAR_VIEWER_H
#define SOLAR_VIEWER_H
//=============================================================================


#include "gl.h"
#include "glfw_window.h"

#include "sphere.h"
#include "shader.h"
#include "texture.h"
#include "planet.h"
#include "ship.h"
#include "path.h"
#include "frame.h"
#include "billboard.h"
#include "bezier.h"


//=============================================================================


enum PLANET {SUN=0, MERCURY=1,VENUS=2, EARTH=3, MOON=4, MARS=5};


//=============================================================================


/// OpenGL viewer that handles all the rendering for us
class Solar_viewer : public GLFW_window
{
public:

    /// default constructor
    /// \_title the window's title
    /// \_width the window's width
    /// \_height the window's height
    Solar_viewer(const char* _title, int _width, int _height);


protected:

    /// function that is called on the creation of the widget for the initialisation of OpenGL
    virtual void initialize();

    /// resize function - called when the window size is changed
    virtual void resize(int width, int height);

    /// paint function - called when the window should be refreshed
    virtual void paint();

    /// keyboard interaction
    virtual void keyboard(int key, int scancode, int action, int mods);


    /// function that draws the planet system
    /// \param _projection the projection matrix for the scene
    /// \param _view the view matrix for the scene
    void draw_scene(mat4& _projection, mat4& _view);

    /// update function on every timer event (controls the animation)
    virtual void timer();

    /// update the body positions (called by the timer).
    void update_body_positions();

    void randomize_planets();

private:

    /// sphere object
    Sphere unit_sphere_;

    /// the sun object
    Planet sun_;
    /// the mercury object
    Planet mercury_;
    /// the venus object
    Planet venus_;
    /// the earth object
    Earth  earth_;
    /// the moon object
    Planet moon_;
    /// the mars object
    Planet mars_;
    /// the stars object
    Planet stars_;
    /// spaceship object
    Ship ship_;
    /// sunglow billboard
    Billboard sunglow_;

    /// default color shader (renders only texture)
    Shader   color_shader_;
    // sun shader (renders the sun: texture plus an optional shimmer effect)
    Shader   sun_shader_;
    /// phong shader (renders texture and basic illumination)
    Shader   phong_shader_;
    /// earth shader (renders the earth using multi texturing)
    Shader   earth_shader_;

    /// simple shader for visualizing curves (just using solid color).
    Shader   solid_color_shader_;

    /// interval for the animation timer
    bool  timer_active_;
    /// update factor for the animation
    float time_step_;

    /// state whether the rendering should be in color or not
    bool greyscale_;

    /// Whether/how to display the ship path curve.
    enum CurveDisplayMode { CURVE_SHOW_NONE = 0, CURVE_SHOW_PATH = 1, CURVE_SHOW_PATH_CP = 2, CURVE_SHOW_PATH_FRAME = 3, CURVE_SHOW_NUM_MODES = 4 } curve_display_mode_;
    Path ship_path_renderer_, ship_path_cp_renderer_;
    Frame ship_path_frame_;
    float ship_path_param_ = 0; // current parametric distance of ship along the curve

    /// Piecewise degree-3 Bezier spline.
    PiecewiseBezier ship_path_;

    // Control polygon for cubic spline
    std::vector<vec3> control_polygon_ = {
        {2.0, 2.0, 0.0},
        {3.0, 0.0, 0.0},
        {3.0, 0.0, -2.0},
        {0.0, 2.0, -3.0},
        {-2.0, 2.0, -1.0},
        {-3.0, 3.0, 0.0},
        {-1.0, 1.0, 1.0}
    };

    /// the field of view for the camera
    float fovy_;
    /// the near plane for the virtual camera
    float near_;
    /// the far plane for the virtual camera
    float far_;

    /// which planet are we looking at (control with key 1-6)
    const Planet* planet_to_look_at_;

    /// rotation in x direction around the planet/sun from the original point
    float x_angle_;
    /// rotation in y direction around the planet/sun from the original point
    float y_angle_;
    /// eye's distance in radii from the observed planet
    float dist_factor_;
    /// true, if we look at the spaceship
    bool in_ship_;
    /// x-rotation of the billboard
    float billboard_x_angle_;
    /// y-rotation of the billboard
    float billboard_y_angle_;

    /// current viewport dimension
    int  width_, height_;
};


//=============================================================================
#endif
//=============================================================================
