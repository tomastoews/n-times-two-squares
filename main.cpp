#include <iostream>
#include <vector>
#include <cmath>
#include <SDL.h>

const int width = 1200;
const int height = 800;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

void initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    // Uint32 windowFlags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_CreateWindowAndRenderer(width, height, /*windowFlags*/ 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawSquares() {
    // The size of all squares has to be dividable by 8 resulting in an integer number 
    // for pixels to be drawn propertly and therefore all main columns to have equal height.
    // This is why the initial square size is 8 * 20 = 160 (160 / 8 = 20).
    int squareSize = 160;
    int mainColumnsCount = 6;
    int rowsCount = 4;
    int baseX = (width - (squareSize*mainColumnsCount)) / 2;
    int baseY = (height - (squareSize*rowsCount)) / 2;

    SDL_SetRenderDrawColor(renderer, 32, 150, 32, 255);

    // Count of squares on the Y axis on a main column
    int yCount = 4;
    // Count of squares on the X axis within a inner column (also number of inner columns)
    int xCount = 1;

    int currentSquareSize = squareSize;

    // The drawing algorithm:

    // Loop 1: Controls drawing of main columns
    // Loop 2: Controls drawing of inner columns within the current main column
    // Loop 3: Controls drawing of squares within the current inner column 

    for (int column = 0; column < mainColumnsCount; column++) {
        for (int innerColumn = 0; innerColumn < xCount; innerColumn++) {
            for (int row = 0; row < yCount; row++) {
                SDL_Rect rect;
                rect.x = baseX + (column*squareSize) + (innerColumn*currentSquareSize);
                rect.y = baseY + (row*currentSquareSize) + 4;
                // Add 1 pixel to avoid double borders
                rect.w = currentSquareSize + 1;
                rect.h = currentSquareSize + 1;
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
        xCount = xCount * 2;
        yCount = yCount * 2;
        currentSquareSize = squareSize*4 / yCount;
    }
    
    SDL_RenderPresent(renderer);
}

int main() {
    initialize();

    SDL_Event event;
    while(event.type != SDL_QUIT) {
        SDL_Delay(16);
        drawSquares();
        SDL_PollEvent(&event);
    }
    
    cleanup();
}