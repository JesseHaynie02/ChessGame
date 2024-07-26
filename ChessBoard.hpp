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
    bool getGameOver() {return game_over;}
    void setGameOver(bool status) {game_over = status;}
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
    bool inCheck(Color color);
    bool inCheckMate(Color color);

    bool whiteShortCastle, whiteLongCastle, blackShortCastle, blackLongCastle;
    bool game_over;
    map<int,unique_ptr<ChessPiece>> board;
    map<string,int> grid;
    // vector<string> moves;
};
#endif