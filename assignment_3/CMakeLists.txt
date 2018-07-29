project(RayTracing)
cmake_minimum_required(VERSION 2.6.4)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# set output paths
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# default to Release builds
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# try to find OpenMP for multicore parallelization
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    if(MSVC)
      add_definitions(/arch:SSE)
    endif(MSVC)
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} -fopenmp)
endif()

# try to find TBB for parallelization
find_package(TBB)
if (TBB_FOUND)
    add_definitions(-DHAS_TBB)
    include_directories(${TBB_INCLUDE_DIRS})
    link_libraries(${TBB_LIBRARIES})
endif()

# compiler flags
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES -DNOMINMAX /openmp")
endif()


add_subdirectory(src)

# documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
	# prepare doxygen configuration file
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	# add doc as target
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
endif(DOXYGEN_FOUND)
