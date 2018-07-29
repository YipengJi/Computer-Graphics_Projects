//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

#ifndef SCENE_H
#define SCENE_H

//== INCLUDES =================================================================

#include "StopWatch.h"
#include "Object.h"
#include "Light.h"
#include "Ray.h"
#include "Material.h"
#include "Image.h"
#include "Camera.h"

#include <memory>
#include <string>

//== CLASS DEFINITION =========================================================

/// \class Sphere Sphere.h
/// This class loads and raytraces scenes consisting of cameras, lights, and
/// objects
class Scene {
public:
    /// Constructor loads scene from file.
    Scene(const std::string &path) {
        read(path);
    }

    /// Allocate image and raytrace the scene.
    Image  render();

    /// Determine the color seen by a viewing ray
    /**
    *	@param[in] _ray passed Ray
    *	@param[in] _depth holds the information, how many times the `_ray` had been reflected. Goes from 0 to max_depth. Should be used for recursive function call.
    *	@return    color
    **/	
    vec3  trace(const Ray& _ray, int _depth);

    /// Computes the closest intersection point between a ray and all objects in the scene.
    /**
    *   	@param _ray Ray that should be tested for intersections with all objects in the scene.
    *   	@param _Object_ptr Output parameter which holds the object from the scene, intersected by the `_ray`, closest to the `_ray`'s origin.
    *   	@param _point returns intersection point
    *   	@param _normal returns normal at `_point`
    *   	@param _t returns distance between the `_ray`'s origin and `_point`
    *   	@return returns `true`, if there is an intersection point between `_ray` and at least one object in the scene.
    **/
    bool  intersect(const Ray& _ray, Object_ptr&, vec3& _point, vec3& _normal, double& _t);

    /// Computes the phong lighting for a given object intersection
    /**
    *	@param _point the point, whose color should be determined.
    *	@param _normal `_point`'s normal
    *	@param _view normalized direction from the point to the viewer's position.
    * 	@param _material holds material parameters of the `_point`, that should be lit.
    */
    vec3  lighting(const vec3& _point, const vec3& _normal, const vec3& _view, const Material& _material);

    void read(const std::string &filename);

    size_t numObjects() const { return objects.size(); }

    // Accessors for scene objects and camera for debugging.
    const std::vector<std::unique_ptr<Object>> &getObjects() const { return objects; }
    const Camera &getCamera() const { return camera; }

private:
    /// camera stores eye position, view direction, and can generate primary rays
    Camera camera;

    /// array for all lights in the scene
    std::vector<Light> lights;

    /// array for all the objects in the scene
    std::vector<std::unique_ptr<Object>> objects;

    /// max recursion depth for mirroring
    int max_depth = 0;

    /// background color
    vec3 background = vec3(0, 0, 0);

    /// global ambient light
    vec3 ambience = vec3(0, 0, 0);
};

//=============================================================================
#endif // SCENE_H defined
//=============================================================================
