#include "ChessGame.hpp"
#include "ChessGraphic.hpp"

int main(int argc, char* argv[]) {
    // // code to test code coverage
    // ifstream ifs;
    // string curGame;
    // regex patternOne(R"(\d+\.\s*)");
    // ifs.open("testChessGame/testGames.txt", ifstream::in);

    // while (getline(ifs, curGame)) {
    //     if (curGame[0] == '1') {
    //         // cout << curGame << endl;
    //         if (curGame.find("forfeits") != string::npos || curGame.find("resigns") != string::npos ||
    //             curGame.find("time") != string::npos || curGame.find("agreement") != string::npos || 
    //             curGame.find("adjudication") != string::npos) {
    //             // cout << "skipping game:\n" << curGame << endl;
    //             curGame.clear();
    //             continue;
    //         }
    //         curGame = regex_replace(curGame, patternOne, "");
    //         // cout << curGame << endl;
    //         istringstream curGameStream(curGame);
    //         playChess(curGameStream);
    //     }
    // }

    // ifs.close();

    // actual program
    playChess(cin);
    return 0;
}