#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include "ChessPiece.hpp"

class ChessBoard {
public:
    ChessBoard();
    void printPieces();
    // check for check and check mate
private:
    map<int,unique_ptr<ChessPiece>> board;
    map<string,int> grid;
    vector<string> moves;
};

#endif