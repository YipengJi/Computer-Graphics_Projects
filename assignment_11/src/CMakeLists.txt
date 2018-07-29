# OpenGL & GLEW library
find_package(OpenGL)
ADD_DEFINITIONS(-DGLEW_STATIC)

# source files
file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)
file(GLOB SHADERS ./*.vert ./*.frag)

# Make sure the textures and shaders are available
set(TEXTURE_PATH ${CMAKE_SOURCE_DIR}/textures CACHE PATH "location of texture images")
set(SHADER_PATH  ${CMAKE_SOURCE_DIR}/src      CACHE PATH "location of shader programs")
add_definitions("-DTEXTURE_PATH=\"${TEXTURE_PATH}\"")
add_definitions("-DSHADER_PATH=\"${SHADER_PATH}\"")

# executable
add_executable(SolarSystem ${HEADERS} ${SOURCES})
target_include_directories(SolarSystem SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS})
# Note: target_link_libraries(glfw) should actually bring in the necessary header files.
target_link_libraries(SolarSystem lodePNG glfw ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
