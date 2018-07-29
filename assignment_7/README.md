Shadows and Cube Mapping
========================
This directory contains the framework code that will be used in the 7th exercise.

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

Textures and Copyright
----------------------
The cube textures are from the Wikipedia article Cube mapping (https://en.wikipedia.org/wiki/Cube_mapping#/media/File:Cube_map.svg).

Keyboard Settings
-----------------
  * arrow keys: Navigation of the eye.
  * W,A,S,D:	Navigation of the selected light.
  * 8/9:	    Change eye's distance to the observed object.
  * 1-3:        Select pre-set scenes.
  * C:          Toggle display of the light as a sphere, cube with demo texture, cube with shadow maps.
  * F:          Toggle between eye view and one light camera looking through the faces 0-6.
  * P:          Write the current view to the screenshot "screenshow_#.png".
  * escape:	    Exit viewer.
