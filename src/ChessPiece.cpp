#include "ChessPiece.hpp"

int ChessPiece::enPassant = 0;

set<int> Pawn::getMoves(map<int,unique_ptr<ChessPiece>>& board) {
    // cout << "Pawn in getMoves at position: " << position << endl;
    int turn = (color == WHITE) ? 1 : -1;
    set<int> possibleMoves;
    PieceIterator selectedPiece = board.find(position + (8 * turn));

    // pawn push
    selectedPiece = board.find(position + (8 * turn));
    if (selectedPiece == board.end()) {
        possibleMoves.insert(position + (8 * turn));
        selectedPiece = board.find(position + (16 * turn));
        bool pawnsFirstMove = ((position >= 9 && position <= 16 && color == WHITE) || 
                               (position >= 49 && position <= 56 && color == BLACK));
        if (selectedPiece == board.end() && pawnsFirstMove) {
            possibleMoves.insert(position + (16 * turn));
        }
    }

    // Check diagonal captures and en passant
    Direction dir;
    if (position % 8 != 0) { // Not on column h
        dir = (color == WHITE ? RIGHT : LEFT);
        if (canPawnTakeDiag(board, dir) || canPawnTakeEnPassant(board, dir)) {
            possibleMoves.insert(position + ((color == WHITE ? 9 : 7) * turn));
        }
    }
    if (position % 8 != 1) { // Not on column a
        dir = (color == WHITE ? LEFT : RIGHT);
        if (canPawnTakeDiag(board, dir) || canPawnTakeEnPassant(board, dir)) {
            possibleMoves.insert(position + ((color == WHITE ? 7 : 9) * turn));
        }
    }

    return possibleMoves;
}

bool Pawn::canPawnTakeDiag(map<int,unique_ptr<ChessPiece>>& board, Direction diagonal) {
    int move, turn;
    move = (diagonal == LEFT) ? 7 : 9;
    turn = (color == WHITE) ? 1 : -1;
    PieceIterator selectedPiece = board.find(position + (move * turn));

    if (selectedPiece != board.end() && selectedPiece->second->getColor() != color) {
        return true;
    }

    return false;
}

bool Pawn::canPawnTakeEnPassant(map<int,unique_ptr<ChessPiece>>& board, Direction side) {
    int move, turn;
    move = (side == RIGHT) ? 1 : -1;
    turn = (color == WHITE) ? 1 : -1;
    PieceIterator selectedPiece = board.find(position + (move * turn));

    if (selectedPiece != board.end() && selectedPiece->second->getColor() != color && 
        selectedPiece->second->getPiece() == "Pawn" && selectedPiece->second->getEnPassant() == (position + (move * turn))) {
        return true;
    }

    return false;
}

set<int> Rook::getMoves(map<int,unique_ptr<ChessPiece>>& board) {
    // cout << "in rook getMoves" << endl;
    set<int> directions = {8, -8, 1, -1};
    return checkDirection(board, directions);
}

set<int> Knight::getMoves(map<int,unique_ptr<ChessPiece>>& board) {
    // cout << "in knight getMoves" << endl;
    set<int> possibleMoves, knightMoves = {-17,-15,-10,-6,6,10,15,17};

    // eliminate moves that are invalid based on the position of the knight
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

    for (auto& moveInc : knightMoves) {
        int move = moveInc + position;
        if (!(move < 1 || move > 64)) {
            if (board.find(move) == board.end() || 
               (board.find(move) != board.end() && board.find(move)->second->getColor() != color)) {
                possibleMoves.insert(move);
            }
        }
    }

    return possibleMoves;
}

set<int> Bishop::getMoves(map<int,unique_ptr<ChessPiece>>& board) {
    // cout << "in bishop getMoves" << endl;
    set<int> directions = {7, -7, 9, -9};
    return checkDirection(board, directions);
}

set<int> Queen::getMoves(map<int,unique_ptr<ChessPiece>>& board) {
    // cout << "in queen getMoves" << endl;
    set<int> directions = {8, -8, 1, -1, 7, -7, 9, -9};
    return checkDirection(board, directions);
}

set<int> King::getMoves(map<int,unique_ptr<ChessPiece>>& board) {
    // cout << "in king getMoves" << endl;
    set<int> possibleMoves, kingMoves = {-1, 1, -7, 7, -8, 8, -9, 9};

    for (set<int>::iterator moveIter = kingMoves.begin(); moveIter != kingMoves.end(); ++moveIter) {
        int movedPosition = position + *moveIter;
        PieceIterator nextSquarePiece = board.find(movedPosition);
        bool stayOnRow = (*moveIter == -1 && position % 8 == 1) || (*moveIter == 1 && position % 8 == 0);
        bool onEdge = ((*moveIter == 7 || *moveIter == -9) && position % 8 == 1) || 
                      ((*moveIter == -7 || *moveIter == 9) && position % 8 == 0);
        bool inBounds = (movedPosition < 1 || movedPosition > 64) || 
                        (nextSquarePiece != board.end() && nextSquarePiece->second->getColor() == color);
        if (!(onEdge || stayOnRow || inBounds)) {
            possibleMoves.insert(movedPosition);
        }
    }

    return possibleMoves;
}

set<int> ChessPiece::checkDirection(map<int,unique_ptr<ChessPiece>>& board, set<int> directions) {
    set<int> possibleMoves, temp;
    for (auto dirIter = directions.begin(); dirIter != directions.end(); ++dirIter) {
        temp = checkSquareRecursive(board, position, *dirIter);
        possibleMoves.insert(temp.begin(), temp.end());
        temp.clear();
    }
    return possibleMoves;
}

set<int> ChessPiece::checkSquareRecursive(map<int,unique_ptr<ChessPiece>>& board, int currentPos, int moveIncrem) {
    set<int> possibleMoves, nextPossibleMove;
    int nextPos = currentPos + moveIncrem;
    PieceIterator nextSquarePiece = board.find(nextPos);
    bool stayOnRow = (moveIncrem == -1 && currentPos % 8 == 1) || (moveIncrem == 1 && currentPos % 8 == 0);
    bool onEdge = ((moveIncrem == 7 || moveIncrem == -9) && currentPos % 8 == 1) || 
                  ((moveIncrem == -7 || moveIncrem == 9) && currentPos % 8 == 0);
    bool inBounds = (nextPos < 1 || nextPos > 64) || 
                    (nextSquarePiece != board.end() && nextSquarePiece->second->getColor() == color);
    if (stayOnRow || inBounds || onEdge) {
        return possibleMoves;
    }
    if (nextSquarePiece != board.end() && nextSquarePiece->second->getColor() != color) {
        possibleMoves.insert(nextPos);
        return possibleMoves;
    } else {
        possibleMoves.insert(nextPos);
    }
    nextPossibleMove = checkSquareRecursive(board, nextPos, moveIncrem);
    possibleMoves.insert(nextPossibleMove.begin(), nextPossibleMove.end());
    return possibleMoves;
}