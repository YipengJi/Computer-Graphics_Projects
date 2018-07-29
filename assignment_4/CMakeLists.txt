project(SolarSystem)

cmake_minimum_required(VERSION 2.8.11)

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

# compiler flags
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

# add source directory to include path
include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/lib/glew)
include_directories(${CMAKE_SOURCE_DIR}/lib/glfw/include/)
include_directories(${CMAKE_SOURCE_DIR}/lib/lodePNG/)


# build source directory
add_subdirectory(lib/glew)
add_subdirectory(lib/glfw)
add_subdirectory(lib/lodePNG)
add_subdirectory(src)

# documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
	# prepare doxygen configuration file
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	# add doc as target
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
endif(DOXYGEN_FOUND)
