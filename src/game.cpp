#include "game.h"

ChessGame::~ChessGame(){
    delete this->board;
}

ChessGame::ChessGame(ChessBoard *board, MCTSAI& ai){
    this->currentPlayer = RED;
    this->ai = ai;
    this->aiColor = ai.root->currentPlayer;
    this->board = board;
}

void ChessGame::Start() {
    while (true) {
        board->Print(true);
        cout << (currentPlayer == RED ? "红方" : "黑方") << "的回合" << endl;
        if (currentPlayer == aiColor)
        {
            srand(time(nullptr));

            ai.Run(2000); // 运行 1000 次模拟

            auto bestMove = ai.GetBestMove();
            cout << "最佳移动: (" << bestMove.first.first << ", " << bestMove.first.second << ") -> ("
                << bestMove.second.first << ", " << bestMove.second.second << ")" << endl;
            // ai.AutoUpdate();
            ai.Update(bestMove);
            currentPlayer = (currentPlayer == RED) ? BLACK : RED;
            board->MovePiece(bestMove.first.first, bestMove.first.second, bestMove.second.first, bestMove.second.second);
            // ai.root->Print();
        }
        else{
            string input;
            cout << "输入移动（例如 a0 a1）：";
            getline(cin, input);

            auto positions = ParseInput(input);
            if (positions.size() != 2) {
                cout << "无效输入！" << endl;
                continue;
            }

            if (board->MovePiece(positions[0].first, positions[0].second, positions[1].first, positions[1].second)) {
                currentPlayer = (currentPlayer == RED) ? BLACK : RED;
                pair<pair<int, int>, pair<int, int>> move = {positions[0], positions[1]};
                ai.Update(move);
            } else {
                cout << "非法移动！" << endl;
                continue;
            }
        }
        if (ai.root->IsGameOver(*board, currentPlayer) != NOT_OVER)
        {
            break;
        }
        
        
    }
}

vector<pair<int, int>> ChessGame::ParseInput(const string& input) {
    vector<pair<int, int>> positions;

    // 检查输入格式
    if (input.length() != 5 || input[2] != ' ') {
        return positions; // 返回空列表表示输入无效
    }

    // 解析起始位置
    char startColChar = input[0];
    char startRowChar = input[1];
    if (startColChar < 'a' || startColChar > 'i' || startRowChar < '0' || startRowChar > '9') {
        return positions; // 返回空列表表示输入无效
    }
    int startCol = startColChar - 'a';
    int startRow = startRowChar - '0';

    // 解析目标位置
    char endColChar = input[3];
    char endRowChar = input[4];
    if (endColChar < 'a' || endColChar > 'i' || endRowChar < '0' || endRowChar > '9') {
        return positions; // 返回空列表表示输入无效
    }
    int endCol = endColChar - 'a';
    int endRow = endRowChar - '0';

    // 返回解析后的坐标
    positions.push_back({startRow, startCol});
    positions.push_back({endRow, endCol});
    return positions;
}