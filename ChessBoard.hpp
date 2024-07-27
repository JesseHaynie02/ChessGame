#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "ChessPiece.hpp"

using PieceIterator = map<int,unique_ptr<ChessPiece>>::iterator;

class ChessBoard {
public:
    ChessBoard();
    void printPieces();
    bool getGameOver() {return gameOver;}
    bool getDraw() {return draw;}
    bool movePiece(string move, Color color);
private:
    PieceIterator findPiece(string move, Color color);
    void checkCastling(string, string, int, int, Color);
    bool clearToCastle(string move, Color color);
    bool castle(string side, int kingLoc, int rookLoc, Color color);
    void checkEnPassant(int originalLocOfPiece, int squareToMoveTo, Color color);
    void moveDo(int originalLocOfPiece, int newLocOfPiece, string pieceMoved, bool isPieceTake, int &pieceTakenLoc, string &pieceTaken, Color color);
    PieceIterator moveUndo(int originalLocOfPiece, int newLocOfPiece, string pieceMoved, int pieceTakenLoc, string pieceTaken, Color color);
    bool isPawnPromotion(string pieceType, string move, Color color);
    bool isDraw(Color color);
    bool inCheck(Color color);
    bool inCheckMate(Color color);
    string serializeBoard();
    size_t hashBoard(string gameState);
    bool isThreeFold(size_t currentGameState);
    bool inStalemate(Color color);
    bool ischeckMateImpossible();

    bool whiteShortCastle, whiteLongCastle, blackShortCastle, blackLongCastle;
    bool gameOver;
    bool draw;
    map<int,unique_ptr<ChessPiece>> board;
    map<string,int> grid;
    unordered_map<size_t,int> boardHistory;
};
#endif