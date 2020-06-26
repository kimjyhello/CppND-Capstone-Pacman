# CPPND: Capstone Modified Pacman Game 

This is the repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). This uses the [Snake game](https://github.com/udacity/CppND-Capstone-Snake-Game) codes provided in the Nanodegree course. 

<!--img src="snake_game.gif"/-->

This project incorporates the concepts learned in the program.
The following are the specifications from the course material that this program touches on (also marked in the codes as comments where applicable):
* C1: The project reads data from a file and process the data: The project reads from an external file
* C2: The project accepts user input and processes the input: The project accepts input from a user as part of the necessary operation of the program.
* C3: The project uses Object Oriented Programming techniques: The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
* C4: Classes use appropriate access specifiers for class members: All class data members are explicitly specified as public, protected, or private.
* C5: Classes encapsulate behavior: Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.
* C6: Overloaded functions allow the same function to operate on different parameters: One function is overloaded with different signatures for the same function name.


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* SDL2_Image
  * For installation refer to [this page](https://www.libsdl.org/projects/SDL_image/)
  * On Max OS X, Homebrew can be used to install sdl2_image 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Pacman`.