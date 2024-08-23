#ifndef CHESSGAME_HPP
#define CHESSGAME_HPP
#include <fstream> // for test coverage
#include <regex> // for test coverage
#include "ChessBoard.hpp"
#include "ChessGraphic.hpp"

enum Winner {WHITE_WINS, BLACK_WINS, DRAW, ERROR};

Winner playChess(istream& input);
string getNextMove(Color color, istream& input);

#endif