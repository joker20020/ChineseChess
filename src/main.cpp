#include <ctime>
#include <cstdlib>
#include "piece.h"
#include "mcts.h"
#include "game.h"


int main() {
    ChessBoard board = ChessBoard();
    MCTSAI ai = MCTSAI(board, RED);
    ChessGame game = ChessGame(&board, ai);
    game.Start();
    return 0;
    // srand(time(nullptr));

    // ChessBoard board = ChessBoard("out chessboard");
    // board.InitializeBoard();

    // MCTSAI ai = MCTSAI(board, RED);
    // ai.Run(1000); // 运行 1000 次模拟

    // auto bestMove = ai.GetBestMove();
    // cout << "最佳移动: (" << bestMove.first.first << ", " << bestMove.first.second << ") -> ("
    //      << bestMove.second.first << ", " << bestMove.second.second << ")" << endl;
    // // ai.AutoUpdate();
    // ai.Update(bestMove);
    // ai.root->Print();

    return 0;
}