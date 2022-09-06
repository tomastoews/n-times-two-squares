#!/bin/bash

# Install SDL2 via a package manager
# The SDL2 C++ libraries are installed into /usr/include/SDL2
# and included from there.

sudo pacman -Suy sdl2 sdl2_ttf sdl2_image libdecor audiofile heaptrack

# or:

# C++ package manager:
# https://vcpkg.io/en/getting-started.html

vcpkg install sdl2 sdl2-ttf sdl2-image
