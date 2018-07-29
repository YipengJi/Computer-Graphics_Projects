
# OpenGL & GLEW library
find_package(OpenGL)
ADD_DEFINITIONS(-DGLEW_STATIC)
include_directories(${CMAKE_SOURCE_DIR}/lib/glew)

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
target_link_libraries(SolarSystem glew glfw lodePNG ${OPENGL_LIBRARIES})
