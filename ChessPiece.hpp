#ifndef CHESSPEICE_HPP
#define CHESSPEICE_HPP
#include <set>
#include <map>
#include <memory>

using namespace std;

class ChessBoard;
enum Color {WHITE, BLACK};
enum Direction {LEFT, RIGHT};

class ChessPiece {
public:
    ChessPiece() = default;
    ChessPiece(int p, const int v, const Color c) : position(p), value(v), color(c) {}
    virtual ~ChessPiece() = default;
    virtual string getPiece() = 0;
    virtual set<int> getMoves(map<int,unique_ptr<ChessPiece>>& board) = 0;
    int getPosition() {return position;};
    Color getColor() {return color;};
    int getValue() {return value;};
    static int getEnPassant() {return enPassant;};
    static int setEnPassant(int location) {enPassant = location;};
protected:
    int position;
    const int value;
    const Color color;
    static int enPassant;
    set<int> checkSquareRecursive(map<int,unique_ptr<ChessPiece>> &board, int currentPos, int moveIncrem);
    set<int> checkDirection(map<int,unique_ptr<ChessPiece>> &board, set<int> directions);
};

class Pawn : public ChessPiece {
public:
    Pawn(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    string getPiece() override {return string("Pawn");};
    set<int> getMoves(map<int,unique_ptr<ChessPiece>>& board) override;
private:
    bool canPawnTakeDiag(map<int,unique_ptr<ChessPiece>>& board, Direction diagonal);
    bool canPawnTakeEnPassant(map<int,unique_ptr<ChessPiece>>& board, Direction side);
};

class Rook : public ChessPiece {
public:
    Rook(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    string getPiece() override {return string("Rook");};
    set<int> getMoves(map<int,unique_ptr<ChessPiece>>& board) override;
};

class Knight : public ChessPiece {
public:
    Knight(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    string getPiece() override {return string("Knight");};
    set<int> getMoves(map<int,unique_ptr<ChessPiece>>& board) override;
};

class Bishop : public ChessPiece {
public:
    Bishop(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    string getPiece() override {return string("Bishop");};
    set<int> getMoves(map<int,unique_ptr<ChessPiece>>& board) override;
};

class Queen : public ChessPiece {
public:
    Queen(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    string getPiece() override {return string("Queen");};
    set<int> getMoves(map<int,unique_ptr<ChessPiece>>& board) override;
};

class King : public ChessPiece {
public:
    King(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    string getPiece() override {return string("King");};
    set<int> getMoves(map<int,unique_ptr<ChessPiece>>& board) override;
};

using PieceIterator = map<int,unique_ptr<ChessPiece>>::iterator;

#endif