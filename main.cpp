#include "main.hpp"

int main() {
    cout << "Chess Game" << endl;
    ChessBoard board;
    Color turn = WHITE;

    while (!board.getGameOver() && !board.getDraw()) {
        board.printPieces();
        string move = getNextMove(turn);

        // determine if move is valid
        cout << "move = " << move << endl;
        if (!board.performMove(move, turn)) {
            cout << "Please enter a valid move" << endl;
            continue;
        }

        if (board.getGameOver()) {
            cout << "Game Over: " << ((turn == WHITE) ? "Black Won" : "White Won") << endl;
        } else if (board.getDraw()) {
            cout << "Game is a Draw" << endl;
        }

        turn = (turn == WHITE) ? BLACK : WHITE;
    }
    return 0;
}

string getNextMove(Color color) {
    string nextMove;

    (color == WHITE) ? cout << "White to Move: ": cout << "Black to Move: ";
    cin >> nextMove;
    cin.sync();

    return nextMove;
}