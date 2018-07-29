cmake_minimum_required(VERSION 2.8)
project(exlsystems)
set (CMAKE_CXX_STANDARD 14)

add_executable(lsys
	src/lsystem.cpp
	src/lsystem_load.cpp
	src/main.cpp
	src/utils/misc.cpp
	src/utils/vec.cpp
	src/utils/render_2d.cpp
	src/utils/render_svg.cpp
	src/utils/LSystemViewer.cpp
	src/utils/glfw_window.cpp
	src/utils/glmath.cpp
	src/utils/shader.cpp
	lib/Jzon/Jzon.cpp
)

# Fix build on Windows
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

if(WIN32)
	set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/src/ ${CMAKE_SOURCE_DIR}/lib/win7)
endif()

# attempt to find the system's GLEW; build the included one if unsuccessful
find_package(GLEW QUIET)
if (GLEW_FOUND)
	MESSAGE("Using system GLEW; GLEW_LIBRARIES = ${GLEW_LIBRARIES}")
	set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}/GL) # At least on Ubuntu, system glew.h is actually in GL/
	link_libraries(${GLEW_LIBRARIES})
else()
	MESSAGE("System GLEW not found... falling back to local GLEW")
	add_subdirectory(lib/glew)
	set(GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/lib/glew)
	set(GLEW_LIBRARIES glew)
endif()

# Attempt to find the system's GLFW; build the included one if unsuccessful
find_package(glfw3 QUIET)
if (glfw3_FOUND)
	MESSAGE("Using system GLFW")
	# Note: target_link_libraries(glfw) performed in src/CMakeLists.txt should
	# actually bring in the necessary header files.
else()
	MESSAGE("System GLFW not found... falling back to local GLFW")
	include_directories(${CMAKE_SOURCE_DIR}/lib/glfw/include/)
	add_subdirectory(lib/glfw)
endif()

# OpenGL
find_package(OpenGL)
target_include_directories(lsys SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS})
target_compile_definitions(lsys PUBLIC -DGLEW_STATIC)
target_link_libraries(lsys PUBLIC lodePNG glfw ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

# JSON
include_directories(${CMAKE_SOURCE_DIR}/lib/Jzon)

# PNG
add_subdirectory(lib/lodePNG)
include_directories(${CMAKE_SOURCE_DIR}/lib/lodePNG)


################################################################################
# OpenGL paths to shaders
################################################################################

set(SHADER_PATH	 ${CMAKE_SOURCE_DIR}/shaders			CACHE PATH "location of shader programs")
add_definitions("-DSHADER_PATH=\"${SHADER_PATH}\"")
