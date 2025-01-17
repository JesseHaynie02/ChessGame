#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP
#include <map>
#include <memory>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "ChessPiece.hpp"

using PieceIterator = map<int,unique_ptr<ChessPiece>>::iterator;

class ChessBoard {
public:
    ChessBoard();
    void printPieces();
    bool performMove(string move, Color color);
    bool getGameOver() {return gameOver;}
    void setGameOver(bool result) {gameOver = result;}
    bool getDraw() {return draw;}
    void setDraw(bool result) {draw = result;}
    set<int> getMovesGraphic(int square);
    void moveDo(int originLoc, int newLoc, string pieceMoved, bool isPieceTake, int& pieceTakenLoc, string& pieceTaken, Color color);
    PieceIterator moveUndo(int originLoc, int newLoc, string pieceMoved, int pieceTakenLoc, string pieceTaken, Color color);
    bool isDraw(Color color);
    bool isCheck(Color color);
    bool isCheckMate(Color color);
    void checkEnPassant(int originLoc, int squareToMoveTo, Color color);
    void checkCastling(string pieceMoved, string pieceTaken, int originLoc, int newLoc, Color color);
    bool clearToCastle(string move, Color color);
    bool castle(string side, int kingLoc, int rookLoc, Color color);
    bool isPawnPromotion(string pieceType, string move, Color color);

    map<int,string> gridItos;
private:
    PieceIterator findPiece(string move, Color color);
    string serializeBoardHistory();
    int serializeFiftyMoveRule();
    size_t hashBoardHistory(string gameState);
    size_t hashFiftyMoveRule(int gameState);
    bool isThreeFold(size_t currentGameState);
    bool isFiftyMoveRule(size_t currentHashState);
    bool isStalemate(Color color);
    bool isCheckMateImpossible();

    bool whiteShortCastle, whiteLongCastle, blackShortCastle, blackLongCastle;
    bool gameOver;
    bool draw;
    map<int,unique_ptr<ChessPiece>> board;
    map<string,int> gridStoi;
    unordered_map<size_t,int> boardHistory;
    pair<size_t,int> fiftyMoveRule;
};
#endif