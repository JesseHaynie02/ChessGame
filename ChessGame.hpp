#ifndef CHESSGAME_HPP
#define CHESSGAME_HPP
#include "ChessBoard.hpp"

enum Winner {WHITE_WINS, BLACK_WINS, DRAW, ERROR};

Winner playChess(istream& input);
string getNextMove(Color color, istream& input);

#endif