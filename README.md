# Game of Life
Simple graphical implementation of [Conway's game of life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
using C++ and SDL2.

Can single step, play and resize the simulation at will.

# Building
Requires a C++ toolchain, cmake, SDL2 and SDL2_TTF installed on the system
```sh
$ cmake -S . -B build/
$ cmake --build build/
$ ./build/gol
```
To produce a debug or release build, add `-D CMAKE_BUILD_TYPE=Debug` to the 
cmake configuration.

## Ubuntu
```sh
$ sudo apt-get install cmake libsdl2-dev libsdl2-ttf-dev
```

## MSYS2
You can build for windows using the MSYS2 environment
```sh
$ pacman -S cmake mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
```

# Code structure
The main Game of Life logic lives in `src/board.h|cpp`. It uses a bit vector to
store board state, and call to `Board::update` advances the simulation

Most classes are children of `Widget`, which represents a drawable UI element
that can react to OS events.

* `Widget` - Base class
  * `Text` - A static text label
    * `Button` - A button that executes a function on a click
  * `Board` - View into a game-of-life simulation
