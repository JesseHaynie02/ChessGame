#include "ChessGraphic.hpp"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == nullptr) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cout << "Renderer could not be created! SLD_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set default draw color to black
    SDL_RenderClear(renderer); // Clear the screen

    // create board
    SDL_Rect box = {0, 0, 80, 80};
    vector<boardSquare> square(64);
    bool toggle_color = 0;
    int index = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // cout << "index = " << index << endl;
            square[index].rect.x = box.x;
            square[index].rect.y = box.y;
            square[index].rect.h = box.h;
            square[index].rect.w = box.w;
            if (toggle_color) {
                SDL_SetRenderDrawColor(renderer, 75, 115, 153, 255);
                square[index].r = 75;
                square[index].g = 115;
                square[index].b = 153;
            } else {
                SDL_SetRenderDrawColor(renderer, 234, 233, 210, 255);
                square[index].r = 234;
                square[index].g = 233;
                square[index].b = 210;
            }
            SDL_RenderFillRect(renderer, &square[index].rect);
            box.x += box.w;
            if(j != 7) {
                toggle_color = !toggle_color;
            }
            ++index;
        }
        box.x = 0;
        box.y += box.h;
    }
    SDL_RenderPresent(renderer); // Present the renderer

    vector<pieceIMG> chessPiecesIMG = {
        {"images/wr.bmp", nullptr, {0,560,80,80}, WHITEP},
        {"images/wn.bmp", nullptr, {80,560,80,80}, WHITEP},
        {"images/wb.bmp", nullptr, {160,560,80,80}, WHITEP},
        {"images/wq.bmp", nullptr, {240,560,80,80}, WHITEP},
        {"images/wk.bmp", nullptr, {320,560,80,80}, WHITEP},
        {"images/wb.bmp", nullptr, {400,560,80,80}, WHITEP},
        {"images/wn.bmp", nullptr, {480,560,80,80}, WHITEP},
        {"images/wr.bmp", nullptr, {560,560,80,80}, WHITEP},
        {"images/br.bmp", nullptr, {0,0,80,80}, BLACKP},
        {"images/bn.bmp", nullptr, {80,0,80,80}, BLACKP},
        {"images/bb.bmp", nullptr, {160,0,80,80}, BLACKP},
        {"images/bq.bmp", nullptr, {240,0,80,80}, BLACKP},
        {"images/bk.bmp", nullptr, {320,0,80,80}, BLACKP},
        {"images/bb.bmp", nullptr, {400,0,80,80}, BLACKP},
        {"images/bn.bmp", nullptr, {480,0,80,80}, BLACKP},
        {"images/br.bmp", nullptr, {560,0,80,80}, BLACKP}
    };

    for (int i = 0; i < 8; ++i) {
        chessPiecesIMG.push_back({"images/wp.bmp", nullptr, {i*80,480,80,80}, WHITEP});
        chessPiecesIMG.push_back({"images/bp.bmp", nullptr, {i*80,80,80,80}, BLACKP});
    }

    for (int i = 0; i < 32; ++i) {
        SDL_Surface *surface = SDL_LoadBMP(chessPiecesIMG[i].file);
        if (surface == nullptr) {
            cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
            return 1;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);
        SDL_FreeSurface(surface);
        if (texture == nullptr) {
            cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
            return 1;
        }

        SDL_RenderCopy(renderer, texture, nullptr, &chessPiecesIMG[i].rect);
        chessPiecesIMG[i].tex = texture;
    }

    SDL_RenderPresent(renderer);

    SDL_Event eve;
    bool quit = false;
    bool dragging = false;
    int selPieceInd = -1;
    SDL_Point mousePos;
    SDL_Rect origLocOfPiece = {0,0,80,80};
    while (!quit) {
        while (SDL_PollEvent(&eve)) {
            switch (eve.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (eve.button.button == SDL_BUTTON_LEFT) {
                        mousePos.x = eve.button.x;
                        mousePos.y = eve.button.y;
                        // Check if a piece is clicked
                        for (int i = 0; i < chessPiecesIMG.size(); ++i) {
                            if (SDL_PointInRect(&mousePos, &chessPiecesIMG[i].rect)) {
                                dragging = true;
                                origLocOfPiece.x = chessPiecesIMG[i].rect.x;
                                origLocOfPiece.y = chessPiecesIMG[i].rect.y;
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                SDL_RenderClear(renderer);

                                for (int i = 0; i < 64; ++i) {
                                    SDL_SetRenderDrawColor(renderer, square[i].r, square[i].g, square[i].b, 255);            
                                    SDL_RenderFillRect(renderer, &square[i].rect);
                                }

                                // Render the chess pieces
                                vector<pieceIMG>::iterator iter = chessPiecesIMG.begin() + i;
                                rotate(iter, iter + 1, chessPiecesIMG.end());
                                for (int j = 0; j < chessPiecesIMG.size(); ++j) {
                                    // cout << "Rendering file: " << chessPiecesIMG[j].file << endl;
                                    SDL_RenderCopy(renderer, chessPiecesIMG[j].tex, nullptr, &chessPiecesIMG[j].rect);
                                }

                                SDL_RenderPresent(renderer);
                                selPieceInd = chessPiecesIMG.size() - 1;
                                break;
                            }
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (eve.button.button == SDL_BUTTON_LEFT) {
                        // cout << "Button up det" << endl;
                        // cout << "mousePos: " << mousePos.x << " and " << mousePos.y << endl;
                        dragging = false;
                        mousePos.x = eve.motion.x;
                        mousePos.y = eve.motion.y;
                        for (int i = 0; i < 64; ++i) {
                            if (SDL_PointInRect(&mousePos, &square[i].rect)) {
                                SDL_Rect temp = square[i].rect;
                                // cout << square[i].rect.x << " and " << square[i].rect.y << endl;
                                for (int j = 0; j < chessPiecesIMG.size(); ++j) {
                                    // cout << "Looking for piece take" << endl;
                                    // cout << square[i].rect.x << " and " << square[i].rect.y << endl;
                                    // cout << chessPiecesIMG[j].rect.x << " and " << chessPiecesIMG[j].rect.y << endl;
                                    if (square[i].rect.x == chessPiecesIMG[j].rect.x && square[i].rect.y == chessPiecesIMG[j].rect.y) {
                                        // cout << "Found piece on square" << endl;
                                        if (chessPiecesIMG[selPieceInd].color != chessPiecesIMG[j].color) {
                                            // cout << "Detected take" << endl;
                                            chessPiecesIMG[selPieceInd].rect = square[i].rect;
                                            SDL_DestroyTexture(chessPiecesIMG[j].tex);
                                            chessPiecesIMG.erase(chessPiecesIMG.begin() + j);
                                            break;
                                        } else if (chessPiecesIMG[selPieceInd].color == chessPiecesIMG[j].color) {
                                            // cout << "Same color piece detected" << endl;
                                            temp = origLocOfPiece;
                                            break;
                                        }
                                    } else if (j == chessPiecesIMG.size() - 1) {
                                        chessPiecesIMG[selPieceInd].rect = square[i].rect;
                                        break;
                                    }
                                }
                                chessPiecesIMG[selPieceInd].rect = temp;
                                break;
                            } else {
                                // cout << "Out of bounds mouse pos" << endl;
                                chessPiecesIMG[selPieceInd].rect = origLocOfPiece;
                            }
                        }
                        selPieceInd = -1;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (dragging && selPieceInd != -1) {
                        if ((eve.motion.x >= 0 && eve.motion.x <= 640)) {
                            chessPiecesIMG[selPieceInd].rect.x = eve.motion.x - chessPiecesIMG[selPieceInd].rect.w / 2;
                            // cout << "eve.motion.x = " << eve.motion.x - chessPiecesIMG[selPieceInd].rect.w / 2 << endl;
                        }
                        if ((eve.motion.y >= 0 && eve.motion.y <= 640)) {
                            chessPiecesIMG[selPieceInd].rect.y = eve.motion.y - chessPiecesIMG[selPieceInd].rect.h / 2;
                            // cout << "eve.motion.y = " << eve.motion.y << endl;
                        }
                    }
                    break;

                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < 64; ++i) {
            SDL_SetRenderDrawColor(renderer, square[i].r, square[i].g, square[i].b, 255);            
            SDL_RenderFillRect(renderer, &square[i].rect);
        }

        // Render the chess pieces
        for (const auto& piece : chessPiecesIMG) {
            SDL_RenderCopy(renderer, piece.tex, nullptr, &piece.rect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}