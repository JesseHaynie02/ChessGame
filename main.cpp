#include "main.hpp"

int main() {
    cout << "Chess Game" << endl;
    ChessBoard board;
    Color turn = WHITE;
    Color &refTurn = turn;
    board.printPieces();

    while (!board.getGameOver()) {
        // get move
        board.printPieces();
        string move = getNextMove(refTurn);

        // determine if move is valid
        // cout << "move = " << move << endl;
        if (!board.movePiece(move, turn)) {
            cout << "Please enter a valid move" << endl;
            continue;
        }
        // board.checkEnPassant();
        (refTurn == WHITE) ? refTurn = BLACK: refTurn = WHITE;

        // make move
        // makeMove(move);

        // check for checkmate
        
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

// bool isMoveValid(ChessBoard &board, string move, Color color) {
//     if (board.isPieceOnGrid(move, color)) {
//         // cout << "Piece is on grid" << endl;
//         // if (chessColumns.find(tolower(move.at(0))) != chessColumns.end()) {
//         //     cout << "Piece identified" << endl;
//         //     if (move.size() == 2) {
//         //         cout << "Pawn push identified" << endl;
//         //         for (int i = 1; i <= 8; ++i) {
//         //             // if () {

//         //             // }
//         //         }
//         //     } else if (move.size() == 3) {
//         //         cout << "Big piece move identified" << endl;

//         //     } else if (move.size() == 4 && move[1] == 'x') {
//         //         cout << "Piece take identified" << endl;

//         //     }
//         // }
//     }
//     else {
//         // cout << "Piece is on not on grid" << endl;
//         // check for pawn promotion, castling, checks, and checkmate
//     }
//     return false;
// }