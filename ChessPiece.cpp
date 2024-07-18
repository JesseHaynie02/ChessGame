#include "ChessPiece.hpp"

int ChessPiece::enPassant = 0;

set<int> Pawn::getMoves(map<int,unique_ptr<ChessPiece>> &board)
{
    // **************** Still need to implement updating en passant every move ****************

    cout << "Pawn in getMoves at position: " << this->position << " ";
    // set<int> possibleMoves = {1,2,3,4,5,6,7,9,10};
    int turn;
    (color == WHITE) ? turn = 1 : turn = -1;
    set<int> possibleMoves;
    map<int,unique_ptr<ChessPiece>>::iterator selectedPiece = board.end();
    // cout << "Pawn position is at: " << position << endl;
    // pawn push
    selectedPiece = board.find(position + (8 * turn));
    if (selectedPiece == board.end()) {
        possibleMoves.insert(position + (8 * turn));
        selectedPiece = board.find(position + (16 * turn));
        bool pawnsFirstMove = ((position >= 9 && position <= 16 && color == WHITE) || 
                               (position >= 49 && position <= 56 && color == BLACK));
        if (selectedPiece == board.end() && pawnsFirstMove) {
            // cout << "Found two move pawn at square: " << position << endl;
            possibleMoves.insert(position + (16 * turn));
            // cout << "Inserted move: " << position + (16 * turn) << endl;
            // this->setEnPassant(position + (16 * turn));
        }
    }

    // Check diagonal captures and en passant
    if (position % 8 != 1) { // Not on column a
        if (this->canPawnTakeDiag(board, RIGHT) || this->canPawnTakeEnPassant(board, RIGHT)) {
            possibleMoves.insert(position + (9 * turn));
        }
    }
    if (position % 8 != 0) { // Not on column h
        if (this->canPawnTakeDiag(board, LEFT) || this->canPawnTakeEnPassant(board, LEFT)) {
            possibleMoves.insert(position + (7 * turn));
        }
    }

    return possibleMoves;
}

bool Pawn::canPawnTakeDiag(map<int,unique_ptr<ChessPiece>> &board, Direction diagonal) {
    int move, turn;
    (diagonal == LEFT) ? move = 7 : move = 9;
    (color == WHITE) ? turn = 1 : turn = -1;
    map<int,unique_ptr<ChessPiece>>::iterator selectedPiece = board.end();

    selectedPiece = board.find(position + (move * turn));
    if (selectedPiece != board.end() && selectedPiece->second->getColor() != color && 
        selectedPiece->second->getPiece() != "King") {
        // cout << "In Pawn canPawnTakeDiag: " << this->position << endl;
        return true;
    }

    return false;
}

bool Pawn::canPawnTakeEnPassant(map<int,unique_ptr<ChessPiece>> &board, Direction side) {
    int move, turn;
    (side == RIGHT) ? move = 1 : move = -1;
    (color == WHITE) ? turn = 1 : turn = -1;
    map<int,unique_ptr<ChessPiece>>::iterator selectedPiece = board.end();

    selectedPiece = board.find(position + (move * turn));
    cout << "In canPawnTakeEnPassant looking at square: " << position + (move * turn) << endl;
    cout << selectedPiece->second->getEnPassant() << " and " << (position + (move * turn)) << endl;
    // selectedPiece->second->getEnPassant() == (position + (move * turn))
    if (selectedPiece != board.end() && selectedPiece->second->getColor() != color && 
        selectedPiece->second->getPiece() == "Pawn" && selectedPiece->second->getEnPassant() == (position + (move * turn))) {
        cout << "In Pawn canPawnTakeEnPassant: " << this->position << endl;
        return true;
    }

    return false;
}

set<int> Rook::getMoves(map<int,unique_ptr<ChessPiece>> &board)
{
    cout << "in rook getMoves" << endl;
    set<int> possibleMoves, temp;
    // move until a piece has been found (either color) if opposite take if not nothing or until out of bounds
    // check up
    temp = checkSquareRecursive(board, position, 8);
    possibleMoves = temp;
    temp.clear();
    // check down
    temp = checkSquareRecursive(board, position, -8);
    possibleMoves.insert(temp.begin(), temp.end());
    temp.clear();
    // // check left
    temp = checkSquareRecursive(board, position, 1);
    possibleMoves.insert(temp.begin(), temp.end());
    temp.clear();
    // // check right
    temp = checkSquareRecursive(board, position, -1);
    possibleMoves.insert(temp.begin(), temp.end());

    for (auto &moves : possibleMoves) {
        cout << moves << " ";
    }
    cout << endl;

    return possibleMoves;
}

set<int> Rook::checkSquareRecursive(map<int,unique_ptr<ChessPiece>> &board, int currentPosition, int moveIncrem) {
    set<int> possibleMoves, nextPossibleMove;
    int currentPos = currentPosition + moveIncrem;
    map<int,unique_ptr<ChessPiece>>::iterator nextSquarePiece = board.find(currentPos);
    cout << "current position = " << currentPos << endl;
    bool stayOnRow = abs(moveIncrem) == 1 && (currentPosition % 8 == 1 || currentPosition % 8 == 0);
    bool inBounds = currentPos < 1 || currentPos > 64 || (nextSquarePiece != board.end() && nextSquarePiece->second->getColor() == color);
    if (stayOnRow || inBounds) {
        return possibleMoves;
    }

    // cout << "in recursive before checking if sqaure is empty or not" << endl;
    if (nextSquarePiece != board.end() && nextSquarePiece->second->getColor() != color) {
        possibleMoves.insert(currentPos);
        return possibleMoves;
    } else {
        possibleMoves.insert(currentPos);
    }
    nextPossibleMove = checkSquareRecursive(board, currentPos, moveIncrem);
    possibleMoves.insert(nextPossibleMove.begin(), nextPossibleMove.end());
    return possibleMoves;
}

set<int> Knight::getMoves(map<int,unique_ptr<ChessPiece>> &board)
{
    return set<int>();
}

set<int> Bishop::getMoves(map<int,unique_ptr<ChessPiece>> &board)
{
    return set<int>();
}

set<int> Queen::getMoves(map<int,unique_ptr<ChessPiece>> &board)
{
    return set<int>();
}

set<int> King::getMoves(map<int,unique_ptr<ChessPiece>> &board)
{
    return set<int>();
}