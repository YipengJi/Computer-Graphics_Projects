# OpenGL & GLEW library
find_package(OpenGL)
ADD_DEFINITIONS(-DGLEW_STATIC)

# source files
file(GLOB SOURCES ./*.cpp)
set(SOURCES glfw_window.cpp glmath.cpp main.cpp shader.cpp ShadowViewer.cpp Mesh.cpp sphere.cpp)
set(HEADERS glfw_window.h gl.h glmath.h shader.h ShadowViewer.h Mesh.h sphere.h Quad.h CubeMapVisualization.h dumpOpenGLBuffer.h)

# Make sure the textures and shaders are available
set(SHADER_PATH   ${CMAKE_SOURCE_DIR}/shaders      CACHE PATH "location of shader programs")
set(TEXTURE_PATH  ${CMAKE_SOURCE_DIR}/textures     CACHE PATH "location of texture images")
set(MESH_PATH     ${CMAKE_SOURCE_DIR}/meshes       CACHE PATH "location of example meshes")
add_definitions("-DSHADER_PATH=\"${SHADER_PATH}\"")
add_definitions("-DMESH_PATH=\"${MESH_PATH}\"")
add_definitions("-DTEXTURE_PATH=\"${TEXTURE_PATH}\"")

# executable
add_executable(ShadowViewer ${HEADERS} ${SOURCES})
target_include_directories(ShadowViewer SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS})
# Note: target_link_libraries(glfw) should actually bring in the necessary header files.
target_link_libraries(ShadowViewer lodePNG glfw ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
