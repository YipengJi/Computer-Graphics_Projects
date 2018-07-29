#ifndef SHIP_H
#define SHIP_H

#include "glmath.h"
#include "gl.h"
#include <vector>
#include "texture.h"

class Ship
{
    public:
        Ship();
        ~Ship();

        /// loads vertices and faces from .off file
        bool load_model(const char* filename);

        /// updates ships position and angle
        void update_ship();

        /// changes ship's forward speed
        void accelerate(float speedup);

        /// changes ship's angular speed
        void accelerate_angular(float angular_speedup);

        /// draws the ship
        void draw(GLenum mode = GL_TRIANGLES);

        /// main diffuse texture for the planet
        Texture tex_;
    private:
        void compute_normals();
        void initialize_buffers();


        /// vertex array
        std::vector<vec3> vertices_;
        /// triangle index array
        std::vector<int> indices_;
        /// vertex normals
        std::vector<vec3> vertex_normals_;
        /// face normals
        std::vector<vec3> face_normals_;

        /// indices of the triangle vertices
        unsigned int n_indices_;

        // vertex array object
        GLuint vao_ = 0;
        /// vertex buffer object
        GLuint vbo_ = 0;
        /// texture coordinates buffer object
        GLuint tbo_ = 0;
        /// normals buffer object
        GLuint nbo_ = 0;
        /// index buffer object
        GLuint ibo_ = 0;



    public:
        /// current position
        vec4 pos_;

        /// current y-axis angle
        float angle_;

        /// current direction in which the ship faces
        vec4 direction_;

        /// current forward speed (pos_ += speed*direction)
        float speed_;

        /// current angular speed (angle_ += angular_speed)
        float angular_speed_;

        /// ship's radius
        float radius_;
};


#endif
