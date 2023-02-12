#include <vector>
#include <cmath>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "config.hh"
#include "Popup.hh"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
Popup popup;
std::vector<SDL_Rect> squares;
bool showPopup = false;

void initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    // Uint32 windowFlags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_CreateWindowAndRenderer(config::windowWidth, config::windowHeight, /*windowFlags*/ 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    squares.clear();
    squares.shrink_to_fit();
}

bool isSquareFocused(SDL_Rect* square) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    SDL_SetRenderDrawColor(renderer, config::green.r, config::green.g, config::green.b, config::green.a);
    if ((x > square->x && x < square->x+square->w) && (y > square->y && y < square->y+square->h)) {
        return true;
    } else {
        return false;
    }
}

void drawSquares(SDL_Event *event) {
    SDL_SetRenderDrawColor(renderer, config::green.r, config::green.g, config::green.b, config::green.a);
    showPopup = false;
    for (int i = 0; i < squares.size(); i++) {
        if (isSquareFocused(&squares[i])) {
            SDL_RenderFillRect(renderer, &squares[i]);
            // The popup object should only be updated once for a focused square.
            if (popup.getSquareNumber() != i) {
                popup.update(&squares[i], i);
            }
            showPopup = true;
        } else {
            SDL_RenderDrawRect(renderer, &squares[i]);
        }
    }
}

void createSquares() {
    // The size of all squares has to be dividable by 8 resulting in an integer number 
    // for pixels to be drawn propertly and therefore all main columns to have equal height.
    // This is why the initial square size is 8 * 20 = 160 (160 / 8 = 20).
    int squareSize = 160;
    int mainColumnsCount = 6;
    int rowsCount = 4;
    int baseX = (config::windowWidth - (squareSize*mainColumnsCount)) / 2;
    int baseY = (config::windowHeight - (squareSize*rowsCount)) / 2;

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
                squares.push_back(rect);
            }
        }
        xCount = xCount * 2;
        yCount = yCount * 2;
        currentSquareSize = squareSize*4 / yCount;
    }
}

int main() {
    initialize();
    createSquares();
    popup = Popup(renderer);

    SDL_Event event;
    while(event.type != SDL_QUIT) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        drawSquares(&event);
        if (showPopup) popup.draw();
        SDL_RenderPresent(renderer);
        SDL_PollEvent(&event);
    }
    
    cleanup();
}
