#include "ChessPiece.hpp"

int ChessPiece::enPassant = 0;

set<int> Pawn::getMoves(map<int,unique_ptr<ChessPiece>> &board) {
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
    // cout << "In canPawnTakeEnPassant looking at square: " << position + (move * turn) << endl;
    cout << selectedPiece->second->getEnPassant() << " and " << (position + (move * turn)) << endl;
    // selectedPiece->second->getEnPassant() == (position + (move * turn))
    if (selectedPiece != board.end() && selectedPiece->second->getColor() != color && 
        selectedPiece->second->getPiece() == "Pawn" && selectedPiece->second->getEnPassant() == (position + (move * turn))) {
        // cout << "In Pawn canPawnTakeEnPassant: " << this->position << endl;
        return true;
    }

    return false;
}

set<int> Rook::getMoves(map<int,unique_ptr<ChessPiece>> &board) {
    cout << "in rook getMoves" << endl;
    set<int> directions = {8, -8, 1, -1};
    return checkDirection(board, directions);
}

set<int> Knight::getMoves(map<int,unique_ptr<ChessPiece>> &board) {
    cout << "in knight getMoves" << endl;
    set<int> possibleMoves, knightMoves = {-17,-15,-10,-6,6,10,15,17};

    if (position % 8 == 7) {
        knightMoves.erase(10);
        knightMoves.erase(-6);
    } else if (position % 8 == 0) {
        knightMoves.erase(17);
        knightMoves.erase(-15);
        knightMoves.erase(10);
        knightMoves.erase(-6);
    } else if (position % 8 == 2) {
        knightMoves.erase(6);
        knightMoves.erase(-10);
    } else if (position % 8 == 1) {
        knightMoves.erase(6);
        knightMoves.erase(-10);
        knightMoves.erase(15);
        knightMoves.erase(-17);
    }

    for (auto &moveInc : knightMoves) {
        int move = moveInc + position;
        if (!(move < 1 || move > 64)) {
            if (board.find(move) == board.end() || (board.find(move) != board.end() && board.find(move)->second->getColor() != color)) {
                if (position % 8 == 7 || position % 8 == 0) {
                    
                }
                possibleMoves.insert(move);
            }
        }
    }

    cout << "Printing moves" << endl;
    for (auto &moves : possibleMoves) {
        cout << moves << " ";
    }
    cout << endl;

    return possibleMoves;
}

set<int> Bishop::getMoves(map<int,unique_ptr<ChessPiece>> &board) {
    cout << "in bishop getMoves" << endl;
    set<int> directions = {7, -7, 9, -9};
    return checkDirection(board, directions);
}

set<int> Queen::getMoves(map<int,unique_ptr<ChessPiece>> &board) {
    cout << "in queen getMoves" << endl;
    set<int> directions = {8, -8, 1, -1, 7, -7, 9, -9};
    return checkDirection(board, directions);
}

set<int> King::getMoves(map<int,unique_ptr<ChessPiece>> &board) {
    return set<int>();
}

set<int> ChessPiece::checkDirection(map<int,unique_ptr<ChessPiece>> &board, set<int> directions) {
    set<int> possibleMoves, temp;
    cout << "directions: ";
    for (auto dirIter = directions.begin(); dirIter != directions.end(); ++dirIter) {
        cout << *dirIter << " " << endl;
        temp = checkSquareRecursive(board, position, *dirIter);
        possibleMoves.insert(temp.begin(), temp.end());
        temp.clear();
    }
    // cout << "\nMoves: ";
    // for (auto &moves : possibleMoves) {
    //     cout << moves << " ";
    // }
    cout << endl;
    return possibleMoves;
}

// Can reduce lines of code used in rook, bishop, and queen getMoves functions
// possibly creating a set of directions and looping through it
set<int> ChessPiece::checkSquareRecursive(map<int,unique_ptr<ChessPiece>> &board, int currentPosition, int moveIncrem) {
    set<int> possibleMoves, nextPossibleMove;
    int currentPos = currentPosition + moveIncrem;
    map<int,unique_ptr<ChessPiece>>::iterator nextSquarePiece = board.find(currentPos);
    bool bishopOnEdge = ((moveIncrem == 7 || moveIncrem == -9) && currentPosition % 8 == 1) || 
                        ((moveIncrem == -7 || moveIncrem == 9) && currentPosition % 8 == 0);
    // bool stayOnRow = abs(moveIncrem) == 1 && (currentPos % 8 == 1 || currentPos % 8 == 0);
    bool stayOnRow = (moveIncrem == -1 && currentPosition % 8 == 1) || (moveIncrem == 1 && currentPosition % 8 == 0);
    bool inBounds = currentPos < 1 || currentPos > 64 || (nextSquarePiece != board.end() && nextSquarePiece->second->getColor() == color);
    if (stayOnRow || inBounds || bishopOnEdge) {
        return possibleMoves;
    }
    cout << "current position = " << currentPos << endl;
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