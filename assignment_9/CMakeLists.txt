cmake_minimum_required(VERSION 2.8)
project(icg_noise)
set (CMAKE_CXX_STANDARD 14)

# set output paths
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

if(WIN32)
	MESSAGE("CMAKE_PREFIX_PATH: " $CMAKE_PREFIX_PATH)
	set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/src/ ${CMAKE_SOURCE_DIR}/lib/win7)
endif()

# default to Release builds
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()


add_library(render_shared STATIC
	src/utils/vec.cpp
	src/render/shader.cpp
	src/render/Mesh.cpp
	src/render/StaticMeshActor.cpp
	src/render/glfw_window.cpp
	src/render/MeshViewer.cpp
	src/render/ShaderViewer.cpp
)

add_executable(shvi
	src/main_shader_viewer.cpp
)
target_link_libraries(shvi render_shared)

add_executable(terrain
	src/main_terrain.cpp
)
target_link_libraries(terrain render_shared)

# OpenMP - parallel loops
find_package( OpenMP )
if(OPENMP_FOUND)
	message("OpenMP: found")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# OpenGL libs

# attempt to find the system's GLEW; build the included one if unsuccessful
find_package(GLEW QUIET)
if (GLEW_FOUND)
	MESSAGE("Using system GLEW")
	set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}/GL) # At least on Ubuntu, system glew.h is actually in GL/
	message("GLEW lib " ${GLEW_LIBRARIES})
else()
	MESSAGE("System GLEW not found... falling back to local GLEW")
	add_subdirectory(lib/glew)
    ADD_DEFINITIONS(-DGLEW_STATIC)
	set(GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/lib/glew)
	set(GLEW_LIBRARIES glew)
endif()

find_package(OpenGL)

target_include_directories(render_shared SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(     render_shared        PUBLIC ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

# Attempt to find the system's GLFW; build the included one if unsuccessful
find_package(glfw3 QUIET)
if (glfw3_FOUND)
	MESSAGE("Using system GLFW")
	# Note: target_link_libraries(glfw) performed in src/CMakeLists.txt should
	# actually bring in the necessary header files.
	message("GLFW lib " ${GLFW_LIBRARIES})
	message("GLFW static " ${GLFW_STATIC_LIBRARIES})
else()
	MESSAGE("System GLFW not found... falling back to local GLFW")
	add_subdirectory(lib/glfw)
	set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/glfw/include/")
	set(GLFW_LIBRARIES glfw)
endif()
include_directories(${GLFW_INCLUDE_DIR})
# Note: target_link_libraries(glfw) should actually bring in the necessary header files.
target_link_libraries(render_shared PUBLIC glfw)

add_subdirectory("lib/lodePNG")
target_include_directories(render_shared SYSTEM PUBLIC "lib/lodePNG")
target_link_libraries(render_shared PUBLIC lodePNG)

set(SHADER_PATH ${CMAKE_SOURCE_DIR}/src/shaders CACHE PATH "location of shader programs")
add_definitions("-DSHADER_PATH=\"${SHADER_PATH}\"")
