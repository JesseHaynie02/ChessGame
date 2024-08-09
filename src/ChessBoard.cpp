#include "ChessBoard.hpp"

ChessBoard::ChessBoard() {
    gameOver = false;
    draw = false;
    whiteShortCastle = true;
    whiteLongCastle = true;
    blackShortCastle = true;
    blackLongCastle = true;

    for (int i = 9; i <= 16; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, WHITE)));
    }
    for (int i = 49; i <= 56; ++i) {
        board.insert(pair<int,unique_ptr<ChessPiece>>(i, make_unique<Pawn>(i, 1, BLACK)));
    }

    // is there a way to reduce this code
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

    // test pieces
    // board.insert(pair<int,unique_ptr<ChessPiece>>(57, make_unique<King>(57, 10, BLACK)));
    // board.insert(pair<int,unique_ptr<ChessPiece>>(56, make_unique<Queen>(56, 9, WHITE)));
    // board.insert(pair<int,unique_ptr<ChessPiece>>(1, make_unique<King>(1, 10, WHITE)));
    // board.insert(pair<int,unique_ptr<ChessPiece>>(32, make_unique<Pawn>(32, 1, WHITE)));
    // board.insert(pair<int,unique_ptr<ChessPiece>>(30, make_unique<Pawn>(30, 1, WHITE)));
    // board.insert(pair<int,unique_ptr<ChessPiece>>(38, make_unique<Pawn>(38, 1, BLACK)));
    // board.insert(pair<int,unique_ptr<ChessPiece>>(40, make_unique<Pawn>(40, 1, BLACK)));

    string boardState = serializeBoardHistory();
    size_t currentHashState = hashBoardHistory(boardState);
    boardHistory[currentHashState]++;

    int fiftyMoveRuleState = serializeFiftyMoveRule();
    size_t fiftyMoveRuleHash = hashFiftyMoveRule(fiftyMoveRuleState);
    fiftyMoveRule.first = fiftyMoveRuleHash;

    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            string position;
            position += (j - 1) + 'a';
            position += i + '0';
            gridStoi.insert(pair<string,int>(position, ((i - 1) * 8) + j));
            gridItos.insert(pair<int,string>(((i - 1) * 8) + j, position));
        }
    }
}

void ChessBoard::printPieces() {
    std::cout << std::left
              << std::setw(10) << "Piece"
              << std::setw(12) << "Position"
              << std::setw(7)  << "Value"
              << "Color" << std::endl;
    for (auto& piece : board) {
        std::cout << std::left
                  << std::setw(10) << piece.second->getPiece()
                  << std::setw(12) << gridItos.at(piece.second->getPosition())
                  << std::setw(7)  << piece.second->getValue()
                  << (piece.second->getColor() ? "Black" : "White")
                  << std::endl;
    }
}

bool ChessBoard::performMove(string move, Color color) {
    map<char,string> promotionPieces {{'N',"Knight"},{'Q',"Queen"},{'R',"Rook"},{'B',"Bishop"}};
    string promotionType;
    if (move.size() < 2 && move.size() > 7) {
        return false;
    }

    if (move.back() == '+') {
        move.pop_back();
    } else if (move.back() == '#') {
        move.pop_back();
    }

    bool isPromotion = (isupper(move.back())) && (promotionPieces.find(move.back()) != promotionPieces.end());
    if (isPromotion) {
        promotionType = promotionPieces.at(move.back());
        move.pop_back();
        if (move.back() == '=') {
            move.pop_back();
        }
    }

    int countXs = count(move.begin(), move.end(), 'x');
    bool isPieceTake = (countXs == 1) && ((move.size() == 4 && move.at(1) == 'x') || (move.size() == 5 && move.at(2) == 'x'));

    if (isPieceTake) {
        move.erase(move.find('x'), 1);
    }

    if (move == "0-0") {
        move = "O-O";
    } else if (move == "0-0-0") {
        move = "O-O-O";
    }
    // castle
    if ((move == "O-O" || move == "O-O-O") && clearToCastle(move, color)) {
        // cout << "clearToCastle is true" << endl;
        int pieceTakenLoc = 0;
        string pieceTaken = "";
        int kingLoc = (color == WHITE ? 5 : 61);
        int rookLoc;
        if (color == WHITE) {
            rookLoc = (move == "O-O" ? 8 : 1);
        } else if (color == BLACK) {
            rookLoc = (move == "O-O" ? 64 : 57);
        }
        bool ableToCastle = castle(move, kingLoc, rookLoc, color);
        if (ableToCastle) {
            if (isCheckMate(color == WHITE ? BLACK : WHITE)) {
                // cout << "found checkmate" << endl;
                gameOver = true;
            } else if (isDraw(color)) {
                draw = true;
            }
        }
        return ableToCastle;
    }

    // regular move
    string locOfMove = move.substr(move.size() - 2, 2);
    bool isMoveOnGrid = gridStoi.find(locOfMove) != gridStoi.end();
    PieceIterator selectedPiece = board.end(), squareToMoveTo = board.end();
    if (isMoveOnGrid) {
        selectedPiece = findPiece(move, color);
        squareToMoveTo = board.find(gridStoi.at(locOfMove));
        if (selectedPiece != board.end()) {
            int originLoc = selectedPiece->second->getPosition();
            int newLoc = gridStoi.at(locOfMove);
            int pieceTakenLoc = newLoc;
            string oldPiece = selectedPiece->second->getPiece();
            string pieceMoved = ((isPromotion && isPawnPromotion(oldPiece, move, color)) ? promotionType : oldPiece);
            string pieceTaken = "";
            moveDo(originLoc, newLoc, pieceMoved, isPieceTake, pieceTakenLoc, pieceTaken, color);
            if (isCheck(color)) {
                moveUndo(originLoc, newLoc, oldPiece, pieceTakenLoc, pieceTaken, color);
                return false;
            }
            checkEnPassant(originLoc, newLoc, color);
            checkCastling(pieceMoved, pieceTaken, originLoc, newLoc, color);
            if (isCheckMate(color == WHITE ? BLACK : WHITE)) {
                gameOver = true;
            } else if (isDraw(color)) {
                draw = true;
            }
            return true;
        }
    }

    return false;
}

// There has to be a way to reduce the code
void ChessBoard::moveDo(int originLoc, int newLoc, string pieceMoved, bool isPieceTake, int& pieceTakenLoc, string& pieceTaken, Color color) {
    PieceIterator selectedPiece = board.find(originLoc), squareToMoveTo = board.find(newLoc);
    // cout << "deleting selectedPiece: " << selectedPiece->second->getPosition() << endl;
    if (isPieceTake) {
        if (pieceMoved == "Pawn" && squareToMoveTo == board.end()) {
            PieceIterator pawnToTake = board.end();
            pawnToTake = board.find(newLoc - (8 * (color == WHITE ? 1 : -1)));
            // cout << selectedPiece->second->getEnPassant() << " == " << pawnToTake->second->getPosition() << endl;
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
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLoc, make_unique<Pawn>(newLoc, 1, color)));
    } else if (pieceMoved == "Rook") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLoc, make_unique<Rook>(newLoc, 5, color)));
    } else if (pieceMoved == "Knight") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLoc, make_unique<Knight>(newLoc, 3, color)));
    } else if (pieceMoved == "Bishop") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLoc, make_unique<Bishop>(newLoc, 3, color)));
    } else if (pieceMoved == "Queen") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLoc, make_unique<Queen>(newLoc, 9, color)));
    } else if (pieceMoved == "King") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(newLoc, make_unique<King>(newLoc, 10, color)));
    }
    // cout << "deleting selectedPiece: " << selectedPiece->second->getPosition() << endl;
    board.erase(selectedPiece);
}

// There has to be a way to reduce the code
PieceIterator ChessBoard::moveUndo(int originLoc, int newLoc, string pieceMoved, int pieceTakenLoc, string pieceTaken, Color color) {
    // cout << "in moveUndo" << endl;
    // cout << "pieceTaken: " << pieceTaken << endl;
    Color colorTaken = (color == WHITE ? BLACK : WHITE);
    board.erase(newLoc);
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
        board.insert(pair<int,unique_ptr<ChessPiece>>(originLoc, make_unique<Pawn>(originLoc, 1, color)));
    } else if (pieceMoved == "Rook") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originLoc, make_unique<Rook>(originLoc, 5, color)));
    } else if (pieceMoved == "Knight") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originLoc, make_unique<Knight>(originLoc, 3, color)));
    } else if (pieceMoved == "Bishop") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originLoc, make_unique<Bishop>(originLoc, 3, color)));
    } else if (pieceMoved == "Queen") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originLoc, make_unique<Queen>(originLoc, 9, color)));
    } else if (pieceMoved == "King") {
        board.insert(pair<int,unique_ptr<ChessPiece>>(originLoc, make_unique<King>(originLoc, 10, color)));
    }
    return board.find(originLoc);
}

PieceIterator ChessBoard::findPiece(string move, Color color) {
    // cout << "In findpiece" << endl;
    set<char> chessColumns {'a','b','c','d','e','f','g','h'};
    map<char,string> moveToPiece {{'R', "Rook"},{'N', "Knight"},{'B', "Bishop"},{'Q', "Queen"},{'K', "King"},{'+',"looking for king"}};
    set<int> possibleMoves;
    string locOfMove = move.substr(move.size() - 2, 2);
    PieceIterator selectedPiece = board.end();

    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        string name;
        if (moveToPiece.find(move[0]) != moveToPiece.end()) {
            name = moveToPiece[move[0]];
        } else if (chessColumns.find(move[0]) != chessColumns.end()) {
            name = "Pawn";
        }
        bool isCorrectPiece = (name == piece->second->getPiece());
        if (piece->second->getColor() == color) {
            // cout << name << " == " << piece->second->getPiece() << endl;
            // cout << "Looking for position = " << gridStoi.at(locOfMove) << endl;
            // cout << "Moves: ";
            // for (auto &moves : possibleMoves) {
            //     cout << moves << " ";
            // }
            // cout << endl;
            possibleMoves.clear();
            possibleMoves = piece->second->getMoves(board);
            if (name == "looking for king" && possibleMoves.find(gridStoi.at(locOfMove)) != possibleMoves.end()) {
                // cout << "King found from opposing side" << endl;
                selectedPiece = piece;
                break;
            }
            if (possibleMoves.find(gridStoi.at(locOfMove)) != possibleMoves.end()) {
                if (move.size() == 2 && isCorrectPiece) { // Pawn push
                    selectedPiece = piece;
                    break;
                } else if (move.size() == 3 && isCorrectPiece) { // Pawn take or big piece move or take
                    if (piece->second->getPiece() == "Pawn") { // If two pawns can take the same piece get the right pawn
                        if (gridItos.at(piece->second->getPosition())[0] == move[0]) {
                            selectedPiece = piece;
                        } else {
                            continue;
                        }
                    } else {
                        selectedPiece = piece;
                    }
                    break;
                } else if (move.size() == 4) { // two of the same pieces can move/take to the same square
                    // cout << "size 4 move detected" << endl;
                    int pieceLoc = move[1] - '\0';
                    if (pieceLoc >= 48 && pieceLoc <= 56) { // piece is on the same row
                        pieceLoc -= 48;
                        int findRow = piece->second->getPosition() / 8;
                        (piece->second->getPosition() % 8 == 0) ? findRow : ++findRow;  
                        if (pieceLoc == findRow && isCorrectPiece) {
                            selectedPiece = piece;
                            break;
                        }
                    } else if (pieceLoc >= 97 && pieceLoc <= 104) { // piece is on the same column
                        int findCol = piece->second->getPosition() % 8;
                        pieceLoc %= 8;
                        if (pieceLoc == findCol && isCorrectPiece) {
                            selectedPiece = piece;
                            break;
                        }
                    }
                } else if (move.size() == 5) { // three or more of the same pieces can see the square (rare case)
                    int pieceLoc = gridStoi.at(move.substr(1,2));
                    if (pieceLoc == piece->second->getPosition() && isCorrectPiece) {
                        selectedPiece = piece;
                        break;
                    }
                }
            }
        }
    }

    return selectedPiece;
}

// can use gridItos to find locOfKing
bool ChessBoard::isCheck(Color color) {
    PieceIterator kingIter = board.end();
    PieceIterator selectedPiece = board.end();
    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        if (piece->second->getPiece() == "King" && piece->second->getColor() == color) {
            kingIter = piece;
        }
    }

    string locOfKing = "+";
    for (map<string,int>::iterator gridStoiIter = gridStoi.begin(); gridStoiIter != gridStoi.end(); ++gridStoiIter) {
        if (gridStoiIter->second == kingIter->second->getPosition()) {
            locOfKing += gridStoiIter->first;
        }
    }

    // cout << "location of king = " << locOfKing << endl;
    selectedPiece = findPiece(locOfKing, (color == WHITE ? BLACK : WHITE));
    if (selectedPiece != board.end()) {
        // cout << "king is in check" << endl;
        return true;
    }
    // cout << "king is not in check" << endl;
    return false;
}

bool ChessBoard::isCheckMate(Color color) {
    if (!isCheck(color)) {
        return false;
    }

    // cout << "in isCheckMate" << endl;
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
                PieceIterator squareToMoveTo = board.find(*pieceMove);
                int originLoc = piece->second->getPosition();
                int newLoc = *pieceMove;
                int pieceTakenLoc = newLoc;
                string pieceMoved = piece->second->getPiece();
                string pieceTaken = "";
                bool isPieceTake = squareToMoveTo != board.end() && squareToMoveTo->second->getColor() != color;
                moveDo(originLoc, newLoc, pieceMoved, isPieceTake, pieceTakenLoc, pieceTaken, color);
                if (!isCheck(color)) {
                    moveUndo(originLoc, newLoc, pieceMoved, pieceTakenLoc, pieceTaken, color);
                    // cout << "king is not checkmated" << endl;
                    return false;
                }
                // cout << "king is still in check" << endl;
                piece = moveUndo(originLoc, newLoc, pieceMoved, pieceTakenLoc, pieceTaken, color);
            }
        }
    }
    // cout << "king is checkmated" << endl;
    return true;
}

void ChessBoard::checkEnPassant(int originLoc, int squareToMoveTo, Color color) {
    bool pawnsFirstMove = ((originLoc >= 9 && originLoc <= 16 && color == WHITE) || 
                           (originLoc >= 49 && originLoc <= 56 && color == BLACK));
    bool pawnDoublePush = ((squareToMoveTo >= 25 && squareToMoveTo <= 32) || (squareToMoveTo >= 33 && squareToMoveTo <= 40));
    if (pawnsFirstMove && pawnDoublePush) {
        // cout << "setEnPassant enabled" << endl;
        board.find(squareToMoveTo)->second->setEnPassant(squareToMoveTo);
    } else {
        // cout << "setEnPassant disabled" << endl;
        board.find(squareToMoveTo)->second->setEnPassant(0);
    }
}

void ChessBoard::checkCastling(string pieceMoved, string pieceTaken, int originLoc, int newLoc, Color color) {
    if (pieceMoved == "King") {
        if (color == WHITE) {
            whiteShortCastle = false;
            whiteLongCastle = false;
        } else if (color == BLACK) {
            blackShortCastle = false;
            blackLongCastle = false;
        }
    } else if (pieceMoved == "Rook" || pieceTaken == "Rook") {
        if (color == WHITE && newLoc != originLoc) {
            if ((pieceMoved == "Rook" && originLoc == 1) || (pieceTaken == "Rook" && newLoc == 1)) {
                whiteLongCastle = false;
            } else if ((pieceMoved == "Rook" && originLoc == 8) || (pieceTaken == "Rook" && newLoc == 8)) {
                whiteShortCastle = false;
            }
        } else if (color == BLACK && newLoc != originLoc) {
            if ((pieceMoved == "Rook" && originLoc == 57) || (pieceTaken == "Rook" && newLoc == 57)) {
                blackLongCastle = false;
            } else if ((pieceMoved == "Rook" && originLoc == 64) || (pieceTaken == "Rook" && newLoc == 64)) {
                blackShortCastle = false;
            }
        }
    }
}

bool ChessBoard::clearToCastle(string move, Color color) {
    PieceIterator one, two, three;
    if (move == "O-O") {
        if (color == WHITE && whiteShortCastle) {
            one = board.find(6);
            two = board.find(7);
            if (one == board.end() && two == board.end()) {
                return true;
            }
        } else if (color == BLACK && blackShortCastle) {
            one = board.find(62);
            two = board.find(63);
            if (one == board.end() && two == board.end()) {
                return true;
            }
        }
    } else if (move == "O-O-O") {
        if (color == WHITE && whiteLongCastle) {
            one = board.find(2);
            two = board.find(3);
            three = board.find(4);
            if (one == board.end() && two == board.end() && three == board.end()) {
                return true;
            }
        } else if (color == BLACK && blackLongCastle) {
            one = board.find(58);
            two = board.find(59);
            three = board.find(60);
            if (one == board.end() && two == board.end() && three == board.end()) {
                return true;
            }
        }
    }
    // cout << "clear to castle returned false" << endl;
    return false;
}

bool ChessBoard::castle(string side, int kingLoc, int rookLoc, Color color) {
    // cout << "in castle function" << endl;
    int pieceTakenLoc = 0;
    string pieceTaken = "";
    int direction = (side == "O-O" ? 1 : -1);
    moveDo(kingLoc, kingLoc + (1 * direction), "King", false, pieceTakenLoc, pieceTaken, color);
    if (isCheck(color)) {
        moveUndo(kingLoc, kingLoc + (1 * direction), "King", pieceTakenLoc, pieceTaken, color);
        return false;
    }
    moveDo(kingLoc + (1 * direction), kingLoc + (2 * direction), "King", false, pieceTakenLoc, pieceTaken, color);
    if (isCheck(color)) {
        moveUndo(kingLoc + (1 * direction), kingLoc + (2 * direction), "King", pieceTakenLoc, pieceTaken, color);
        return false;
    }
    moveDo(rookLoc, rookLoc + (side == "O-O" ? -2 : 3), "Rook", false, pieceTakenLoc, pieceTaken, color);
    // cout << "returng castle true" << endl;
    return true;
}

bool ChessBoard::isPawnPromotion(string pieceType, string move, Color color) {
    if (pieceType == "Pawn" && (color == WHITE && move.back() == '8' || color == BLACK && move.back() == '1')) {
        return true;
    }
    return false;
}

bool ChessBoard::isDraw(Color color) {
    string boardStateStr = serializeBoardHistory();
    int boardStateInt = serializeFiftyMoveRule();
    size_t currentHashHistory = hashBoardHistory(boardStateStr);
    size_t currentHashFiftyMoveRule = hashFiftyMoveRule(boardStateInt);
    if (isThreeFold(currentHashHistory)) {
        return true;
    } else {
        boardHistory[currentHashHistory]++;
    }
    if (isFiftyMoveRule(currentHashFiftyMoveRule)) {
        // cout << "fifty move draw" << endl;
        return true;
    }
    // cout << "Testing if " << (color == WHITE ? "BLACK" : "WHITE") << " is in stalemate" << endl;
    if (isStalemate(color == WHITE ? BLACK : WHITE)) {
        return true;
    }
    if (isCheckMateImpossible()) {
        // cout << "checkmate impossible" << endl;
        return true;
    }
    return false;
}

int ChessBoard::serializeFiftyMoveRule() {
    int gameState;

    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        gameState += hash<string>{}(piece->second->getPiece());
        gameState += piece->second->getValue();
        if (piece->second->getPiece() == "Pawn") {
            gameState += piece->second->getPosition();
        }
    }
    return gameState;
}

string ChessBoard::serializeBoardHistory() {
    string gameState;

    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        // piece, position, value, and color
        gameState += piece->second->getPiece();
        gameState += to_string(piece->second->getPosition());
        gameState += to_string(piece->second->getValue());
        gameState += to_string(piece->second->getColor());
    }

    return gameState;
}

size_t ChessBoard::hashBoardHistory(string gameState) {
    return hash<string>{}(gameState);
}

size_t ChessBoard::hashFiftyMoveRule(int gameState) {
    return hash<int>{}(gameState);
}

bool ChessBoard::isThreeFold(size_t currentHashState) {
    unordered_map<size_t,int>::iterator iter = boardHistory.find(currentHashState);
    return (iter != boardHistory.end() && iter->second >= 2);
}

bool ChessBoard::isFiftyMoveRule(size_t currentHashState) {
    if (currentHashState == fiftyMoveRule.first) {
        fiftyMoveRule.second++;
        if (fiftyMoveRule.second >= 100) {
            return true;
        }
    } else {
        fiftyMoveRule.second = 0;
        fiftyMoveRule.first = currentHashState;
    }
    return false;
}

bool ChessBoard::isStalemate(Color color) {
    // cout << "In isStalemate" << endl;
    if (isCheck(color)) {
        return false;
    }

    // cout << "in isStalemate" << endl;
    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        if (piece->second->getColor() == color) {
            set<int> possibleMoves = piece->second->getMoves(board);
            for (set<int>::iterator pieceMove = possibleMoves.begin(); pieceMove != possibleMoves.end(); ++pieceMove) {
                PieceIterator squareToMoveTo = board.find(*pieceMove);
                int originLoc = piece->second->getPosition();
                int newLoc = *pieceMove;
                int pieceTakenLoc = newLoc;
                string pieceMoved = piece->second->getPiece();
                string pieceTaken = "";
                bool isPieceTake = squareToMoveTo != board.end() && squareToMoveTo->second->getColor() != color;
                moveDo(originLoc, newLoc, pieceMoved, isPieceTake, pieceTakenLoc, pieceTaken, color);
                if (!isCheck(color)) {
                    moveUndo(originLoc, newLoc, pieceMoved, pieceTakenLoc, pieceTaken, color);
                    // cout << "Not in stalemate" << endl;
                    return false;
                }
                piece = moveUndo(originLoc, newLoc, pieceMoved, pieceTakenLoc, pieceTaken, color);
            }
        }
    }
    return true;
}

// Has to be a way to reduce this
bool ChessBoard::isCheckMateImpossible() {
    map<string,int> whitePieceCount = {{"Pawn",0},{"Rook",0},{"Knight",0},{"Bishop",0},{"Queen",0},{"King",0}};
    map<string,int> blackPieceCount = whitePieceCount;
    for (PieceIterator piece = board.begin(); piece != board.end(); ++piece) {
        if (piece->second->getColor() == WHITE) {
            whitePieceCount[piece->second->getPiece()]++;
        } else if (piece->second->getColor() == BLACK) {
            blackPieceCount[piece->second->getPiece()]++;
        }
    }
    if (whitePieceCount["King"] == 1 && blackPieceCount["King"] == 1) {
        // King vs. King
        if (whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Knight"] == 0 && 
            whitePieceCount["Bishop"] == 0 && whitePieceCount["Queen"] == 0 &&
            blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Knight"] == 0 && 
            blackPieceCount["Bishop"] == 0 && blackPieceCount["Queen"] == 0) {
            return true;
        }
        // King and Bishop vs. King
        if (whitePieceCount["Bishop"] == 1 && whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && 
            whitePieceCount["Knight"] == 0 && whitePieceCount["Queen"] == 0 &&
            blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Knight"] == 0 && 
            blackPieceCount["Bishop"] == 0 && blackPieceCount["Queen"] == 0) {
            return true;
        }
        // King and Knight vs. King
        if (whitePieceCount["Knight"] == 1 && whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && 
            whitePieceCount["Bishop"] == 0 && whitePieceCount["Queen"] == 0 &&
            blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Knight"] == 0 && 
            blackPieceCount["Bishop"] == 0 && blackPieceCount["Queen"] == 0) {
            return true;
        }
        // King and Two Knights vs. King
        if (whitePieceCount["Knight"] == 2 && whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && 
            whitePieceCount["Bishop"] == 0 && whitePieceCount["Queen"] == 0 &&
            blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Knight"] == 0 && 
            blackPieceCount["Bishop"] == 0 && blackPieceCount["Queen"] == 0) {
            return true;
        }
        // Black has no pieces left, and white can potentially win
        if (blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Knight"] == 0 && 
            blackPieceCount["Bishop"] == 0 && blackPieceCount["Queen"] == 0 &&
            whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Knight"] == 0 && 
            whitePieceCount["Bishop"] == 0 && whitePieceCount["Queen"] == 0) {
            return true;
        }
        // Black has a King and a Bishop, and white has only a King
        if (blackPieceCount["Bishop"] == 1 && blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && 
            blackPieceCount["Knight"] == 0 && blackPieceCount["Queen"] == 0 &&
            whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Knight"] == 0 && 
            whitePieceCount["Bishop"] == 0 && whitePieceCount["Queen"] == 0) {
            return true;
        }
        // Black has a King and a Knight, and white has only a King
        if (blackPieceCount["Knight"] == 1 && blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && 
            blackPieceCount["Bishop"] == 0 && blackPieceCount["Queen"] == 0 &&
            whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Knight"] == 0 && 
            whitePieceCount["Bishop"] == 0 && whitePieceCount["Queen"] == 0) {
            return true;
        }
        // Black has a King and two Knights, and white has only a King
        if (blackPieceCount["Knight"] == 2 && blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && 
            blackPieceCount["Bishop"] == 0 && blackPieceCount["Queen"] == 0 &&
            whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Knight"] == 0 && 
            whitePieceCount["Bishop"] == 0 && whitePieceCount["Queen"] == 0) {
            return true;
        }
        // only two bishops
        if (whitePieceCount["Bishop"] == 1 && blackPieceCount["Bishop"] == 1 &&
            whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Knight"] == 0 &&
            whitePieceCount["Queen"] == 0 &&
            blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Knight"] == 0 &&
            blackPieceCount["Queen"] == 0) {
            return true;
        }
        // only two knights
        if (whitePieceCount["Knight"] == 1 && blackPieceCount["Knight"] == 1 &&
            whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Bishop"] == 0 &&
            whitePieceCount["Queen"] == 0 &&
            blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Bishop"] == 0 &&
            blackPieceCount["Queen"] == 0) {
            return true;
        }
        // one white bishop and one black knight
        if (whitePieceCount["Knight"] == 0 && blackPieceCount["Knight"] == 1 &&
            whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Bishop"] == 1 &&
            whitePieceCount["Queen"] == 0 &&
            blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Bishop"] == 0 &&
            blackPieceCount["Queen"] == 0) {
            return true;
        }
        // one white knight and one black bishop
        if (whitePieceCount["Knight"] == 1 && blackPieceCount["Knight"] == 0 &&
            whitePieceCount["Pawn"] == 0 && whitePieceCount["Rook"] == 0 && whitePieceCount["Bishop"] == 0 &&
            whitePieceCount["Queen"] == 0 &&
            blackPieceCount["Pawn"] == 0 && blackPieceCount["Rook"] == 0 && blackPieceCount["Bishop"] == 1 &&
            blackPieceCount["Queen"] == 0) {
            return true;
        }
    }
    return false;
}