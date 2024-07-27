#include "main.hpp"

int main() {
    cout << "Chess Game" << endl;
    ChessBoard board;
    Color turn = WHITE;
    Color &refTurn = turn;
    // board.printPieces();

    while (!board.getGameOver() && !board.getDraw()) {
        // get move
        board.printPieces();
        string move = getNextMove(refTurn);

        // determine if move is valid
        cout << "move = " << move << endl;
        if (!board.movePiece(move, turn)) {
            cout << "Please enter a valid move" << endl;
            continue;
        }
        (refTurn == WHITE) ? refTurn = BLACK: refTurn = WHITE;
        if (board.getGameOver()) {
            cout << "Game Over: " << ((refTurn == WHITE) ? "Black Won" : "White Won") << endl;
        } else if (board.getDraw()) {
            cout << "Game is a Draw" << endl;
        }
    }
    return 0;
}

string getNextMove(Color& color) {
    string nextMove;

    (color == WHITE) ? cout << "White to Move: ": cout << "Black to Move: ";
    cin >> nextMove;
    cin.sync();

    return nextMove;
}