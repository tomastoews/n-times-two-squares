#!/bin/bash

rm -rf build/
mkdir build/

pkg-config --libs SDL2_ttf

g++ -c main.cpp -o build/main.o -I/usr/include/SDL2 # Here is also SDL_ttf.h installed
g++ build/main.o -o build/main -L/usr/lib -lSDL2main -lSDL2 -lSDL2_ttf

if [ $? -eq 0 ];
then
    echo ""
    echo "1: Normal execution (default)"
    echo "2: Execute with heaptrack"

    read option

    case $option in
        "1")
            ./build/main
            ;;
        "2")
            heaptrack ./build/main
            ;;
        *)
            ./build/main
            ;;
    esac
fi
