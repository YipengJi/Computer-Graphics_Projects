Solar System
============
This directory contains the framework code that will be used in the four OpenGL exercises.

Building under Linux/macOS
--------------------------
Inside the exercise's top-level directory, execute the following commands:

    mkdir build
    cd build
    cmake ..
    make

The last command -- i.e. `make` -- compiles the application. Rerun it whenever you have added/changed code in order to recompile.

Building under Windows Visual Studio
------------------------------------
  * Install Visual Studio Community 2013 or later
  * You will be asked, if you want to install additional packages. Make sure that you check the c++ development option.
  * Inside the exercise's top-level directory create a new `build` folder (CTRL + SHIFT + N)
  * Install [CMake] (https://cmake.org/download/)
  * Start the cmake-gui.exe
  * Click `Browse Source` and select the exercise's top-level directory
  * Click `Browse Build` and select the created `build` folder
  * Click `Configure` and select your Visual Studio version
  * Start Configuring. 
  * If no major errors occur, click `Generate`
  * Start Visual Studio
  * Use `Open Project` to load your `SolarSystem.sln`
  * On the right, there should be the solution explorer. Find the project `SolarViewer`, right click and choose `Set as StartUp Project`
  * Press CTRL + F5 to compile and run

Documentation
-------------
You may build an HTML documentation as long as you have [Doxygen](www.doxygen.org/) installed. To do so, still inside the directory `build`, execute the following command:

    make doc

View the documentation by opening the file `html/index.html` with any web browser / HTML viewer. If you are into LaTeX, navigate into the directory `latex` and execute the command `make` to create a printable version of the documentation.

Textures and Copyright
----------------------
All earth textures are from the [NASA Earth Observatory](http://earthobservatory.nasa.gov/Features/BlueMarble/) and have been modified by Prof. Hartmut Schirmacher, Beuth Hochschule für Technik Berlin. The sun texture is from http://www.solarsystemscope.com/textures. All other textures are from http://textures.forrest.cz/index.php?spgmGal=maps&spgmPic=14. The ship model if from https://free3d.com.

Keyboard Settings
-----------------
  * arrow keys: Navigation Camera
  * W,A,S,D:	Navigation Ship
  * g:		toggle greyscale
  * +/-:	increase/decrease time_step
  * y/z:	switch mono/stereo view mode
  * 1-6:	set camera to planets/sun
  * 7:		set camera to ship
  * 8/9:	change camera's distance to the observed object
  * space:	pause 
  * r:		randomize planets' positions
  * escape:	exit viewer

Assignment 5: Transformations and Viewing
-----------------------------------------
In this assignment, you will place the planets, moon, and space ship in the
Solar System scene and set up the view. All of your implementation will be done
in `src/solar_viewer.cpp`. For detailed instructions, please see the assignment
5 handout.
