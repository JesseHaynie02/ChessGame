#include "ChessBoard.hpp"

ChessBoard::ChessBoard() {
    for (int i = 9; i <= 16; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, WHITE)));
    }
    for (int i = 49; i <= 56; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, BLACK)));
    }
    // test piece
    // board.insert(pair<int,unique_ptr<ChessPiece>>(28, make_unique<Rook>(28, 5, WHITE)));
    // board.insert(pair<int,unique_ptr<ChessPiece>>(29, make_unique<Rook>(29, 5, BLACK)));

    board.insert(pair<int,unique_ptr<ChessPiece>>(1, make_unique<Rook>(1, 5, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(2, make_unique<Knight>(2, 3, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(3, make_unique<Bishop>(3, 3, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(4, make_unique<Queen>(4, 9, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(5, make_unique<King>(5, 10, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(6, make_unique<Bishop>(6, 3, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(7, make_unique<Knight>(7, 3, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(8, make_unique<Rook>(8, 5, WHITE)));

    board.insert(pair<int,unique_ptr<ChessPiece>>(57, make_unique<Rook>(57, 5, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(58, make_unique<Knight>(58, 3, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(59, make_unique<Bishop>(59, 3, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(60, make_unique<Queen>(60, 9, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(61, make_unique<King>(61, 10, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(62, make_unique<Bishop>(62, 3, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(63, make_unique<Knight>(63, 3, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(64, make_unique<Rook>(64, 5, BLACK)));

    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            string position;
            position += (j - 1) + 'a';
            position += i + '0';
            // cout << position << " = " << ((i - 1) * 8) + j << endl;
            grid.insert(pair<string,int>(position, ((i - 1) * 8) + j));
        }
    }
}

void ChessBoard::printPieces() {
    // cout << "board size = " << board.size() << endl;
    for (auto& piece : board) {
        cout << piece.second->getPiece() << " position = " << piece.second->getPosition() << ", value = " << piece.second->getValue() 
       << ", color = " << (piece.second->getColor() ? "Black" : "White") << endl;
    }
}

// does move put ones own king in check? if so cant make move
//     possible soln: move piece (call find piece and pass it the king and if the king is now in one of the pieces possible moves move is not possible) move piece back 
// does move put other king in check/checkmate
bool ChessBoard::movePiece(string move, Color color) {
    set<char> promotionPieces {'K','B','R','Q'};
    if (move.size() < 2 && move.size() > 7) {
        return false;
    }

    // Is it necessary to keep a flag noting that the player indicated check/mate or should I just
    // remove the character and check for myself wether the king is in check/mate?
    if (move.back() == '+') {
        // check true (implement further)
        move.pop_back();
    } else if (move.back() == '#') {
        // checkmate true (implement further)
        move.pop_back();
    }

    // bool isPromotion = (move.back() == '+') || (move[0] == 'N' && piece->second->getPiece() == "Knight");
    bool isPromotion = (isupper(move.back())) && (promotionPieces.find(move.back()) != promotionPieces.end()) 
                        && move.at(move.size() - 2) == '=';
    if (isPromotion) {
        // isPawnPromotion (implement further)
        move.pop_back();
        move.pop_back();
    }

    int countXs = count(move.begin(), move.end(), 'x');
    bool isPieceTake = (countXs == 1) && ((move.size() == 4 && move.at(1) == 'x') || (move.size() == 5 && move.at(2) == 'x'));

    if (isPieceTake) {
        // remove take in order to make finding piece easier (implement further)
        // cout << "Take occurs" << endl;
        move.erase(move.find('x'), 1);
    }
    // cout << "After take new string: " << move << endl;

    // prolly can just combine this if case
    if (move == "O-O") {
        // King side castle (implement further)
    } else if (move == "O-O-O") {
        // Queen side castle (implement further)
    }

    string locOfMove = move.substr(move.size() - 2, 2);
    bool isMoveOnGrid = grid.find(locOfMove) != grid.end();
    PieceIterator selectedPiece = board.end();
    PieceIterator squareToMoveTo = board.end();
    // int selectedPiece = 0;
    board.begin();
    if (isMoveOnGrid) {
        cout << "move is on grid" << endl;
        selectedPiece = findPiece(move, color);
        squareToMoveTo = board.find(grid.at(locOfMove));
        if (selectedPiece != board.end()) {
            int originalLocOfPiece = selectedPiece->second->getPosition();
            int newLocOfPiece = grid.at(locOfMove);
            int pieceTakenLoc = newLocOfPiece;
            string pieceTaken = "";
            cout << "piece detected" << endl;
            if (isPieceTake) {
                // delete piece
                // cout << "in piece take" << endl;
                if (selectedPiece->second->getPiece() == "Pawn" && squareToMoveTo == board.end()) {
                    PieceIterator pawnToTake = board.end();
                    pawnToTake = board.find(grid.at(locOfMove) - (8 * (color ? 1 : -1)));
                    if (pawnToTake != board.end() && selectedPiece->second->getEnPassant() == pawnToTake->second->getPosition()) {
                        // cout << "deleting en passant take" << endl;
                        pieceTakenLoc = pawnToTake->second->getPosition();
                        pieceTaken = "Pawn";
                        board.erase(pawnToTake);
                    }
                } else {
                    pieceTaken = squareToMoveTo->second->getPiece();
                    board.erase(squareToMoveTo);
                }
            }
            // move piece
            checkEnPassant(selectedPiece, grid.at(locOfMove));
            string piece = selectedPiece->second->getPiece();
            if (piece == "Pawn") {
                board.insert(pair<int,unique_ptr<ChessPiece>>(grid.at(locOfMove), make_unique<Pawn>(grid.at(locOfMove), selectedPiece->second->getValue(), color)));
            } else if (piece == "Rook") {
                board.insert(pair<int,unique_ptr<ChessPiece>>(grid.at(locOfMove), make_unique<Rook>(grid.at(locOfMove), selectedPiece->second->getValue(), color)));
            } else if (piece == "Knight") {
                board.insert(pair<int,unique_ptr<ChessPiece>>(grid.at(locOfMove), make_unique<Knight>(grid.at(locOfMove), selectedPiece->second->getValue(), color)));
            } else if (piece == "Bishop") {
                board.insert(pair<int,unique_ptr<ChessPiece>>(grid.at(locOfMove), make_unique<Bishop>(grid.at(locOfMove), selectedPiece->second->getValue(), color)));
            } else if (piece == "Queen") {
                board.insert(pair<int,unique_ptr<ChessPiece>>(grid.at(locOfMove), make_unique<Queen>(grid.at(locOfMove), selectedPiece->second->getValue(), color)));
            } else if (piece == "King") {
                board.insert(pair<int,unique_ptr<ChessPiece>>(grid.at(locOfMove), make_unique<King>(grid.at(locOfMove), selectedPiece->second->getValue(), color)));
            }
            board.erase(selectedPiece);

            // return !moveCausesCheck(originalLocOfPiece, newLocOfPiece, pieceTakenLoc, piece, pieceTaken, color);
            return true;
        }
    }

    return false;
}

// function implemented but not tested
bool ChessBoard::moveCausesCheck(int originalLocOfPiece, int newLocOfPiece, int pieceTakenLoc, string pieceMoved, string pieceTaken, Color color) {

    PieceIterator kingIter = board.end();
    PieceIterator selectedPiece = board.end();
    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        if (piece->second->getPiece() == "King" && piece->second->getColor() == color) {
            kingIter = piece;
        }
    }

    string locOfKing = "";
    for (map<string,int>::iterator gridIter = grid.begin(); gridIter != grid.end(); ++gridIter) {
        if (gridIter->second == kingIter->second->getPosition()) {
            locOfKing = gridIter->first;
        }
    }

    selectedPiece = findPiece(locOfKing, color);
    if (selectedPiece != board.end()) { // move does cause check should move piece back and return true
        board.erase(newLocOfPiece);
        if (pieceTaken == "Pawn") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Pawn>(pieceTakenLoc, selectedPiece->second->getValue(), color)));
        } else if (pieceTaken == "Rook") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Rook>(pieceTakenLoc, selectedPiece->second->getValue(), color)));
        } else if (pieceTaken == "Knight") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Knight>(pieceTakenLoc, selectedPiece->second->getValue(), color)));
        } else if (pieceTaken == "Bishop") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Bishop>(pieceTakenLoc, selectedPiece->second->getValue(), color)));
        } else if (pieceTaken == "Queen") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Queen>(pieceTakenLoc, selectedPiece->second->getValue(), color)));
        } else if (pieceTaken == "King") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<King>(pieceTakenLoc, selectedPiece->second->getValue(), color)));
        }

        if (pieceMoved == "Pawn") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Pawn>(originalLocOfPiece, selectedPiece->second->getValue(), color)));
        } else if (pieceMoved == "Rook") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Rook>(originalLocOfPiece, selectedPiece->second->getValue(), color)));
        } else if (pieceMoved == "Knight") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Knight>(originalLocOfPiece, selectedPiece->second->getValue(), color)));
        } else if (pieceMoved == "Bishop") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Bishop>(originalLocOfPiece, selectedPiece->second->getValue(), color)));
        } else if (pieceMoved == "Queen") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Queen>(originalLocOfPiece, selectedPiece->second->getValue(), color)));
        } else if (pieceMoved == "King") {
            board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<King>(originalLocOfPiece, selectedPiece->second->getValue(), color)));
        }
    }

    return false;
}

// will need to implement when two rooks or knights can access same square (implemented, not tested)
PieceIterator ChessBoard::findPiece(string move, Color color) {
    cout << "In findpiece" << endl;
    set<char> chessColumns {'a','b','c','d','e','f','g'};
    set<int> possibleMoves;
    string locOfMove = move.substr(move.size() - 2, 2);
    PieceIterator selectedPiece = board.end();

    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        bool isCorrectPiece = (chessColumns.find(move[0]) != chessColumns.end()) || ((move[0] == piece->second->getPiece()[0]) || 
                              (move[0] == 'N' && piece->second->getPiece() == "Knight"));
        if (piece->second->getColor() == color) {
            possibleMoves.clear();
            possibleMoves = piece->second->getMoves(board);
            // cout << "Moves: ";
            // for (auto &moves : possibleMoves) {
            //     cout << moves << " ";
            // }
            // cout << endl;
            // cout << "determining if move is in set of possibleMoves" << endl;
            if (possibleMoves.find(grid.at(locOfMove)) != possibleMoves.end()) {
                if (move.size() == 2) {
                    selectedPiece = piece;
                    break;
                } else if (move.size() == 3) {
                    if (isCorrectPiece) {
                        selectedPiece = piece;
                        break;
                    }
                } else if (move.size() == 4) { // two of the same pieces can move to the same square
                    int pieceLoc = move[1] - '0';
                    if (pieceLoc <= 8) { // knight or rook is on the same row
                        int findRow = piece->second->getPosition() / 8;
                        (piece->second->getPosition() % 8 == 0) ? findRow : ++findRow;  
                        if (pieceLoc == findRow && isCorrectPiece) {
                            selectedPiece = piece;
                            break;
                        }
                    } else if (pieceLoc >= 97 && pieceLoc <= 104) { // knight or rook is on the same column
                        int findCol = piece->second->getPosition() % 8;
                        pieceLoc %= 8;
                        if (pieceLoc == findCol && isCorrectPiece) {
                            selectedPiece = piece;
                            break;
                        }
                    }
                } else if (move.size() == 5) { // three or more of the same pieces can see the square (rare case)
                    int pieceLoc = grid.at(move.substr(1,2));
                    if (pieceLoc == piece->second->getPosition() && isCorrectPiece) {
                        selectedPiece = piece;
                        break;
                    }
                }
            }
        }
    }

    // cout << "found piece to move at square: " << selectedPiece->second->getPosition() << endl;
    return selectedPiece;
}

void ChessBoard::checkEnPassant(PieceIterator selectedPiece, int squareToMoveTo) {
    int position = selectedPiece->second->getPosition();
    Color color = selectedPiece->second->getColor();
    bool pawnsFirstMove = ((position >= 9 && position <= 16 && color == WHITE) || (position >= 49 && position <= 56 && color == BLACK));
    bool pawnDoublePush = ((squareToMoveTo >= 25 && squareToMoveTo <= 32) || (squareToMoveTo >= 33 && squareToMoveTo <= 40));
    if (pawnsFirstMove && pawnDoublePush) {
        cout << "setEnPassant enabled" << endl;
        selectedPiece->second->setEnPassant(squareToMoveTo);
    } else {
        cout << "setEnPassant disabled" << endl;
        selectedPiece->second->setEnPassant(0);
    }
}