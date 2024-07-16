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
    // map<int,unique_ptr<ChessPiece>> getBoard() {return board;};
    // bool isPieceOnGrid(string move, Color color);
    // check for check and check mate
private:
    PieceIterator findPiece(string move, Color color);
    void checkEnPassant(PieceIterator, int);
    // bool moveCausesCheck(string move, Color color);

    bool game_over;
    // string currEnPassant;
    // int enPassantCounter;
    map<int,unique_ptr<ChessPiece>> board;
    map<string,int> grid;
    // vector<string> moves;
};
#endif