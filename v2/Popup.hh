#include "config.hh"

class Popup {
    
    public:
        Popup(SDL_Renderer* renderer, SDL_Rect* square, int boxNumber) {
            this->renderer = renderer;
            this->square = square;
            this->boxNumber = boxNumber;
            calculatePosition();
            createTitle();
        }

        ~Popup() {
            SDL_FreeSurface(titleSurface);
            SDL_DestroyTexture(titleTexture);
        }

        void draw() {
            drawBackground();
            drawFrame();
            drawTitle();
        }

    private:
        int x;
        int y;
        int boxNumber;
        int width = 150;
        int height = 200;
        SDL_Renderer* renderer;
        SDL_Rect* square;

        SDL_Surface* titleSurface;
        SDL_Texture* titleTexture;
        SDL_Rect titleRect;
        SDL_Rect backgroundRect;
        SDL_Rect frameRect;
        SDL_Color white = {255, 255, 255};
        
        void calculatePosition() {
            int offset = 20;
            int x, y;
            
            auto isXRightOffset = [](Popup *popup, int offset) { return (popup->square->x+popup->square->w+popup->width+offset) > windowWidth; };
            auto isXLeftOffset = [](Popup *popup, int offset) { return (popup->square->x-(popup->width+offset)) < 0; };
            auto isYBottomOffset = [](Popup *popup, int offset) { return (popup->square->y+popup->square->h+popup->height/2) > windowHeight; };
            auto isYTopOffset = [](Popup *popup, int offset) { return (popup->square->y-(popup->height/2)) < 0; };
            
            // Check upper right corner
            if (isXRightOffset(this, offset) && isYTopOffset(this, offset)) {
                x = this->square->x - width - offset;
                y = this->square->y + offset;
            }
            
            // Check lower right corner
            else if (isXRightOffset(this, offset) && isYBottomOffset(this, offset)) {
                x = this->square->x - width - offset;
                y = this->square->y - height - offset;
            }
            
            // Check upper left corner
            else if (isXLeftOffset(this, offset) && isYTopOffset(this, offset)) {
                x = this->square->x + width + offset;
                y = this->square->y + offset;
            } 
            
            // Check lower left corner
            else if (isXLeftOffset(this, offset) && isYBottomOffset(this, offset)) {
                x = this->square->x + width + offset;
                y = this->square->y - height - offset;
            }

            // Check top
            else if (isYTopOffset(this, offset) && !isXRightOffset(this, offset) && !isXLeftOffset(this, offset)) {
                x = this->square->x - (width/2) + (this->square->w/2);
                y = this->square->y + this->square->h + offset;
            }

            // Check bottom
            else if (isYBottomOffset(this, offset) && !isXRightOffset(this, offset) && !isXLeftOffset(this, offset)) {
                x = this->square->x - (width/2) + (this->square->w/2);
                y = this->square->y - (height + offset);
            }

            // Check right
            else if (isXRightOffset(this, offset) && !isYTopOffset(this, offset) && !isYBottomOffset(this, offset)) {
                x = this->square->x - (width + offset);
                y = this->square->y - (height/2) + (this->square->h/2);
            }

            // For (Check left case) and else:
            else {
                x = this->square->x + this->square->w + offset;
                y = this->square->y - (height/2) + (this->square->h/2);
            }

            this->x = x;
            this->y = y;
        }

        void drawBackground() {
            backgroundRect.x = x;
            backgroundRect.y = y;
            backgroundRect.w = width;
            backgroundRect.h = height;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_Surface *rgbSurface = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);
            SDL_FillRect(rgbSurface, &backgroundRect, SDL_MapRGB(rgbSurface->format, 0, 0, 0));
            SDL_RenderFillRect(renderer, &backgroundRect);
            SDL_FreeSurface(rgbSurface);
        }

        void drawFrame() {
            frameRect.x = x;
            frameRect.y = y;
            frameRect.w = width;
            frameRect.h = height;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &frameRect);
        }

        void createTitle() {
            int textOffset = 10;
            TTF_Font* font = TTF_OpenFont("8bit.ttf", 16);
            std::string text =  "Box: " + std::to_string(boxNumber);
            titleSurface = TTF_RenderText_Solid(font, text.c_str(), white);
            titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            titleRect.x = x+textOffset;
            titleRect.y = y;
            titleRect.w = width-(textOffset*2);
            titleRect.h = 40;
            TTF_CloseFont(font);
        }

        void drawTitle() {
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        }

};
