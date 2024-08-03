#include "ChessGame.hpp"

Winner playChess(istream& input) {
    cout << "Chess Game" << endl;
    ChessBoard board;
    Color turn = WHITE;

    while (!board.getGameOver() && !board.getDraw()) {
        board.printPieces();
        string move = getNextMove(turn, input);

        // determine if move is valid
        // cout << "move = " << move << endl;
        cout << move << endl;
        if (!board.performMove(move, turn)) {
            cout << "Please enter a valid move" << endl;
            continue;
        }

        if (board.getGameOver()) {
            cout << "Game Over: " << ((turn == WHITE) ? "White Won" : "Black Won") << endl;
            return ((turn == WHITE) ? WHITE_WINS : BLACK_WINS);
        } else if (board.getDraw()) {
            cout << "Game is a Draw" << endl;
            return DRAW;
        }

        turn = (turn == WHITE) ? BLACK : WHITE;
    }
    return ERROR;
}

string getNextMove(Color color, istream& input) {
    string nextMove;

    (color == WHITE) ? cout << "White to Move: ": cout << "Black to Move: ";
    input >> nextMove;
    cin.sync();

    return nextMove;
}