//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "solar_viewer.h"
#include "glmath.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <array>
#include <math.h>

#define PI 3.141592653

//=============================================================================


Solar_viewer::Solar_viewer(const char* _title, int _width, int _height)
    : GLFW_window(_title, _width, _height),
      unit_sphere_(50), //level of tesselation

      /** Use the following for better planet distances/sizes (but still not realistic)
      * To get a true-to-scale solar system, planets would be 20x smaller, and their distance to the sun would be ~11x larger
      * For example r_mercury/r_sun = 0.0034 and distance_mercury_to_sun/r_sun = 33.3
      **/
      //  sun_    (0.0f,              2.0f*(float)(M_PI)/26.0f,   1.0f,    0.0f),
      //  mercury_(2.0f*(float)(M_PI)/116.0f,  2.0f*(float)(M_PI)/58.5f,   0.068f, -3.1f),
      //  venus_  (2.0f*(float)(M_PI)/225.0f,  2.0f*(float)(M_PI)/243.0f,  0.174f,   -7.2f),
      //  earth_  (2.0f*(float)(M_PI)/365.0f,  2.0f*(float)(M_PI),        0.182f,   -9.8f),
      //  moon_   (2.0f*(float)(M_PI)/27.0f,   0.0f,  0.048f,   -0.5f),
      //  mars_   (2.0f*(float)(M_PI)/687.0f,  2.0f*(float)(M_PI)*24.0f/25.0f, 0.098f,-13.8f),
      //  stars_  (0.0f, 0.0f, 30.0f, 0.0f)

      // Even more unrealistic placement/sizing for nicer visualization.
      sun_    (0.0,              2.0*M_PI/26.0,   1.0f,    0.0f),
      mercury_(2.0*M_PI/116.0f,  2.0*M_PI/58.5,   0.075f, -1.4f),
      venus_  (2.0*M_PI/225.0f,  2.0*M_PI/243.0,  0.2f,   -2.2f),
      earth_  (2.0*M_PI/365.0f,  2.0*M_PI,        0.25,   -3.3f),
      moon_   (2.0*M_PI/27.0f,   0.0,  0.04,   -0.4f),
      mars_   (2.0*M_PI/687.0f,  2.0*M_PI*24.0/25.0, 0.15,-5.0f),
      stars_  (0.0, 0.0, 21.0, 0.0)
{
    // start animation
    timer_active_ = true;
    time_step_ = 1.0f/24.0f; // one hour

    // rendering parameters
    greyscale_     = false;
    fovy_ = 45;
    near_ = 0.01f;
    far_  = 20;

    // viewing angle
    planet_to_look_at_ = &earth_;
    x_angle_ = 0.0f;
    y_angle_ = 0.0f;
    dist_factor_ = 4.5f;

    ship_.pos_ = planet_to_look_at_->pos_ - vec4(0.0f, 0.0f, dist_factor_*planet_to_look_at_->radius_, 0.0f);
    ship_.direction_ = vec4(0.0f, 0.0f, 1.0f,0.0f);
    in_ship_ = false;

    srand((unsigned int)time(NULL));
}

//-----------------------------------------------------------------------------

void
Solar_viewer::
keyboard(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        // Change view between the various bodies with keys 1..6
        if ((key >= GLFW_KEY_1) && (key <= GLFW_KEY_6)) {
            std::array<const Planet *, 6> bodies = { &sun_, &mercury_, &venus_, &earth_, &moon_, &mars_};
            in_ship_ = false;
            planet_to_look_at_ = bodies.at(key - GLFW_KEY_1);
        }
        switch (key)
        {
            // Key 7 switches to viewing the ship.
            case GLFW_KEY_7:
            {
                planet_to_look_at_ = NULL;
                in_ship_ = true;
                break;
            }

            // \todo Paste your dist_factor_-modifying keypress code from assignment 5.
            case GLFW_KEY_8:
            {
				dist_factor_ -= 0.1f;
				
				if (dist_factor_ < 2.5f) 
					dist_factor_ = 2.5f;
					
				//std::cout << "Dist factor is " << dist_factor_ << std::endl;
				
                break;
            }
            
            case GLFW_KEY_9:
            {
                dist_factor_ += 0.1f;
                
                if (dist_factor_ > 20.0f)
                	dist_factor_ = 20.0f;
                	
                //std::cout << "Dist factor is " << dist_factor_ << std::endl;
                
                break;
            }

            case GLFW_KEY_R:
            {
                randomize_planets();
                break;
            }

            case GLFW_KEY_G:
            {
                greyscale_ = !greyscale_;
                break;
            }

            case GLFW_KEY_W:
            {
                if (in_ship_)
                    ship_.accelerate(0.001f);
                break;
            }
            case GLFW_KEY_S:
            {
                if (in_ship_)
                    ship_.accelerate(-0.001f);
                break;
            }
            case GLFW_KEY_A:
            {
                if (in_ship_)
                    ship_.accelerate_angular(0.02f);
                break;
            }
            case GLFW_KEY_D:
            {
                if (in_ship_)
                    ship_.accelerate_angular(-0.02f);
                break;
            }

            case GLFW_KEY_LEFT:
            {
                y_angle_ -= 10.0;
                break;
            }

            case GLFW_KEY_RIGHT:
            {
                y_angle_ += 10.0;
                break;
            }

            case GLFW_KEY_DOWN:
            {
                x_angle_ += 10.0;
                break;
            }

            case GLFW_KEY_UP:
            {
                x_angle_ -= 10.0;
                break;
            }

            case GLFW_KEY_SPACE:
            {
                timer_active_ = !timer_active_;
                break;
            }

            case GLFW_KEY_P:
            case GLFW_KEY_KP_ADD:
            case GLFW_KEY_EQUAL:
            {
                time_step_ *= 2.0f;
                std::cout << "Time step: " << time_step_ << " days\n";
                break;
            }

            case GLFW_KEY_M:
            case GLFW_KEY_KP_SUBTRACT:
            case GLFW_KEY_MINUS:
            {
                time_step_ *= 0.5f;
                std::cout << "Time step: " << time_step_ << " days\n";
                break;
            }

            case GLFW_KEY_ESCAPE:
            {
                glfwSetWindowShouldClose(window_, GL_TRUE);
                break;
            }
        }
    }
}

// Update the current positions of the celestial bodies based their angular distance
// around their orbits. This position is needed to set up the camera in the scene
// (see Solar_viewer::paint)
void Solar_viewer::update_body_positions() {
    // \todo Paste your planet positioning code from assignment 5 here.
    vec4 solar_orbit_center = sun_.pos_;
    vec4 earth_orbit_center = earth_.pos_;

    //update mercury
    vec4 mercury_relative_pos = vec4(cos(mercury_.angle_orbit_),0,-sin(mercury_.angle_orbit_),0);
    mercury_.pos_ = solar_orbit_center + mercury_.distance_ * mercury_relative_pos;

    //update venus
    vec4 venus_relative_pos = vec4(cos(venus_.angle_orbit_),0,-sin(venus_.angle_orbit_),0);
    venus_.pos_ = solar_orbit_center + venus_.distance_ * venus_relative_pos;

    //update earth
    vec4 earth_relative_pos = vec4(cos(earth_.angle_orbit_),0,-sin(earth_.angle_orbit_),0);
    earth_.pos_ = solar_orbit_center + earth_.distance_ * earth_relative_pos;

    //update moon
    vec4 moon_relative_pos = vec4(cos(moon_.angle_orbit_),0,-sin(moon_.angle_orbit_),0);
    moon_.pos_ = earth_orbit_center + moon_.distance_ * moon_relative_pos;

    //update mars
    vec4 mars_relative_pos = vec4(cos(mars_.angle_orbit_),0,-sin(mars_.angle_orbit_),0);
    mars_.pos_ = solar_orbit_center + mars_.distance_ * mars_relative_pos;

    //update stars
    vec4 stars_relative_pos = vec4(cos(stars_.angle_orbit_),0,-sin(stars_.angle_orbit_),0);
    stars_.pos_ = solar_orbit_center + stars_.distance_ * stars_relative_pos;
}

//-----------------------------------------------------------------------------


void Solar_viewer::timer()
{
    if (timer_active_)
    {
        sun_.time_step(time_step_);
        mercury_.time_step(time_step_);
        venus_.time_step(time_step_);
        earth_.time_step(time_step_);
        moon_.time_step(time_step_);
        mars_.time_step(time_step_);
        update_body_positions();

        ship_.update_ship();
    }
}


//-----------------------------------------------------------------------------


void Solar_viewer::resize(int _width, int _height)
{
    width_  = _width;
    height_ = _height;
    glViewport(0, 0, _width, _height);
}


//-----------------------------------------------------------------------------


void Solar_viewer::initialize()
{
    // set initial state
    glClearColor(1,1,1,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

    // Allocate textures
    sun_    .tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    mercury_.tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    venus_  .tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

    earth_  .tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    earth_.night_.init(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    earth_.cloud_.init(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    earth_.gloss_.init(GL_TEXTURE3, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

    moon_   .tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    mars_   .tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    stars_  .tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);
    ship_   .tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

    sunglow_.tex_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

    // Load/generate textures
    sun_    .tex_.loadPNG(TEXTURE_PATH "/sun.png");
    mercury_.tex_.loadPNG(TEXTURE_PATH "/mercury.png");
    venus_  .tex_.loadPNG(TEXTURE_PATH "/venus.png");

    earth_  .tex_.loadPNG(TEXTURE_PATH "/day.png");
    earth_.night_.loadPNG(TEXTURE_PATH "/night.png");
    earth_.cloud_.loadPNG(TEXTURE_PATH "/clouds.png");
    earth_.gloss_.loadPNG(TEXTURE_PATH "/gloss.png");

    moon_   .tex_.loadPNG(TEXTURE_PATH "/moon.png");
    mars_   .tex_.loadPNG(TEXTURE_PATH "/mars.png");
    stars_  .tex_.loadPNG(TEXTURE_PATH "/stars2.png");

    ship_.     load_model(TEXTURE_PATH "/spaceship.off");
    ship_   .tex_.loadPNG(TEXTURE_PATH "/ship.png");

    sunglow_.tex_.createSunBillboardTexture();

    // setup shaders
    color_shader_.load(SHADER_PATH "/color.vert", SHADER_PATH "/color.frag");
    phong_shader_.load(SHADER_PATH "/phong.vert", SHADER_PATH "/phong.frag");
    earth_shader_.load(SHADER_PATH "/earth.vert", SHADER_PATH "/earth.frag");
      sun_shader_.load(SHADER_PATH   "/sun.vert", SHADER_PATH   "/sun.frag");
}
//-----------------------------------------------------------------------------


void Solar_viewer::paint()
{
    // clear framebuffer and depth buffer first
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // \todo Paste your viewing/navigation code from assignment 5 here.
//    vec4     eye = vec4(0,0,7,1.0);
//    vec4  center = sun_.pos_;
//    vec4      up = vec4(0,1,0,0);
//    float radius = sun_.radius_;
//    mat4    view = mat4::look_at(vec3(eye), (vec3)center, (vec3)up);
    
    vec4 center, eye, up;
    float radius;
    if (in_ship_) {
    	center = ship_.pos_;
    	eye = center + mat4::rotate_y(y_angle_ + ship_.angle_) * vec4(0,dist_factor_*ship_.radius_*0.85, -dist_factor_*ship_.radius_*3,0);
    	up = mat4::rotate_y(y_angle_ + ship_.angle_) * vec4(0,1,0,0);    	
    }
    else {	
		//1. get view center
		center = planet_to_look_at_->pos_;
		//2. calculate eye position
		radius = planet_to_look_at_->radius_;
		eye = center + mat4::rotate_y(y_angle_) * mat4::rotate_x(x_angle_) * vec4(0,0,dist_factor_*radius,0);
		//3. calculate up after rotation
		up = mat4::rotate_y(y_angle_) * mat4::rotate_x(x_angle_) * vec4(0,1,0,0);
	}
	
	//4. get view mat
	mat4 view = mat4::look_at(vec3(eye), vec3(center), vec3(up));

    /** \todo Orient the billboard used to display the sun's glow
     *  Update billboard_x_andle_ and billboard_y_angle_ so that the billboard plane
     *  drawn to produce the sun's halo is orthogonal to the view vector for
     *  the sun's center.
     */
    billboard_x_angle_ = x_angle_;
    billboard_y_angle_ = y_angle_;

    mat4 projection = mat4::perspective(fovy_, (float)width_/(float)height_, near_, far_);
    draw_scene(projection, view);

}


//-----------------------------------------------------------------------------


void Solar_viewer::draw_scene(mat4& _projection, mat4& _view)
{
    // the matrices we need: model, modelview, modelview-projection, normal
    mat4 m_matrix;
    mat4 mv_matrix;
    mat4 mvp_matrix;
    mat3 n_matrix;

    // the sun is centered at the origin and -- for lighting -- considered to be a point, so that is the light position in world coordinates
    vec4 light = vec4(0.0, 0.0, 0.0, 1.0); //in world coordinates
    // convert light into camera coordinates
    light = _view * light;

    static float sun_animation_time = 0;
    if (timer_active_) sun_animation_time += 0.01f;

    // render sun
    m_matrix = mat4::rotate_y(sun_.angle_self_) * mat4::scale(sun_.radius_);
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    sun_shader_.use();
    sun_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    sun_shader_.set_uniform("t", sun_animation_time, true /* Indicate that time parameter is optional;
                                                             it may be optimized away by the GLSL    compiler if it's unused. */);
    sun_shader_.set_uniform("tex", 0);
    sun_shader_.set_uniform("greyscale", (int)greyscale_);
    sun_.tex_.bind();
    unit_sphere_.draw();
    

    // \todo Paste your star/planet/moon/ship drawing calls from assignment 5 here.
 	//render STAR
	m_matrix = mat4::translate(stars_.pos_) * mat4::rotate_y(stars_.angle_self_) * mat4::scale(stars_.radius_);
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    color_shader_.use();
    color_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    color_shader_.set_uniform("tex", 0);
    color_shader_.set_uniform("greyscale", (int)greyscale_);
    stars_.tex_.bind();
    unit_sphere_.draw();

	//render MERCURY   
	m_matrix = mat4::translate(mercury_.pos_) * mat4::rotate_y(mercury_.angle_self_) * mat4::scale(mercury_.radius_);
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    n_matrix = transpose(inverse(mat3(mv_matrix)));
    phong_shader_.use();
    phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    phong_shader_.set_uniform("modelview_matrix", mv_matrix);
    phong_shader_.set_uniform("normal_matrix", n_matrix);
    phong_shader_.set_uniform("light_position", light);
    phong_shader_.set_uniform("tex", 0);
    phong_shader_.set_uniform("greyscale", (int)greyscale_);
    mercury_.tex_.bind();
    unit_sphere_.draw();
    
    //render VENUS     
	m_matrix = mat4::translate(venus_.pos_) * mat4::rotate_y(venus_.angle_self_) * mat4::scale(venus_.radius_);
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    n_matrix = transpose(inverse(mat3(mv_matrix)));
    phong_shader_.use();
    phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    phong_shader_.set_uniform("modelview_matrix", mv_matrix);
    phong_shader_.set_uniform("normal_matrix", n_matrix);
    phong_shader_.set_uniform("light_position", light);
    phong_shader_.set_uniform("tex", 0);
    phong_shader_.set_uniform("greyscale", (int)greyscale_);
    venus_.tex_.bind();
    unit_sphere_.draw();
    
    //render EARTH
	m_matrix = mat4::translate(earth_.pos_) * mat4::rotate_y(earth_.angle_self_) * mat4::scale(earth_.radius_);
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    n_matrix = transpose(inverse(mat3(mv_matrix)));
    earth_shader_.use();
    earth_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    earth_shader_.set_uniform("modelview_matrix", mv_matrix);
    earth_shader_.set_uniform("normal_matrix", n_matrix);
    earth_shader_.set_uniform("light_position", light);
    earth_shader_.set_uniform("day_texture", 0);
    earth_shader_.set_uniform("night_texture", 1);
    earth_shader_.set_uniform("cloud_texture", 2);
    earth_shader_.set_uniform("gloss_texture", 3);
    earth_shader_.set_uniform("greyscale", (int)greyscale_);
    earth_.night_.bind();
    earth_.gloss_.bind();
    earth_.cloud_.bind();
    earth_.tex_.bind();
    unit_sphere_.draw();
    
    //std::cout << glGetUniformLocation(phong_shader_.pid_,"modelview_projection_matrix") << std::endl;
    
    //render MOON     
	m_matrix = mat4::translate(moon_.pos_) * mat4::rotate_y(moon_.angle_self_) * mat4::scale(moon_.radius_);
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    n_matrix = transpose(inverse(mat3(mv_matrix)));
    phong_shader_.use();
    phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    phong_shader_.set_uniform("modelview_matrix", mv_matrix);
    phong_shader_.set_uniform("normal_matrix", n_matrix);
    phong_shader_.set_uniform("light_position", light);
    phong_shader_.set_uniform("tex", 0);
    phong_shader_.set_uniform("greyscale", (int)greyscale_);
    moon_.tex_.bind();
    unit_sphere_.draw();
    
    //render MARS     
	m_matrix = mat4::translate(mars_.pos_) * mat4::rotate_y(mars_.angle_self_) * mat4::scale(mars_.radius_);
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    n_matrix = transpose(inverse(mat3(mv_matrix)));
    phong_shader_.use();
    phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    phong_shader_.set_uniform("modelview_matrix", mv_matrix);
    phong_shader_.set_uniform("normal_matrix", n_matrix);
    phong_shader_.set_uniform("light_position", light);
    phong_shader_.set_uniform("tex", 0);
    phong_shader_.set_uniform("greyscale", (int)greyscale_);
    mars_.tex_.bind();
    unit_sphere_.draw();
    
    //render SHIP
    if (in_ship_) {
		m_matrix = mat4::translate(ship_.pos_) * mat4::rotate_y(ship_.angle_) * mat4::scale(ship_.radius_);
    mv_matrix = _view * m_matrix;
		mvp_matrix = _projection * mv_matrix;
		n_matrix = transpose(inverse(mat3(mv_matrix)));
		phong_shader_.use();
		phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
		phong_shader_.set_uniform("modelview_matrix", mv_matrix);
		phong_shader_.set_uniform("normal_matrix", n_matrix);
		phong_shader_.set_uniform("light_position", light);
		phong_shader_.set_uniform("tex", 0);
		phong_shader_.set_uniform("greyscale", (int)greyscale_);
		ship_.tex_.bind();
		ship_.draw();
    }

    /** \todo Switch from using color_shader_ to the fancier shaders you'll
     * implement in this assignment:
     *      mercury, venus, moon, mars, ship: phong_shader_
     *      earth: earth_shader_
     *      stars, sunglow: still use color_shader_
     *  You'll need to make sure all the GLSL uniform variables are set. For
     *  Phong shading, you need to pass in the modelview matrix, the normal transformation
     *  matrix, and light position in addition to the color_shader_ parameters.
     */

    /** \todo Render the sun's halo here using the "color_shader_"
    *   - Construct a model matrix that scales the billboard to 3 times the
    *     sun's radius and orients it according to billboard_x_angle_ and
    *     billboard_y_angle_
    *   - Bind the texture for and draw sunglow_
    **/
    
    //render billboard     
    m_matrix = mat4::rotate_y(billboard_y_angle_) * mat4::rotate_x(billboard_x_angle_) * mat4::scale(3 * sun_.radius_);
    mv_matrix = _view * m_matrix;
    mvp_matrix = _projection * mv_matrix;
    color_shader_.use();
    color_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    color_shader_.set_uniform("tex", 0);
    color_shader_.set_uniform("greyscale", (int)greyscale_);
    sunglow_.tex_.bind();
    sunglow_.draw();

    // check for OpenGL errors
    glCheckError();
}

void Solar_viewer::randomize_planets()
{
    std::cout << "Randomizing planets..." << std::endl;
    float temp_dt = time_step_;
    time_step_ = (float)(rand()%20000);
    timer();
    time_step_ = temp_dt;
}


//=============================================================================
