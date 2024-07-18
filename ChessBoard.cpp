#include "ChessBoard.hpp"

ChessBoard::ChessBoard() {
    for (int i = 9; i <= 16; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, WHITE)));
    }
    for (int i = 49; i <= 56; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, BLACK)));
    }
    // test piece
    board.insert(pair<int,unique_ptr<ChessPiece>>(28, make_unique<Pawn>(28, 1, BLACK)));

    board.insert(pair<int,unique_ptr<ChessPiece>>(1, make_unique<Rook>(1, 5, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(2, make_unique<Knight>(2, 3, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(3, make_unique<Bishop>(3, 3, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(4, make_unique<Queen>(4, 9, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(5, make_unique<King>(5, 100, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(6, make_unique<Rook>(6, 5, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(7, make_unique<Knight>(7, 3, WHITE)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(8, make_unique<Bishop>(8, 3, WHITE)));

    board.insert(pair<int,unique_ptr<ChessPiece>>(57, make_unique<Rook>(57, 5, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(58, make_unique<Knight>(58, 3, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(59, make_unique<Bishop>(59, 3, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(60, make_unique<Queen>(60, 9, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(61, make_unique<King>(61, 100, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(62, make_unique<Rook>(62, 5, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(63, make_unique<Knight>(63, 3, BLACK)));
    board.insert(pair<int,unique_ptr<ChessPiece>>(64, make_unique<Bishop>(64, 3, BLACK)));

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

// If a take occurs check to see there is only one x set take flag to true and remove the x in the move
//      note: a correct x can occur only if one x is present
// Remove the promotion notation ex. =Q
// Move the piece once the correct piece has been found
bool ChessBoard::movePiece(string move, Color color) {
    set<char> chessColumns {'a','b','c','d','e','f','g'}; // (possibly deprecated)
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
        // cout << "move is on grid" << endl;
        selectedPiece = findPiece(move, color);
        squareToMoveTo = board.find(grid.at(locOfMove));
        if (selectedPiece != board.end()) {
            // cout << "piece detected" << endl;
            if (isPieceTake) {
                // delete piece
                // cout << "in piece take" << endl;
                if (selectedPiece->second->getPiece() == "Pawn" && squareToMoveTo == board.end()) {
                    PieceIterator pawnToTake = board.end();
                    pawnToTake = board.find(grid.at(locOfMove) - (8 * (color ? 1 : -1)));
                    if (pawnToTake != board.end() && selectedPiece->second->getEnPassant() == pawnToTake->second->getPosition()) {
                        // cout << "deleting en passant take" << endl;
                        board.erase(pawnToTake);
                    }
                } else {
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
            return true;
        }
    }

    // bool isPawnTake = islower(move[0]) && chessColumns.find(move[0]) != chessColumns.end() && move[1] == 'x';
    // bool isBigPiece = (isupper(move[0]) && chessColumns.find(tolower(move[0])) != chessColumns.end() && isMoveOnGrid);

    // if (move.size() == 2 && isMoveOnGrid) { // Pawn push
    //     // implement further
    //     cout << "Pawn push identified" << endl;
    //     // selectedPiece = findPawnToMove(move, color);
    //     // if (selectedPiece != board.end() && !moveCausesCheck(move)) {
    //     //     board.erase(selectedPiece);
    //     //     board.insert(pair<int,unique_ptr<ChessPiece>>(grid.at(move), make_unique<Pawn>(grid.at(move), 1, color)));
    //     //     return true;
    //     // }
    // } else if (move.size() == 3 && isBigPiece) { // Big piece move
    //     // implement further
    //     cout << "Big piece move identified" << endl;
    // } else if (move.size() == 4) {
    //     if (isPawnTake) {
    //         // pawn take implement further
    //         cout << "Pawn take identified" << endl;
    //         // selectedPiece = findPawnToMove(move, color);
    //     } else if (move[1] == 'x' && isBigPiece) {
    //         // big piece take implement further
    //         cout << "Big piece take identified" << endl;
    //     }
    // }

    return false;
}

// will need to implement when two rooks or knights can access same square (implemented not tested)
PieceIterator ChessBoard::findPiece(string move, Color color) {
    set<char> chessColumns {'a','b','c','d','e','f','g'};
    set<int> possibleMoves;
    string locOfMove = move.substr(move.size() - 2, 2);
    PieceIterator selectedPiece = board.end();

    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        bool isCorrectPiece = (chessColumns.find(move[0]) != chessColumns.end()) || (move[0] == piece->second->getPiece()[0]) || 
                              (move[0] == 'N' && piece->second->getPiece() == "Knight");
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
                } else if (move.size() == 4) {
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
                }
            }
        }
    }

    // cout << "found piece to move at square: " << selectedPiece->second->getPosition() << endl;
    return selectedPiece;
}

// bool ChessBoard::moveCausesCheck(string move, Color color) {
//     for (auto &piece : board) {
//         // if (piece.second->getColor() != color && !piece.second->checksKing()) {
//         //     return true;
//         // }
//     }

//     return false;
// }

void ChessBoard::checkEnPassant(PieceIterator selectedPiece, int squareToMoveTo) {
    int position = selectedPiece->second->getPosition();
    Color color = selectedPiece->second->getColor();
    bool pawnsFirstMove = ((position >= 9 && position <= 16 && color == WHITE) || (position >= 49 && position <= 56 && color == BLACK));
    bool pawnDoublePush = ((squareToMoveTo >= 25 && squareToMoveTo <= 32) || (squareToMoveTo >= 33 && squareToMoveTo <= 40));
    if (pawnsFirstMove && pawnDoublePush) {
        // cout << "setEnPassant enabled" << endl;
        selectedPiece->second->setEnPassant(squareToMoveTo);
    } else {
        // cout << "setEnPassant disabled" << endl;
        selectedPiece->second->setEnPassant(0);
    }
}

// PieceIterator ChessBoard::findPawnToMove(string move, Color color) {
//     int direction;
//     string location = move.substr(move.size() - 2, 2);
//     (color == WHITE) ? direction = 1 : direction = -1;
//     PieceIterator iter = board.end();

//     location[0] = move[0];
//     // cout << "location = " << location << endl;
//     iter = board.find(grid.at(location));
//     if (iter == board.end() && (move[1] == '4' || move[1] == '5')) {
//         location[1] -= direction;
//         // cout << "location = " << location << endl;
//         iter = board.find(grid.at(location));
//         if (iter == board.end()) {
//             location[1] -= direction;
//             iter = board.find(grid.at(location));
//             if (iter->second->getColor() == color && iter->second->getPiece() == "Pawn") {
//                 cout << "Pawn is safe to push two square" << endl;
//                 currEnPassant = move.substr(move.size() - 2, 2);
//                 enPassantCounter = 0;
//                 // cout << "location = " << location << endl;
//                 return iter;    
//             }
//         }
//     } else if (iter == board.end()) {
//         location[1] -= direction;
//         // cout << "location = " << location << endl;
//         iter = board.find(grid.at(location));
//         if (iter->second->getColor() == color && iter->second->getPiece() == "Pawn") {
//             cout << "Pawn is safe to push one square or take piece" << endl;
//             // cout << "location = " << location << endl;
//             return iter;
//         }
//     }

//     return board.end();
// }

// bool ChessBoard::isPieceOnGrid(string move, Color color) {
//     set<char> chessColumns {'a','b','c','d','e','f','g'};
//     unordered_map<char, string> piece_name {{'R',"Rook"},{'N',"Knight"},{'B',"Bishop"},{'Q',"Queen"},{'K',"King"}};
//     if (move.size() < 2 && move.size() > 5) {
//         return false;
//     }

//     cout << "In isPieceOnGrid evaluating grid position " << move.substr(move.size() - 2, 2) << endl;
//     string potentialMove = move.substr(move.size() - 2, 2);
//     if (grid.find(potentialMove) != grid.end() && chessColumns.find(tolower(move.at(0))) != chessColumns.end()) {
//         cout << "Piece is on grid" << endl;
//         cout << "Piece identified" << endl;
//         if (move.size() == 2 || (move.size() == 4 && move[1] == 'x' && islower(move.at(0)))) {
//             cout << "Pawn identified" << endl;
//             for (int i = 1; i <= 64; ++i) {
//                 PieceIterator iter = board.find(i);
//                 if (iter != board.end() && iter->second->getPiece() == "Pawn" 
//                     && iter->second->color == color && iter->second->checkMove(*this, move)) {

//                 }
//             }
//         } else if (move.size() == 3) {
//             cout << "Big piece move identified" << endl;

//         } else if (move.size() == 4 && move[1] == 'x') {
//             cout << "Piece take identified" << endl;

//         }
//     }
//     return false;
// }