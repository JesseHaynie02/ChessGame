#include "ChessGraphic.hpp"

void initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
    }
}

void initWinRen(SDL_Window *&window, SDL_Renderer *&renderer) {
    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == nullptr) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Renderer could not be created! SLD_Error: " << SDL_GetError() << endl;
    }
}

void initializeGraphic(SDL_Renderer *renderer, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs) {

    bool toggle_color = 0;
    int index = 0;
    SDL_Rect box = {0, 560, 80, 80};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            squares[index].position = index + 1;
            squares[index].rect.x = box.x;
            squares[index].rect.y = box.y;
            squares[index].rect.h = box.h;
            squares[index].rect.w = box.w;

            if (toggle_color) {
                SDL_SetRenderDrawColor(renderer, 75, 115, 153, 255);
                squares[index].r = 75;
                squares[index].g = 115;
                squares[index].b = 153;
            } else {
                SDL_SetRenderDrawColor(renderer, 234, 233, 210, 255);
                squares[index].r = 234;
                squares[index].g = 233;
                squares[index].b = 210;
            }

            SDL_RenderFillRect(renderer, &squares[index++].rect);

            box.x += box.w;
            if(j != 8) {
                toggle_color = !toggle_color;
            }
        }
        box.x = 0;
        box.y -= box.h;
    }

    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(1, make_unique<pieceIMG>("images/wr.bmp", nullptr, WHITE, "Rook", SDL_Rect{0,560,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(2, make_unique<pieceIMG>("images/wn.bmp", nullptr, WHITE, "Knight", SDL_Rect{80,560,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(3, make_unique<pieceIMG>("images/wb.bmp", nullptr, WHITE, "Bishop", SDL_Rect{160,560,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(4, make_unique<pieceIMG>("images/wq.bmp", nullptr, WHITE, "Queen", SDL_Rect{240,560,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(5, make_unique<pieceIMG>("images/wk.bmp", nullptr, WHITE, "King", SDL_Rect{320,560,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(6, make_unique<pieceIMG>("images/wb.bmp", nullptr, WHITE, "Bishop", SDL_Rect{400,560,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(7, make_unique<pieceIMG>("images/wn.bmp", nullptr, WHITE, "Knight", SDL_Rect{480,560,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(8, make_unique<pieceIMG>("images/wr.bmp", nullptr, WHITE, "Rook", SDL_Rect{560,560,80,80})));

    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(57, make_unique<pieceIMG>("images/br.bmp", nullptr, BLACK, "Rook", SDL_Rect{0,0,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(58, make_unique<pieceIMG>("images/bn.bmp", nullptr, BLACK, "Knight", SDL_Rect{80,0,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(59, make_unique<pieceIMG>("images/bb.bmp", nullptr, BLACK, "Bishop", SDL_Rect{160,0,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(60, make_unique<pieceIMG>("images/bq.bmp", nullptr, BLACK, "Queen", SDL_Rect{240,0,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(61, make_unique<pieceIMG>("images/bk.bmp", nullptr, BLACK, "King", SDL_Rect{320,0,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(62, make_unique<pieceIMG>("images/bb.bmp", nullptr, BLACK, "Bishop", SDL_Rect{400,0,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(63, make_unique<pieceIMG>("images/bn.bmp", nullptr, BLACK, "Knight", SDL_Rect{480,0,80,80})));
    pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(64, make_unique<pieceIMG>("images/br.bmp", nullptr, BLACK, "Rook", SDL_Rect{560,0,80,80})));

    for (int i = 1; i <= 8; ++i) {
        pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(i + 8, make_unique<pieceIMG>("images/wp.bmp", nullptr, WHITE, "Pawn", SDL_Rect{80 * (i - 1),480,80,80})));
        pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(i + 48, make_unique<pieceIMG>("images/bp.bmp", nullptr, BLACK, "Pawn", SDL_Rect{80 * (i - 1),80,80,80})));
    }

    for (map<int,unique_ptr<pieceIMG>>::iterator IMG = pieceIMGs.begin(); IMG != pieceIMGs.end(); ++IMG) {
        if (IMG->second->file == nullptr) {
            continue;
        }

        SDL_Surface *surface = SDL_LoadBMP(IMG->second->file);
        if (surface == nullptr) {
            cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);
        SDL_FreeSurface(surface);
        if (texture == nullptr) {
            cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
        }

        SDL_RenderCopy(renderer, texture, nullptr, &IMG->second->rect);
        IMG->second->tex = texture;
    }

    SDL_RenderPresent(renderer);
}

void refreshGame(SDL_Renderer *renderer, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (vector<boardSquare>::iterator square = squares.begin(); square != squares.end(); ++square) {
        SDL_SetRenderDrawColor(renderer, square->r, square->g, square->b, 255);            
        SDL_RenderFillRect(renderer, &square->rect);
    }
    for (map<int,unique_ptr<pieceIMG>>::iterator IMG = pieceIMGs.begin(); IMG != pieceIMGs.end(); ++IMG) {
        if (IMG->second->file == nullptr) {
            continue;
        }
        SDL_RenderCopy(renderer, IMG->second->tex, nullptr, &IMG->second->rect);
    }
}

void destroyGUI(SDL_Window *&window, SDL_Renderer *&renderer, map<int,unique_ptr<pieceIMG>> &pieceIMGs) {
    for (map<int,unique_ptr<pieceIMG>>::iterator piece = pieceIMGs.begin(); piece != pieceIMGs.end(); ++piece) {
        if (piece->second->file != nullptr) {
            SDL_DestroyTexture(piece->second->tex);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void mouseButtonDown(SDL_Renderer *&renderer, ChessBoard &board, gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs) {
    if (state.event.button.button == SDL_BUTTON_LEFT) {
        state.mousePos.x = state.event.button.x;
        state.mousePos.y = state.event.button.y;
        // Check if a piece is clicked
        for (map<int,unique_ptr<pieceIMG>>::iterator currentIMG = pieceIMGs.begin(); currentIMG != pieceIMGs.end(); ++currentIMG) {
            if (SDL_PointInRect(&state.mousePos, &currentIMG->second->rect)) {
                cout << "Piece clicked: " << currentIMG->second->pieceName << endl;
                // if (currentIMG->second->color != state.turn) {
                //     return;
                // }

                cout << "Position = " << currentIMG->first << endl;
                state.moves = board.getMovesGraphic(currentIMG->first);
                cout << "moves: ";
                for (auto &iter : state.moves) {
                    cout << iter << " ";
                }
                cout << endl;
                state.origLoc = currentIMG->first;
                state.pieceMoved = currentIMG->second->pieceName;
                state.dragging = true;
                state.origLocRect.x = currentIMG->second->rect.x;
                state.origLocRect.y = currentIMG->second->rect.y;
                state.selPieceInd = currentIMG->first;

                refreshGame(renderer, squares, pieceIMGs);
                SDL_RenderCopy(renderer, currentIMG->second->tex, nullptr, &currentIMG->second->rect);
                SDL_RenderPresent(renderer);
                cout << "at end of mouseButtonDown" << endl;

                break;
            } else {
                cout << "empty square clicked" << endl;
                state.selPieceInd = -1;
            }
        }
    }
}

void mouseMotion(gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs) {
    if (state.dragging && state.selPieceInd != -1) {
        if ((state.event.motion.x >= 0 && state.event.motion.x <= 640)) {
            pieceIMGs.at(state.selPieceInd)->rect.x = state.event.motion.x - pieceIMGs.at(state.selPieceInd)->rect.w / 2;
        }
        if ((state.event.motion.y >= 0 && state.event.motion.y <= 640)) {
            pieceIMGs.at(state.selPieceInd)->rect.y = state.event.motion.y - pieceIMGs.at(state.selPieceInd)->rect.h / 2;
        }
    }
}

void resetState(gameState &state) {
    state.selPieceInd = -1;
    state.moves.clear();
    state.origLoc = 0;
    state.newLoc = 0;
    state.pieceTakenLoc = 0;
    state.isPieceTake = false;
    state.pieceMoved = "";
    state.pieceTaken = "";
}

bool castle(SDL_Renderer *&renderer, ChessBoard &board, gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs) {
    cout << state.origLocRect.x << " and " << state.origLocRect.y << endl;
    bool result = false;
    string move = "";
    cout << "in castle" << endl;
    int rookLoc, rookDiff;
    int kingLoc = (state.turn == WHITE ? 5 : 61);
    if (state.newLoc == 3 || state.newLoc == 59) {
        move = "O-O-O";
    } else if (state.newLoc == 7 || state.newLoc == 63) {
        move = "O-O";
    }
    if (state.turn == WHITE) {
        rookLoc = (move == "O-O" ? 8 : 1);
        rookDiff = (move == "O-O" ? -2 : 3);
    } else if (state.turn == BLACK) {
        rookLoc = (move == "O-O" ? 64 : 57);
        rookDiff = (move == "O-O" ? -2 : 3);
    }
    map<int,unique_ptr<pieceIMG>>::iterator kingIMG = pieceIMGs.find(kingLoc);
    map<int,unique_ptr<pieceIMG>>::iterator rookIMG = pieceIMGs.find(rookLoc);

    if (state.newLoc == 3 || state.newLoc == 7 || state.newLoc == 59 || state.newLoc == 63) {
        cout << "Accessing king piece" << endl;
        int kingDiff;
        kingDiff = (move == "O-O" ? 2 : -2);
        if (state.turn == WHITE) {
            rookLoc = (move == "O-O" ? 8 : 1);
            rookDiff = (move == "O-O" ? -2 : 3);
        } else if (state.turn == BLACK) {
            rookLoc = (move == "O-O" ? 64 : 57);
            rookDiff = (move == "O-O" ? -2 : 3);
        }
        bool clearToCastle = board.clearToCastle(move, state.turn);
        if (clearToCastle && board.castle(move, kingLoc, rookLoc, state.turn)) {
            cout << "Succesful castle" << endl;
            // map<int,unique_ptr<pieceIMG>>::iterator kingIMG = pieceIMGs.find(kingLoc);
            // if (kingIMG == pieceIMGs.end()) {
            //     cout << "didnt find king" << endl;
            // }
            // cout << kingIMG->second->rect.x << " and " << kingIMG->second->rect.y << endl;
            kingIMG->second->rect.x = state.origLocRect.x + (80 * kingDiff);
            kingIMG->second->rect.y = state.origLocRect.y;
            rookIMG->second->rect.x += (80 * rookDiff);
            auto movePiece = pieceIMGs.extract(kingIMG->first);
            movePiece.key() = kingLoc + kingDiff;
            pieceIMGs.insert(std::move(movePiece));
            movePiece = pieceIMGs.extract(rookIMG->first);
            movePiece.key() = rookLoc + rookDiff;
            pieceIMGs.insert(std::move(movePiece));
            state.turn = (state.turn == WHITE) ? BLACK : WHITE;
            result = true;
            // cout << kingIMG->second->rect.x << endl;
        }
    }
    return result;
}

void mouseButtonUp(SDL_Renderer *&renderer, ChessBoard &board, gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs) {
    if (state.event.button.button == SDL_BUTTON_LEFT && state.selPieceInd >= 0) {
        state.dragging = false;
        state.mousePos.x = state.event.motion.x;
        state.mousePos.y = state.event.motion.y;
        for (auto IMG = pieceIMGs.begin(); IMG != pieceIMGs.end(); ++IMG) {
            cout << IMG->first << ": " << IMG->second->rect.x << " " << IMG->second->rect.y << endl;
        }

        map<int,unique_ptr<pieceIMG>>::iterator currentIMG = pieceIMGs.find(state.selPieceInd);
        if (currentIMG == pieceIMGs.end()) {
            cout << "currentIMG cant be found" << endl;
            return;
        } else if (currentIMG != pieceIMGs.end() && state.turn != currentIMG->second->color) {
            cout << "same color piece found" << endl;
            currentIMG->second->rect.x = state.origLocRect.x;
            currentIMG->second->rect.y = state.origLocRect.y;
            return;
        }

        cout << currentIMG->second->rect.x << " and " << currentIMG->second->rect.y << endl;
        for (vector<boardSquare>::iterator square = squares.begin(); square != squares.end(); ++square) {
            if (SDL_PointInRect(&state.mousePos, &square->rect)) {
                state.newLoc = square->position;
                // cout << "new location = " << state.newLoc << endl;
                // cout << "piece moved = " << state.pieceMoved << endl;
                if (state.moves.find(state.newLoc) == state.moves.end()) {
                    cout << "in case no move found\nstate.turn = " << state.turn << "\ncurrentIMG->second->color = " << currentIMG->second->color << endl;
                    // currentIMG->second->rect = state.origLocRect;
                    if (state.pieceMoved == "King" && state.turn == currentIMG->second->color) {
                        if (!castle(renderer, board, state, squares, pieceIMGs)) {
                            currentIMG->second->rect.x = state.origLocRect.x;
                            currentIMG->second->rect.y = state.origLocRect.y; 
                        }
                        break;
                    }
                    currentIMG->second->rect.x = state.origLocRect.x;
                    currentIMG->second->rect.y = state.origLocRect.y;
                    break;
                }
                map<int,unique_ptr<pieceIMG>>::iterator IMG = pieceIMGs.find(square->position);
                if (IMG != pieceIMGs.end() && IMG->second->color != currentIMG->second->color) {
                    cout << "piece take detected" << endl;
                    state.isPieceTake = true;
                    state.pieceTaken = IMG->second->pieceName;
                    state.pieceTakenLoc = square->position;
                    SDL_DestroyTexture(IMG->second->tex);
                    pieceIMGs.erase(IMG);
                    state.selPieceInd = square->position;
                    currentIMG->second->rect = square->rect;
                }
                if (IMG == pieceIMGs.end() && !state.isPieceTake) {
                    // not piece take can move to empty square
                    if (currentIMG->second->pieceName == "Pawn") {
                        map<int,unique_ptr<pieceIMG>>::iterator enPassPawn = pieceIMGs.find(square->position + ((state.turn == WHITE ? -1 : 1) * 8));
                        map<int,unique_ptr<pieceIMG>>::iterator emptySquareCheck = pieceIMGs.find(state.newLoc);
                        if (enPassPawn != pieceIMGs.end() && enPassPawn->second->pieceName == "Pawn" && enPassPawn->second->color != state.turn &&
                            enPassPawn->second->rect.x == square->rect.x && emptySquareCheck == pieceIMGs.end()) {
                                cout << "en pessant detected" << endl;
                                state.isPieceTake = true;
                                state.pieceTaken = enPassPawn->second->pieceName;
                                state.pieceTakenLoc = enPassPawn->first;
                                SDL_DestroyTexture(enPassPawn->second->tex);
                                pieceIMGs.erase(enPassPawn);
                                state.selPieceInd = enPassPawn->first;
                                currentIMG->second->rect = square->rect;
                        }
                    }
                    cout << "not piece take" << endl;
                    currentIMG->second->rect.x = square->rect.x;
                    currentIMG->second->rect.y = square->rect.y;
                } else if (IMG != pieceIMGs.end() && !state.isPieceTake) {
                    cout << "trying to take same color piece" << endl;
                    currentIMG->second->rect = state.origLocRect;
                }
                cout << "origLoc = " << state.origLoc << " newLoc = " << state.newLoc << " turn = " << state.turn << " color of piece = " << currentIMG->second->color << endl;

                // cout << "checking if pawnPromotion: " << state.pieceMoved << " " << board.gridItos.at(state.newLoc) << " " << state.turn << endl;
                if (board.isPawnPromotion(state.pieceMoved, board.gridItos.at(state.newLoc), state.turn)) { 
                    cout << "Pawn promotion detected" << endl;
                    currentIMG->second->rect = square->rect;
                    string piecePromo = graphicPawnPromotion(renderer, state, squares, pieceIMGs, square->rect);
                    cout << "after graphicPawnPromo" << endl;
                    if (currentIMG->second == nullptr) {
                        // cout << "currentIMG deleted" << endl;
                        currentIMG = pieceIMGs.find(state.newLoc);
                    } else {
                        // cout << "currentIMG not deleted" << endl;
                    }
                    if (!(currentIMG->second->rect.x == state.origLocRect.x && currentIMG->second->rect.y == state.origLocRect.y)) {
                        cout << "promo worked" << endl;

                        board.moveDo(state.origLoc, state.newLoc, piecePromo, state.isPieceTake, state.pieceTakenLoc, state.pieceTaken, state.turn);
                        if (board.isCheck(state.turn)) {
                            board.moveUndo(state.origLoc, state.newLoc, "Pawn", state.pieceTakenLoc, state.pieceTaken, state.turn);
                        } else {
                            auto movePiece = pieceIMGs.extract(currentIMG->first);
                            movePiece.key() = square->position;
                            pieceIMGs.insert(move(movePiece));
                            board.checkEnPassant(state.origLoc, state.newLoc, state.turn);
                            board.checkCastling(state.pieceMoved, state.pieceTaken, state.origLoc, state.newLoc, state.turn);
                            if (board.isCheckMate(state.turn == WHITE ? BLACK : WHITE)) {
                                board.setGameOver(true);
                            } else if (board.isDraw(state.turn)) {
                                board.setDraw(true);
                            }
                            state.turn = (state.turn == WHITE) ? BLACK : WHITE;
                        }
                    } else {
                        // cout << "promo did not work" << endl;
                    }
                    break;
                } else if (state.newLoc != state.origLoc && state.turn == currentIMG->second->color) {
                    cout << "origLoc: " << state.origLoc << " newLoc: " << state.newLoc << " pieceMoved: " << state.pieceMoved << " pieceTakenLoc: " << state.pieceTakenLoc << " pieceTaken: " << state.pieceTaken << " turn: " << state.turn << endl;
                    // board.moveDo(state.origLoc, state.newLoc, state.pieceMoved, state.isPieceTake, state.pieceTakenLoc, state.pieceTaken, state.turn);
                    // auto movePiece = pieceIMGs.extract(currentIMG->first);
                    // movePiece.key() = square->position;
                    // pieceIMGs.insert(move(movePiece));

                    board.moveDo(state.origLoc, state.newLoc, state.pieceMoved, state.isPieceTake, state.pieceTakenLoc, state.pieceTaken, state.turn);
                    if (board.isCheck(state.turn)) {
                        board.moveUndo(state.origLoc, state.newLoc, state.pieceMoved, state.pieceTakenLoc, state.pieceTaken, state.turn);
                    } else {
                        auto movePiece = pieceIMGs.extract(currentIMG->first);
                        movePiece.key() = square->position;
                        pieceIMGs.insert(move(movePiece));
                        board.checkEnPassant(state.origLoc, state.newLoc, state.turn);
                        board.checkCastling(state.pieceMoved, state.pieceTaken, state.origLoc, state.newLoc, state.turn);
                        if (board.isCheckMate(state.turn == WHITE ? BLACK : WHITE)) {
                            board.setGameOver(true);
                        } else if (board.isDraw(state.turn)) {
                            board.setDraw(true);
                        }
                        state.turn = (state.turn == WHITE) ? BLACK : WHITE;
                    }
                }
                break;
            } else {
                currentIMG->second->rect = state.origLocRect;
            }
        }
    }
}

string graphicPawnPromotion(SDL_Renderer *renderer, gameState &state, vector<boardSquare> &squares, map<int,unique_ptr<pieceIMG>> &pieceIMGs, SDL_Rect row) {
    // cout << "in graphicPawnPromotion" << endl;
    string piecePromo = "";
    SDL_Event event;
    SDL_Point mousePos;
    bool quit = false, canceled = false;
    // int direction = (state.turn == WHITE ? -80 : 80);
    vector<pieceIMG> promoPieces(4);
    vector<boardSquare> promoSquares(4);

    if (state.turn == WHITE) {
        promoPieces[0] = pieceIMG{"images/wq.bmp", nullptr, WHITE, "Queen", {row.x, row.y, 80, 80}};
        promoPieces[1] = pieceIMG{"images/wn.bmp", nullptr, WHITE, "Knight", {row.x, row.y + 80, 80, 80}};
        promoPieces[2] = pieceIMG{"images/wr.bmp", nullptr, WHITE, "Rook", {row.x, row.y + 160, 80, 80}};
        promoPieces[3] = pieceIMG{"images/wb.bmp", nullptr, WHITE, "Bishop", {row.x, row.y + 240, 80, 80}};
    } else {
        promoPieces[0] = pieceIMG{"images/bq.bmp", nullptr, BLACK, "Queen", {row.x, row.y, 80, 80}};
        promoPieces[1] = pieceIMG{"images/bn.bmp", nullptr, BLACK, "Knight", {row.x, row.y - 80, 80, 80}};
        promoPieces[2] = pieceIMG{"images/br.bmp", nullptr, BLACK, "Rook", {row.x, row.y - 160, 80, 80}};
        promoPieces[3] = pieceIMG{"images/bb.bmp", nullptr, BLACK, "Bishop", {row.x, row.y - 240, 80, 80}};
    }

    refreshGame(renderer, squares, pieceIMGs);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 4; ++i) {
        int direction = (state.turn == WHITE ? 1 : -1); 

        promoSquares[i].rect.h = 80;
        promoSquares[i].rect.w = 80;
        promoSquares[i].rect.x = row.x;
        promoSquares[i].rect.y = row.y + (direction * (80 * i));

        promoSquares[i].r = 255;
        promoSquares[i].g = 255;
        promoSquares[i].b = 255;

        SDL_RenderFillRect(renderer, &promoSquares[i].rect);

        SDL_Surface *surface = SDL_LoadBMP(promoPieces[i].file);
        if (surface == nullptr) {
            cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);
        SDL_FreeSurface(surface);
        if (texture == nullptr) {
            cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
        }

        SDL_RenderCopy(renderer, texture, nullptr, &promoPieces[i].rect);
        promoPieces[i].tex = texture;
    }

    SDL_RenderPresent(renderer);

    // SDL_Delay(2000);
    
    while (!quit) {
        refreshGame(renderer, squares, pieceIMGs);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < 4; ++i) {
            SDL_RenderFillRect(renderer, &promoSquares[i].rect);

            if (promoPieces[i].file != "") {
                SDL_RenderCopy(renderer, promoPieces[i].tex, nullptr, &promoPieces[i].rect);
            }
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mousePos.x = event.button.x;
                        mousePos.y = event.button.y;
                        // for (int j = 0; j < promoSquares.size(); ++j) {
                            // if (promoSquares[j].rect.x == row.x && promoSquares[j].rect.y == row.y) {
                                for (int i = 0; i < 4; ++i) {
                                    if (SDL_PointInRect(&mousePos, &promoPieces[i].rect)) {
                                        cout << "piece promo detected" << endl;
                                        // cout << "Piece chosen: " << pawnPromoSel[i].piece << endl;
                                        string file = "images/";
                                        file += (state.turn == WHITE ? "w" : "b");
                                        file += tolower((promoPieces[i].pieceName == "Knight" ? promoPieces[i].pieceName[1] : promoPieces[i].pieceName[0]), locale());
                                        file += ".bmp";
                                        const char *filec = file.c_str();
                                        cout << "filec = " << filec << endl;
                                        // pieceIMGs.push_back({filec, nullptr, {row.x,row.y,80,80}, color, pawnPromoSel[i].piece});
                                        cout << "state.newLoc: " << state.origLoc << endl;
                                        SDL_DestroyTexture(pieceIMGs[state.origLoc]->tex);
                                        cout << "after deleting texture" << endl;
                                        map<int,unique_ptr<pieceIMG>>::iterator pawnGettingPromoted = pieceIMGs.find(state.selPieceInd);
                                        cout << "state.selPieceInd: " << state.selPieceInd << endl;
                                        cout << "deleting pawn: " << pawnGettingPromoted->first << " " << pawnGettingPromoted->second->rect.x << " " << pawnGettingPromoted->second->rect.y << endl;
                                        if (pawnGettingPromoted != pieceIMGs.end()) {
                                            pieceIMGs.erase(pawnGettingPromoted);
                                        }
                                        cout << "deleting pawn piece thats getting promoted" << endl;
                                        cout << "promoPieces[i].pieceName: " << promoPieces[i].pieceName << endl;
                                        cout << "state.newLoc: " << state.newLoc << endl;
                                        pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(state.newLoc, 
                                                         make_unique<pieceIMG>(filec, nullptr, state.turn, promoPieces[i].pieceName, SDL_Rect{row.x,row.y,80,80})));
                                        cout << "after inserting piece" << endl;

                                        // SDL_DestroyTexture(pieceIMGs[state.selPieceInd]->tex);
                                        // map<int,unique_ptr<pieceIMG>>::iterator pawnGettingPromoted = pieceIMGs.find(state.selPieceInd);
                                        // if (pawnGettingPromoted != pieceIMGs.end()) {
                                        //     pieceIMGs.erase(pawnGettingPromoted);
                                        // }
                                        map<int,unique_ptr<pieceIMG>>::iterator newPiece = pieceIMGs.find(state.newLoc);
                                        SDL_Surface *surface = SDL_LoadBMP(newPiece->second->file);
                                        if (surface == nullptr) {
                                            cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
                                        }
                                        cout << "after creating texture" << endl;

                                        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                                        SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);
                                        SDL_FreeSurface(surface);
                                        if (texture == nullptr) {
                                            cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
                                        }

                                        SDL_RenderCopy(renderer, texture, nullptr, &newPiece->second->rect);
                                        newPiece->second->tex = texture;
                                        refreshGame(renderer, squares, pieceIMGs);
                                        SDL_RenderPresent(renderer);
                                        quit = true;
                                        canceled = false;
                                        piecePromo = newPiece->second->pieceName;
                                        break;
                                    } else {
                                        quit = true;
                                        canceled = true;
                                    }
                                }
                                if (quit && canceled) {
                                    cout << "piece not selected" << endl;
                                    cout << pieceIMGs[state.origLoc]->rect.x << " " << pieceIMGs[state.origLoc]->rect.y << "\n" << state.origLocRect.x << " " << state.origLocRect.y << endl;
                                    // pieceIMGs[]
                                    cout << pieceIMGs[state.origLoc]->pieceName << endl;
                                    pieceIMGs[state.origLoc]->rect = state.origLocRect;
                                    // SDL_Delay(2000);
                                    // cout << pieceIMGs[j].rect.x << " " << pieceIMGs[j].rect.y << "\n" << ogLoc.x << " " << ogLoc.y << endl;
                                    if (state.isPieceTake) {
                                        string file = "images/";
                                        file += (state.turn == WHITE ? "b" : "w");
                                        file += tolower((state.pieceTaken == "Knight" ? state.pieceTaken[1] : state.pieceTaken[0]), locale());
                                        file += ".bmp";
                                        const char *filec = file.c_str();
                                        cout << "filec = " << filec << endl;
                                        // pieceIMGs.push_back({filec, nullptr, {row.x,row.y,80,80}, color, pieceTaken});
                                        pieceIMGs.insert(pair<int,unique_ptr<pieceIMG>>(state.pieceTakenLoc, 
                                                         make_unique<pieceIMG>(filec, nullptr, (state.turn == WHITE ? BLACK : WHITE), state.pieceTaken, SDL_Rect{row.x,row.y,80,80})));
                                        map<int,unique_ptr<pieceIMG>>::iterator oldPiece = pieceIMGs.find(state.pieceTakenLoc);
                                        SDL_Surface *surface = SDL_LoadBMP(oldPiece->second->file);
                                        if (surface == nullptr) {
                                            cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
                                        }

                                        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                                        SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);
                                        SDL_FreeSurface(surface);
                                        if (texture == nullptr) {
                                            cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
                                        }

                                        SDL_RenderCopy(renderer, texture, nullptr, &oldPiece->second->rect);
                                        oldPiece->second->tex = texture;
                                        break;
                                    }
                                }
                            } else {
                                quit = true;
                            }
                        // }
                    // }
                    break;

                default:
                    break;
            }
        }
    }
    // cout << "leaving pawn promo" << endl;
    for (int i = 0; i < 4; ++i) {
        SDL_DestroyTexture(promoPieces[i].tex);
    }
    return piecePromo;
}