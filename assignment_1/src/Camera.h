//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef CAMERA_H
#define CAMERA_H


//== INCLUDES =================================================================

#include "vec3.h"


//== CLASS DEFINITION =========================================================


/// \class Camera Camera.h
/// The camera class stores the following camera parameters in its
/// member variables: eye, center, up, fovy, width, and height. The function primary_ray(x,y)
/// produces a ray from the camera center through the pixel (x,y).
class Camera
{
public:

	
    /// Default (empty) constructor
    Camera() {}

	
    /// Construct camera by specifying all required data. Calls init().
    /// \param[in] _eye Camera center
    /// \param[in] _center Center of the scene, which the camera is looking at.
    /// \param[in] _up Vector specifying the up direction.
    /// \param[in] _fovy Opening angle (field of view) in y-direction
    /// \param[in] _width Width of the image (in pixels)
    /// \param[in] _height Height of the image (in pixels)
    Camera(const vec3&   _eye,
           const vec3&   _center,
           const vec3&   _up,
           double        _fovy,
           unsigned int  _width,
           unsigned int  _height)
    : eye(_eye), center(_center), up(_up), fovy(_fovy), width(_width), height(_height)
    {
        init();
    }


	/// This function precomputes some variables that are later required for primary_ray()
    void init()
    {
        // compute viewing direction and distance of eye to scene center
        vec3  view = normalize(center - eye);
        double dist = distance(center, eye);

        // compute width & height of the image plane
        // based on the opening angle of the camera (fovy) and the distance
        // of the eye to the near plane (dist)
        double w = width;
        double h = height;
        double image_height = 2.0 * dist * tan(0.5*fovy/180.0*M_PI);
        double image_width  = w/h * image_height;

        // compute right and up vectors on the image plane
        x_dir = normalize( cross(view, up) ) * image_width / w;
        y_dir = normalize( cross(x_dir, view) ) * image_height / h;

        // compute lower left corner on the image plane
        lower_left = center - 0.5f*w*x_dir - 0.5f*h*y_dir;
    }


    /// create a ray for a pixel in the image
	/// \param[in] _x pixel location in image
	/// \param[in] _y pixel location in image
    Ray primary_ray(unsigned int _x, unsigned int _y) const
    {
        return Ray(eye, lower_left + static_cast<double>(_x)*x_dir + static_cast<double>(_y)*y_dir - eye);
    }


public:

    /// position of the eye in 3D space (camera center)
    vec3 eye;
	
    /// the center of the scene the camera is looking at
	vec3 center;

	/// up-direction of the camera
	vec3 up;

	/// opening angle (field of view) in y-direction
    double  fovy;

	/// image width in pixels
    unsigned int width;

	/// image height in pixels
	unsigned int height;


private:

    vec3 x_dir;
	vec3 y_dir;
	vec3 lower_left;
};


//-----------------------------------------------------------------------------


/// read camera data from stream
inline std::istream& operator>>(std::istream& is, Camera& c)
{
    is >> c.eye >> c.center >> c.up >> c.fovy >> c.width >> c.height;
    c.init();
    return is;
}


//=============================================================================
#endif // RAY_H defined
//=============================================================================
