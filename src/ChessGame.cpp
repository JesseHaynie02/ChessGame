#include "ChessGame.hpp"

Winner playChess(istream& input) {
    cout << "Chess Game" << endl;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    vector<boardSquare> squares(64);
    map<int,unique_ptr<pieceIMG>> pieceIMGs;

    initializeSDL();
    initWinRen(window, renderer);
    initializeGraphic(renderer, squares, pieceIMGs);

    ChessBoard board;
    gameState state;
    Winner result = ERROR;

    while (!board.getGameOver() && !board.getDraw() && !state.quit) {
        // board.printPieces();
        while (SDL_PollEvent(&state.event)) {
            switch (state.event.type) {
                case SDL_QUIT:
                    state.quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:

                    mouseButtonDown(renderer, board, state, squares, pieceIMGs);
                    break;

                case SDL_MOUSEBUTTONUP:

                    mouseButtonUp(renderer, board, state, squares, pieceIMGs);
                    resetState(state);
                    break;

                case SDL_MOUSEMOTION:

                    mouseMotion(state, squares, pieceIMGs);
                    break;

                default:
                    break;
            }
        }
        refreshGame(renderer, squares, pieceIMGs);
        map<int,unique_ptr<pieceIMG>>::iterator IMG = pieceIMGs.find(state.selPieceInd);
        if (IMG != pieceIMGs.end()) {
            SDL_RenderCopy(renderer, IMG->second->tex, nullptr, &IMG->second->rect);
        }
        SDL_RenderPresent(renderer);
    }

    if (board.getGameOver()) {
        cout << "Game Over: " << ((state.turn == WHITE) ? "Black Won" : "White Won") << endl;
        result = ((state.turn == WHITE) ? BLACK_WINS : WHITE_WINS);
    } else if (board.getDraw()) {
        cout << "Game is a Draw" << endl;
        result = DRAW;
    }
    destroyGUI(window, renderer, pieceIMGs);
    return result;
}

// for command line chess
string getNextMove(Color color, istream& input) {
    string nextMove;

    (color == WHITE) ? cout << "White to Move: ": cout << "Black to Move: ";
    input >> nextMove;
    cin.sync();

    return nextMove;
}