#include "ChessBoard.hpp"

ChessBoard::ChessBoard() {
    for (int i = 9; i <= 16; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, ChessPiece::WHITE)));
    }
    for (int i = 49; i <= 56; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, ChessPiece::BLACK)));
    }

    board.insert(pair<int,unique_ptr<ChessPiece>>(1, make_unique<Rook>(1, 5, ChessPiece::WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(2, make_unique<Knight>(2, 3, ChessPiece::WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(3, make_unique<Bishop>(3, 3, ChessPiece::WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(4, make_unique<Queen>(4, 9, ChessPiece::WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(5, make_unique<King>(5, 0, ChessPiece::WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(6, make_unique<Rook>(6, 5, ChessPiece::WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(7, make_unique<Knight>(7, 3, ChessPiece::WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(8, make_unique<Bishop>(8, 3, ChessPiece::WHITE)));

    board.insert(pair<int,unique_ptr<ChessPiece>>(57, make_unique<Rook>(57, 5, ChessPiece::BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(58, make_unique<Knight>(58, 3, ChessPiece::BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(59, make_unique<Bishop>(59, 3, ChessPiece::BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(60, make_unique<Queen>(60, 9, ChessPiece::BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(61, make_unique<King>(61, 0, ChessPiece::BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(62, make_unique<Rook>(62, 5, ChessPiece::BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(63, make_unique<Knight>(63, 3, ChessPiece::BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(64, make_unique<Bishop>(64, 3, ChessPiece::BLACK)));

    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            string position;
            position += (j - 1) + 'a';
            position += i + '0';
            // cout << position << " = " << ((i - 1) * 8) + j << endl;
            grid.insert(pair<string,int>(position, ((i - 1) * 8) + j));
        }
    }
}

void ChessBoard::printPieces() {
    // cout << "board size = " << board.size() << endl;
    for (auto& it : board) {
        it.second->printPiece() << " position = " << it.second->position << ", value = " << it.second->value 
       << ", color = " << (it.second->color ? "Black" : "White") << endl;
    }
}