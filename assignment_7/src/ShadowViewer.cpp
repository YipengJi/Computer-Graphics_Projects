//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#include "ShadowViewer.h"
#include "dumpOpenGLBuffer.h"
#include "Quad.h"
#include <cassert>

void ShadowViewer::initialize() {
    m_initialized = true;
    m_quad.initialize();
    m_updateMesh();
    m_cubeMapVisualization.initialize();

    m_phong_shader        .load(SHADER_PATH "/phong_shadow.vert",  SHADER_PATH "/phong_shadow.frag");
    m_solid_color_shader  .load(SHADER_PATH "/solid_color.vert",   SHADER_PATH "/solid_color.frag");
    m_shadowmap_gen_shader.load(SHADER_PATH "/shadowmap_gen.vert", SHADER_PATH "/shadowmap_gen.frag");

    m_shadowMap = std::unique_ptr<PointLightShadowMap>(new PointLightShadowMap());
}

// Replace the mesh with one read from m_meshPath
void ShadowViewer::m_updateMesh() {
    if (!m_initialized || m_meshPath.empty()) return; // We can only build the mesh after OpenGL has been initialized.
    if (!m_mesh) m_mesh = std::unique_ptr<Mesh>(new Mesh(m_meshPath));
    else         m_mesh->read(m_meshPath);
}

// Construct the viewing matrix needed to render cube face 'cube_face' of the
// shadow cube map for light 'li'.
mat4 ShadowViewer::m_constructLightViewMatrix(size_t li, size_t cube_face) const {
    assert((li < m_numActiveLights) && (cube_face < 6));
    mat4 scene_view_matrix = m_constructSceneViewMatrix();
    /** \todo
    * Construct the viewing matrix for rendering the scene from the perspective
    * of the light looking through cube face number cube_face.
    * Recall that the cube faces are indexed in the order: +x, -x, +y, -y, +z, -z
    * See the assignment handout for an illustration of how to orient the "light camera".
    * Note, for compatibility with the Phong lighting calculation done in eye
    * space, the cube face should be oriented relative to the eye coordinates
    * defined by scene_view_matrix.
    * Hint: use mat4::look_at
    **/
    
    vec3 light_pos = scene_view_matrix * m_light[li].position();
    vec3 center, up;
    switch (cube_face) {
		case 0:
		    up = vec3(0, 1, 0);
		    center = light_pos + vec3(1.0f,0,0);     
		    break;
		case 1:
		    up = vec3(0, 1, 0);
		    center = light_pos + vec3(-1.0f,0,0);
		    break;
		case 4:
		    up = vec3(0, 1, 0);
		    center = light_pos + vec3(0,0,1.0f);
		    break;
		case 5:
		    up = vec3(0, 1, 0);
		    center = light_pos + vec3(0,0,-1.0f);
		    break;
		case 2:
		    up = vec3(0, 0, -1);
		    center = light_pos + vec3(0,1.0f,0);
		    break;
		case 3:
		    up = vec3(0, 0, 1);
		    center = light_pos + vec3(0,-1.0f,0);
		    break;
		default:
		    break;
    }
    mat4 directions = mat4::look_at(light_pos,center,up);
    
    return directions * scene_view_matrix;
    //return mat4::identity() * scene_view_matrix;
}

mat4 ShadowViewer::m_constructLightProjectionMatrix() const {
    /** \todo
    * Construct the projection matrix for rendering the scene from the perspective
    * of the light to generate shadow maps.
    **/
    
    return mat4::perspective(90.0f, 1.0f, 0.1f, 6.0f);
    //return mat4::identity();
}

void ShadowViewer::m_render_shadow_cubemap(size_t li, const mat4 &plane_m_matrix, const mat4 &mesh_m_matrix) {
    // Render shadow maps to the cube map texture
    // Loop over cube faces in order: +x, -x, +y, -y, +z, -z
    for (size_t cube_face = 0; cube_face < 6; ++cube_face) {
        m_shadowMap->selectCubeFaceTextureAsRenderTarget(cube_face);

        // Clear depth map with high depths
        glClearColor(100.0f, 100.0f, 100.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shadowmap_gen_shader.use();
        mat4 light_view = m_constructLightViewMatrix(li, cube_face);
        mat4 light_proj = m_constructLightProjectionMatrix();
        m_shadowmap_gen_shader.set_uniform("modelview_matrix",                         light_view * plane_m_matrix, true);
        m_shadowmap_gen_shader.set_uniform("modelview_projection_matrix", light_proj * light_view * plane_m_matrix);
        m_quad.draw();

        m_shadowmap_gen_shader.set_uniform("modelview_matrix",                         light_view * mesh_m_matrix, true);
        m_shadowmap_gen_shader.set_uniform("modelview_projection_matrix", light_proj * light_view * mesh_m_matrix);
        m_mesh->draw();

        // Switch back to rendering to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_width, m_height);
    }
}

// Draw the scene using view_matrix and projection_matrix.
// Note, view_matrix can differ from m_constructSceneViewMatrix(),
// e.g. when viewing the scene from the perspective of the shadow cube faces
// for debugging.
void ShadowViewer::draw(const mat4 &view_matrix, const mat4 &projection_matrix) {
    if (!m_mesh) throw std::runtime_error("No mesh to draw!");

    // Clear framebuffer and depth buffer first
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // We need to re-draw the front-most fragments multiple times
                            // (we cannot use the default setting of GL_LESS)
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    /////////////////////////////////////
    // Transformation/projection matrices
    /////////////////////////////////////
    mat4 plane_m_matrix   = mat4::scale(3.0) * mat4::rotate_x(90.0f);
    mat4 plane_mv_matrix  = view_matrix * plane_m_matrix;
    mat4 plane_mvp_matrix = projection_matrix * plane_mv_matrix;

    mat4 mesh_m_matrix    = m_mesh->modelMatrix();
    mat4 mesh_mv_matrix   = view_matrix * mesh_m_matrix;
    mat4 mesh_mvp_matrix  = projection_matrix * mesh_mv_matrix;


    // \todo Construct the matrices for transforming normals into eye coordinates
    //       You can paste in your solution from assignment 6.
    mat3 plane_n_matrix   = transpose(inverse(mat3(plane_mv_matrix)));
    mat3 mesh_n_matrix    = transpose(inverse(mat3(mesh_mv_matrix)));

    vec3 ambient_light(0.2, 0.2, 0.2),
        plane_diffuse (0.5, 0.5, 0.7), // used as ambient color too
        plane_specular(0.0, 0.0, 0.0),
         mesh_diffuse (1.0, 0.0, 0.0), // used as ambient color too
         mesh_specular(1.0, 1.0, 1.0);

    // Render the ambient light contribution
    m_solid_color_shader.use();
    m_solid_color_shader.set_uniform("color", ambient_light * plane_diffuse);
    m_solid_color_shader.set_uniform("modelview_projection_matrix", plane_mvp_matrix);
    m_quad.draw();

    m_solid_color_shader.use();
    m_solid_color_shader.set_uniform("color", ambient_light * mesh_diffuse);
    m_solid_color_shader.set_uniform("modelview_projection_matrix", mesh_mvp_matrix);
    m_mesh->draw();

    // Render contribution of each light
    for (size_t li = 0; li < m_numActiveLights; ++li) {
        m_render_shadow_cubemap(li, plane_m_matrix, mesh_m_matrix);

        /** \todo
        * Configure OpenGL to *add* the specular and diffuse contribution
        * output by our shader to the colors already in the framebuffer.
        * Hint: read the documentation for glBlendFunc
        **/
        glEnable(GL_BLEND);
        
        glBlendFunc(GL_ONE,GL_ONE);

        m_phong_shader.use();
        m_shadowMap->bind();

        m_phong_shader.set_uniform("shininess", 8.0f); // pass 'optional = true' to avoid 'Invalid uniform location'
        m_phong_shader.set_uniform("shadow_map",   0); // warnings caused by incomplete shader implementations

        /** \todo
         * Draw this light's specular and diffuse contribution on the floor
         * plane (m_quad) and mesh (m_mesh) (taking the shadows into account).
         * You'll need to pass in the light position ***in eye coordinates** as
         * well as the proper material and transformation matrices.
         **/
         
//        vec3 light_pos = scene_view_matrix * m_light[li].position();
//        vec3 light_color = ambient_light;
//        
//        m_phong_shader.set_uniform("light_position",light_pos, true); // warnings caused by incomplete shader implementations
//        m_phong_shader.set_uniform("light_color",light_color, true); // warnings caused by incomplete shader implementations
       
        //for both
        m_phong_shader.set_uniform("light_position", vec3(view_matrix * m_light[li].position()));
        m_phong_shader.set_uniform("light_color", m_light[li].color);
        
        //for m_quad
        m_phong_shader.set_uniform("modelview_projection_matrix", plane_mvp_matrix);
  	 	m_phong_shader.set_uniform("modelview_matrix", plane_mv_matrix);
   		m_phong_shader.set_uniform("normal_matrix", plane_n_matrix);

        m_phong_shader.set_uniform("diffuse_color", plane_diffuse);
        m_phong_shader.set_uniform("specular_color", plane_specular);
        m_quad.draw();

        //for m_mesh
        m_phong_shader.set_uniform("modelview_projection_matrix", mesh_mvp_matrix);
  	 	m_phong_shader.set_uniform("modelview_matrix", mesh_mv_matrix);
   		m_phong_shader.set_uniform("normal_matrix", mesh_n_matrix);

        m_phong_shader.set_uniform("diffuse_color", mesh_diffuse);
        m_phong_shader.set_uniform("specular_color", mesh_specular);
        m_mesh->draw();
        
        m_shadowMap->unbind();

        // All other shaders should overwrite the framebuffer color, not add to it...
        glDisable(GL_BLEND);
    }

    // Draw the lights with solid color spheres
    m_solid_color_shader.use();
    for (size_t i = 0; i < m_numActiveLights; ++i) {
        mat4 mvp_matrix = projection_matrix * view_matrix * mat4::translate(m_light[i].position()) * mat4::scale(0.05);
        m_solid_color_shader.set_uniform("modelview_projection_matrix", mvp_matrix);
        vec3 color = (m_selectedLight == i) ? vec3(0.5, 1.0, 0.5)
                                            : vec3(0.9, 0.9, 0.9);
        m_solid_color_shader.set_uniform("color", color);
        m_unitSphere.draw();
    }

    // Draw a visualization of the cube map computed for the *last* light
    // (since it's the last light that drew to the shadow map buffer).
    if ((m_numActiveLights > 0) && (m_cubeMapVisualizationMode > 0)) {
        mat4 scene_view = m_constructSceneViewMatrix();
        m_cubeMapVisualization.demoTextures = m_cubeMapVisualizationMode == 1;
        m_cubeMapVisualization.draw(scene_view, view_matrix, projection_matrix,
                                    scene_view * m_light[m_numActiveLights - 1].position(), *m_shadowMap);
    }

    if (m_screenshotRequested) {
        static int screenshot_counter = 0;
        ++screenshot_counter;
        dumpOpenGLBuffer("screenshot_" + std::to_string(screenshot_counter) + ".png", m_width, m_height);
        m_screenshotRequested = false;
    }
}

mat4 ShadowViewer::m_constructSceneViewMatrix() const {
    vec3 center(0.0f, 1.0f, 0.0f); // object's center
    float view_radius = sqrt(3.0);
    mat3 rot = mat4::rotate_y(m_viewParameters.y_angle) *
               mat4::rotate_x(m_viewParameters.x_angle);
    vec3 eye = rot * vec3(0, 0, m_viewParameters.dist_factor * view_radius) + vec3(0.0f, 1.0f, 0.0f);
    vec3 up =  rot * vec3(0, 1, 0);
    return mat4::look_at(eye, center, up);
}

void ShadowViewer::paint() {
    mat4 view_matrix = m_constructSceneViewMatrix();
    mat4 projection_matrix = mat4::perspective(m_viewParameters.fovy,
                                               float(m_width) / float(m_height),
                                               m_viewParameters.near,
                                               m_viewParameters.far);

    // For debugging, we can render the scene from the viewpoint of the last light,
    // as computed for rendering the requested shadow cube map face.
    if ((m_numActiveLights > 0) && (m_viewLightCubeFace < NONE)) {
        view_matrix       = m_constructLightViewMatrix(m_selectedLight, m_viewLightCubeFace);
        projection_matrix = m_constructLightProjectionMatrix();
    }

    draw(view_matrix, projection_matrix);
    glCheckError();
}
