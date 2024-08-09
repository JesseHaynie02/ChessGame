#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <SDL_main.h>
#include <SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

using namespace std;

enum ColorOfPiece {WHITEP,BLACKP};

struct pieceIMG {
    const char *file;
    SDL_Texture *tex;
    SDL_Rect rect;
    ColorOfPiece color;
};

struct boardSquare {
    int r,g,b;
    SDL_Rect rect;
};



#endif