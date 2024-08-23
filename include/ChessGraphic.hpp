#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <SDL_main.h>
#include <SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "ChessPiece.hpp"
#include "ChessGame.hpp"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define SQUARE_WIDTH (SCREEN_WIDTH / 8)
#define SQUARE_HEIGHT (SCREEN_HEIGHT / 8)

using namespace std;

struct pieceIMG {
    const char *file;
    SDL_Texture *tex;
    Color color;
    string pieceName;
    SDL_Rect rect;

    pieceIMG() : file(nullptr), tex(nullptr), color(WHITE), pieceName(""), rect{0, 0, 0, 0} {}

    pieceIMG(const char *file, SDL_Texture *tex, Color color, const string &pieceName, const SDL_Rect &rect)
        : file(file), tex(tex), color(color), pieceName(pieceName), rect(rect) {}
};

struct boardSquare {
    int r,g,b;
    int position; // do i even need?
    SDL_Rect rect;
    // pieceIMG piece; // do i even need?
};

struct gameState {
    Color turn = WHITE;
    bool quit = false, dragging = false;
    SDL_Event event;
    SDL_Point mousePos;
    SDL_Rect origLocRect = {0,0,80,80};
    int selPieceInd = -1;
    set<int> moves;
    int origLoc = 0, newLoc = 0, pieceTakenLoc = 0;
    bool isPieceTake = false;
    string pieceMoved = "", pieceTaken = "";
};

void initializeSDL();
void initWinRen(SDL_Window *&window, SDL_Renderer *&renderer);
void initializeGraphic(SDL_Renderer *renderer, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs);
void refreshGame(SDL_Renderer *renderer, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs);
void destroyGUI(SDL_Window *&window, SDL_Renderer *&renderer, map<int,unique_ptr<pieceIMG>> &pieceIMGs);
void resetState(gameState &state);
void mouseButtonDown(SDL_Renderer *&renderer, ChessBoard &board, gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs);
void mouseMotion(gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs);
void mouseButtonUp(SDL_Renderer *&renderer, ChessBoard &board, gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs);
bool castle(SDL_Renderer *&renderer, ChessBoard &board, gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs);
string graphicPawnPromotion(SDL_Renderer *renderer, gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs, SDL_Rect row);

#endif