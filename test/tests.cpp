#include <gtest/gtest.h>
#include <fstream>
#include <regex>
#include "../ChessGame.hpp"
#include "../ChessBoard.hpp"
#include "../ChessPiece.hpp"

TEST(mainTest, TestFullGame) {
    ifstream ifs;
    Winner winner;
    string curGame, decision;
    regex patternOne(R"(\d+\.\s*)");
    ifs.open("test/testGames.txt", ifstream::in);
    ASSERT_EQ(ifs.is_open(), true) << "failed to open file";

    cout << "Printing games" << endl;
    // for (int i = 0; i < 8; ++i) {
    //     cout << "i = " << i << endl;
    //     getline(ifs, curGame);
        while (getline(ifs, curGame)) {
            if (curGame[0] == '1') {
                // cout << curGame << endl;
                if (curGame.find("forfeits") != string::npos || curGame.find("resigns") != string::npos ||
                    curGame.find("time") != string::npos || curGame.find("agreement") != string::npos || 
                    curGame.find("adjudication") != string::npos) {
                    // cout << "skipping game:\n" << curGame << endl;
                    curGame.clear();
                    continue;
                }
                decision = curGame.substr(curGame.size() - 3, 3);
                // cout << decision << endl;
                if (decision == "1-0") {
                    // cout << "White win detected: " << decision << endl;
                    winner = WHITE_WINS;
                } else if (decision == "0-1") {
                    // cout << "Black win detected: " << decision << endl;
                    winner = BLACK_WINS;
                } else {
                    // cout << "Draw detected: " << decision << endl; 
                    winner = DRAW;
                }
                curGame = regex_replace(curGame, patternOne, "");
                // cout << curGame << endl;
                istringstream curGameStream(curGame);
                EXPECT_EQ(playChess(curGameStream), winner) << "failed chess game: ";
            }
        }
    // }

    ifs.close();
    ASSERT_EQ(ifs.is_open(), false) << "failed to close file";
}