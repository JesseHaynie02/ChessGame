#ifndef CHESSPEICE_HPP
#define CHESSPEICE_HPP
#include <string>
#include <iostream>

using namespace std;

class ChessPiece {
    friend class ChessBoard;
public:
    enum Color {WHITE, BLACK};
    ChessPiece() = default;
    ChessPiece(int p, const int v, const Color c) : position(p), value(v), color(c) {}
    virtual ~ChessPiece() = default;
    virtual void move() = 0;
    virtual ostream& printPiece() = 0;
protected:
    int position;
    const int value;
    const Color color;
};

class Pawn : public ChessPiece {
public:
    Pawn(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    void move() override;
    ostream& printPiece() override;
};

class Rook : public ChessPiece {
public:
    Rook(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    void move() override;
    ostream& printPiece() override;
};

class Knight : public ChessPiece {
public:
    Knight(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    void move() override;
    ostream& printPiece() override;
};

class Bishop : public ChessPiece {
public:
    Bishop(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    void move() override;
    ostream& printPiece() override;
};

class Queen : public ChessPiece {
public:
    Queen(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    void move() override;
    ostream& printPiece() override;
};

class King : public ChessPiece {
public:
    King(int p, const int v, const Color c) : ChessPiece(p, v, c) {}
    void move() override;
    ostream& printPiece() override;
};

#endif