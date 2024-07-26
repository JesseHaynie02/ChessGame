#include "ChessBoard.hpp"

ChessBoard::ChessBoard() {
    for (int i = 9; i <= 16; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, WHITE)));
    }
    for (int i = 49; i <= 56; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, BLACK)));
    }
    // test piece
    // board.insert(pair<int,unique_ptr<ChessPiece>>(28, make_unique<Pawn>(28, 1, BLACK)));
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

bool ChessBoard::movePiece(string move, Color color) {
    map<char,string> promotionPieces {{'K',"Knight"},{'Q',"Queen"},{'R',"Rook"},{'B',"Bishop"}};
    string promotionType;
    if (move.size() < 2 && move.size() > 7) {
        return false;
    }

    if (move.back() == '+') {
        move.pop_back();
    } else if (move.back() == '#') {
        move.pop_back();
    }

    // bool isPromotion = (move.back() == '+') || (move[0] == 'N' && piece->second->getPiece() == "Knight");
    bool isPromotion = (isupper(move.back())) && (promotionPieces.find(move.back()) != promotionPieces.end()) 
                        && move.at(move.size() - 2) == '=';
    if (isPromotion) {
        // isPawnPromotion (implement further)
        promotionType = promotionPieces.at(move.back());
        move.pop_back();
        move.pop_back();
    }

    int countXs = count(move.begin(), move.end(), 'x');
    bool isPieceTake = (countXs == 1) && ((move.size() == 4 && move.at(1) == 'x') || (move.size() == 5 && move.at(2) == 'x'));

    if (isPieceTake) {
        move.erase(move.find('x'), 1);
    }

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
            string oldPiece = selectedPiece->second->getPiece();
            string pieceMoved = ((isPromotion && isPawnPromotion(oldPiece, move, color)) ? promotionType : oldPiece);
            string pieceTaken = "";
            moveDo(originalLocOfPiece, newLocOfPiece, pieceMoved, isPieceTake, pieceTakenLoc, pieceTaken, color);
            if (inCheck(color)) {
                moveUndo(originalLocOfPiece, newLocOfPiece, oldPiece, pieceTakenLoc, pieceTaken, color);
                return false;
            }
            checkEnPassant(originalLocOfPiece, newLocOfPiece, color);
            if (inCheckMate(color == WHITE ? BLACK : WHITE)) {
                cout << "found checkmate" << endl;
                setGameOver(true);
            }
            return true;
        }
    }

    return false;
}

void ChessBoard::moveDo(int originalLocOfPiece, int newLocOfPiece, string pieceMoved, bool isPieceTake, int &pieceTakenLoc, string &pieceTaken, Color color) {
    PieceIterator selectedPiece = board.find(originalLocOfPiece);
    PieceIterator squareToMoveTo = board.find(newLocOfPiece);
    if (isPieceTake) {
        if (pieceMoved == "Pawn" && squareToMoveTo == board.end()) {
            PieceIterator pawnToTake = board.end();
            pawnToTake = board.find(newLocOfPiece - (8 * (color == WHITE ? 1 : -1)));
            cout << selectedPiece->second->getEnPassant() << " == " << pawnToTake->second->getPosition() << endl;
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
    if (pieceMoved == "Pawn") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLocOfPiece, make_unique<Pawn>(newLocOfPiece, 1, color)));
    } else if (pieceMoved == "Rook") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLocOfPiece, make_unique<Rook>(newLocOfPiece, 5, color)));
    } else if (pieceMoved == "Knight") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLocOfPiece, make_unique<Knight>(newLocOfPiece, 3, color)));
    } else if (pieceMoved == "Bishop") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLocOfPiece, make_unique<Bishop>(newLocOfPiece, 3, color)));
    } else if (pieceMoved == "Queen") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLocOfPiece, make_unique<Queen>(newLocOfPiece, 9, color)));
    } else if (pieceMoved == "King") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLocOfPiece, make_unique<King>(newLocOfPiece, 10, color)));
    }
    board.erase(selectedPiece);
}

PieceIterator ChessBoard::moveUndo(int originalLocOfPiece, int newLocOfPiece, string pieceMoved, int pieceTakenLoc, string pieceTaken, Color color) {
    // cout << "in moveUndo" << endl;
    // cout << "pieceTaken: " << pieceTaken << endl;
    Color colorTaken = (color == WHITE ? BLACK : WHITE);
    board.erase(newLocOfPiece);
    if (pieceTaken == "Pawn") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Pawn>(pieceTakenLoc, 1, colorTaken)));
    } else if (pieceTaken == "Rook") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Rook>(pieceTakenLoc, 5, colorTaken)));
    } else if (pieceTaken == "Knight") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Knight>(pieceTakenLoc, 3, colorTaken)));
    } else if (pieceTaken == "Bishop") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Bishop>(pieceTakenLoc, 3, colorTaken)));
    } else if (pieceTaken == "Queen") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<Queen>(pieceTakenLoc, 9, colorTaken)));
    } else if (pieceTaken == "King") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(pieceTakenLoc, make_unique<King>(pieceTakenLoc, 10, colorTaken)));
    }

    if (pieceMoved == "Pawn") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Pawn>(originalLocOfPiece, 1, color)));
    } else if (pieceMoved == "Rook") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Rook>(originalLocOfPiece, 5, color)));
    } else if (pieceMoved == "Knight") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Knight>(originalLocOfPiece, 3, color)));
    } else if (pieceMoved == "Bishop") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Bishop>(originalLocOfPiece, 3, color)));
    } else if (pieceMoved == "Queen") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<Queen>(originalLocOfPiece, 9, color)));
    } else if (pieceMoved == "King") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originalLocOfPiece, make_unique<King>(originalLocOfPiece, 10, color)));
    }
    return board.find(originalLocOfPiece);
}

// will need to implement when two rooks or knights can access same square (implemented, not tested)
PieceIterator ChessBoard::findPiece(string move, Color color) {
    // cout << "In findpiece" << endl;
    set<char> chessColumns {'a','b','c','d','e','f','g','h'};
    map<char,string> moveToPiece {{'R', "Rook"},{'N', "Knight"},{'B', "Bishop"},{'Q', "Queen"},{'K', "King"},{'+',"looking for king"}};
    set<int> possibleMoves;
    string locOfMove = move.substr(move.size() - 2, 2);
    PieceIterator selectedPiece = board.end();

    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        string nameOfPiece;
        if (moveToPiece.find(move[0]) != moveToPiece.end()) {
            nameOfPiece = moveToPiece[move[0]];
        } else if (chessColumns.find(move[0]) != chessColumns.end()) {
            nameOfPiece = "Pawn";
        }
        bool isCorrectPiece = (nameOfPiece == piece->second->getPiece());
        if (piece->second->getColor() == color) {
            cout << nameOfPiece << " == " << piece->second->getPiece() << endl;
            possibleMoves.clear();
            possibleMoves = piece->second->getMoves(board);
            cout << "Looking for position = " << grid.at(locOfMove) << endl;
            cout << "Moves: ";
            for (auto &moves : possibleMoves) {
                cout << moves << " ";
            }
            cout << endl;
            // cout << "determining if move is in set of possibleMoves" << endl;
            if (nameOfPiece == "looking for king" && possibleMoves.find(grid.at(locOfMove)) != possibleMoves.end()) {
                cout << "King found from opposing side" << endl;
                selectedPiece = piece;
                break;
            }
            if (possibleMoves.find(grid.at(locOfMove)) != possibleMoves.end()) {
                if (move.size() == 2 && isCorrectPiece) {
                    // cout << "In if case move.size() == 2\n" << nameOfPiece << " == " << piece->second->getPiece() << endl;
                    selectedPiece = piece;
                    break;
                } else if (move.size() == 3 && isCorrectPiece) {
                    // cout << "In if case move.size() == 3\n" << nameOfPiece << " == " << piece->second->getPiece() << endl;
                    selectedPiece = piece;
                    break;
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

bool ChessBoard::inCheck(Color color) {
    PieceIterator kingIter = board.end();
    PieceIterator selectedPiece = board.end();
    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        if (piece->second->getPiece() == "King" && piece->second->getColor() == color) {
            kingIter = piece;
        }
    }

    string locOfKing = "+";
    for (map<string,int>::iterator gridIter = grid.begin(); gridIter != grid.end(); ++gridIter) {
        if (gridIter->second == kingIter->second->getPosition()) {
            locOfKing += gridIter->first;
        }
    }

    // cout << "location of king = " << locOfKing << endl;
    selectedPiece = findPiece(locOfKing, (color == WHITE ? BLACK : WHITE));
    if (selectedPiece != board.end()) {
        cout << "king is in check" << endl;
        return true;
    }
    cout << "king is not in check" << endl;
    return false;
}

bool ChessBoard::inCheckMate(Color color) {
    if (!inCheck(color)) {
        return false;
    }

    // cout << "in inCheckMate" << endl;
    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        if (piece->second->getColor() == color) {
            // cout << "Looking at piece: " << piece->second->getPiece() << " position = " << piece->second->getPosition() << ", value = " << piece->second->getValue() 
                //  << ", color = " << (piece->second->getColor() ? "Black" : "White") << endl;
            set<int> possibleMoves = piece->second->getMoves(board);
            // cout << "Moves: ";
            // for (auto &moves : possibleMoves) {
            //     cout << moves << " ";
            // }
            // cout << endl;
            for (set<int>::iterator pieceMove = possibleMoves.begin(); pieceMove != possibleMoves.end(); ++pieceMove) {
                // cout << "pieceMove: " << *pieceMove << endl;
                PieceIterator squareToMoveTo = board.find(*pieceMove);
                int originalLocOfPiece = piece->second->getPosition();
                int newLocOfPiece = *pieceMove;
                int pieceTakenLoc = newLocOfPiece;
                string pieceMoved = piece->second->getPiece();
                string pieceTaken = "";
                bool isPieceTake = squareToMoveTo != board.end() && squareToMoveTo->second->getColor() != color;
                moveDo(originalLocOfPiece, newLocOfPiece, pieceMoved, isPieceTake, pieceTakenLoc, pieceTaken, color);
                // cout << "after moveDo printing board" << endl;
                // printPieces();
                if (!inCheck(color)) {
                    moveUndo(originalLocOfPiece, newLocOfPiece, pieceMoved, pieceTakenLoc, pieceTaken, color);
                    // cout << "king is not checkmated" << endl;
                    return false;
                }
                // cout << "king is still in check" << endl;
                // cout << "pieceTaken: " << pieceTaken << endl;
                // cout << "pieceTakenLoc: " << pieceTakenLoc << endl;
                piece = moveUndo(originalLocOfPiece, newLocOfPiece, pieceMoved, pieceTakenLoc, pieceTaken, color);
                // printPieces();
                // cout << "undid move" << endl;
            }
        }
    }
    // cout << "king is checkmated" << endl;
    return true;
}

void ChessBoard::checkEnPassant(int originalLocOfPiece, int squareToMoveTo, Color color) {
    bool pawnsFirstMove = ((originalLocOfPiece >= 9 && originalLocOfPiece <= 16 && color == WHITE) || 
                           (originalLocOfPiece >= 49 && originalLocOfPiece <= 56 && color == BLACK));
    bool pawnDoublePush = ((squareToMoveTo >= 25 && squareToMoveTo <= 32) || (squareToMoveTo >= 33 && squareToMoveTo <= 40));
    if (pawnsFirstMove && pawnDoublePush) {
        cout << "setEnPassant enabled" << endl;
        board.find(squareToMoveTo)->second->setEnPassant(squareToMoveTo);
    } else {
        cout << "setEnPassant disabled" << endl;
        board.find(squareToMoveTo)->second->setEnPassant(0);
    }
}

bool ChessBoard::isPawnPromotion(string pieceType, string move, Color color) {
    if (pieceType == "Pawn" && (color == WHITE && move.back() == '8' || color == BLACK && move.back() == '1')) {
        return true;
    }
    return false;
}