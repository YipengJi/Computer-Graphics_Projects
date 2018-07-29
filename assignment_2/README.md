Ray Tracer
==========

We are using a couple of C++11 features, so please ensure that you use an up-to-date compiler (GCC 4.7 or higher, Visual Studio 2012). [CMake](www.cmake.org) is used for setting up build environments.


Building under Linux/macOS
--------------------------

Inside the exercise's top-level directory, execute the following commands:

    mkdir build
    cd build
    cmake ..
    make

The last command -- i.e. `make` -- compiles the application. Rerun it whenever you have added/changed code in order to recompile.

To build a pretty documentation use:

    make doc

and open the `index.html` in the html folder with your favourite browser. To build the documentation, you must install Doxygen.

Building with XCode (macOS)
---------------------------

If you wish, you can use the CMake build system to generate an XCode project.
Inside the exercise's top-level directory, execute the following commands:

    mkdir xcode
    cd xcode
    cmake -G Xcode ..
    open RayTracing.xcodeproj


Building under Microsoft Windows (Visual Studio)
------------------------------------------------

* Start the CMake-GUI.
* Open the RayTracing top-level directory as source directory.
* Choose a build directory.
* Click on configure and select Visual Studio as generator.
* Click generate to create the Visual Studio project files.
* Open the Visual Studio solution file that is in the build directory you chose in CMake.


Running the Ray Tracer
----------------------

The program expects two command line arguments: an input scene (`*.sce`) and an output image (`*.tga`). To render the scene with the three spheres, while inside the `build` directory, type:

    ./raytrace ../scenes/spheres/spheres.sce output.tga

If you have finished all exercise tasks, use

    ./raytrace 0

to render all scenes at once.

To set the command line parameters in MSVC or Xcode, please refer to the documentation of these programs (or use the command line...).


Assignment 2: Phong Lighting
----------------------------
In this assignment you will implement the Phong Lighting Model, shadows and reflections. Fill in the missing code in the file Scene.cpp. Follow the instructions in TODO comments and assignment sheet.
