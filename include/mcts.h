#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "piece.h"

using namespace std;

// 棋盘和棋子定义（省略，参考之前的代码）

// MCTS 节点定义
class MCTSNode {
public:
    ChessBoard board; // 当前棋盘状态
    Color currentPlayer; // 当前玩家
    MCTSNode* parent; // 父节点
    vector<MCTSNode*> children; // 子节点
    int visitCount; // 访问次数
    double totalScore; // 总得分
    pair<pair<int, int>, pair<int, int>> lastMove; // 记录最后移动

    MCTSNode(ChessBoard board, Color currentPlayer, MCTSNode* parent = nullptr);

    ~MCTSNode();

    // 判断是否为叶子节点
    bool IsLeaf() const;

    // 判断是否为根节点
    bool IsRoot() const;

    // 计算 UCB1 值
    double UCB1(double explorationWeight = 1.414) const;

    // 选择最佳子节点
    MCTSNode* SelectBestChild();

    // 扩展子节点
    void Expand();

    // 随机模拟游戏
    double Simulate();

    // 回溯更新节点
    void Backpropagate(double score);

    // 获取最后移动
    pair<pair<int, int>, pair<int, int>> GetLastMove() const;

private:
    // 生成合法移动
    vector<pair<pair<int, int>, pair<int, int>>> GenerateLegalMoves(const ChessBoard& board, Color player);

    // 判断游戏是否结束
    bool IsGameOver(const ChessBoard& board);
    // 评估棋盘状态
    double EvaluateBoard(const ChessBoard& board, Color player);
};

// MCTS AI
class MCTSAI {
public:
    MCTSNode* root;

    MCTSAI(const ChessBoard board, Color player) {
        root = new MCTSNode(board, player);
    }

    ~MCTSAI() {
        delete root;
    }

    // 运行 MCTS
    void Run(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            MCTSNode* node = Select(root);
            if (!node->IsLeaf()) {
                node = node->SelectBestChild();
            }
            if (!IsGameOver(node->board)) {
                node->Expand();
                node = node->children[0];
            }
            double score = node->Simulate();
            node->Backpropagate(score);
        }
    }

    // 选择最佳移动
    pair<pair<int, int>, pair<int, int>> GetBestMove() {
        MCTSNode* bestChild = *max_element(root->children.begin(), root->children.end(), [](MCTSNode* a, MCTSNode* b) {
            return a->visitCount < b->visitCount;
        });
        return bestChild->GetLastMove();
    }

private:
    // 选择节点
    MCTSNode* Select(MCTSNode* node) {
        while (!node->IsLeaf()) {
            node = node->SelectBestChild();
        }
        return node;
    }

    // 判断游戏是否结束
    bool IsGameOver(const ChessBoard& board) {
        // 检查是否有一方的将/帅被吃掉
        bool redKingAlive = false, blackKingAlive = false;
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (board.GetPiece(row, col) && board.GetPiece(row, col)->type == KING) {
                    if (board.GetPiece(row, col)->color == RED) redKingAlive = true;
                    else blackKingAlive = true;
                }
            }
        }
        return !redKingAlive || !blackKingAlive;
    }
};

// 主函数
// int main() {
//     srand(time(nullptr));

//     ChessBoard board;
//     board.InitializeBoard();

//     MCTSAI ai(board, RED);
//     ai.Run(1000); // 运行 1000 次模拟

//     auto bestMove = ai.GetBestMove();
//     cout << "最佳移动: (" << bestMove.first.first << ", " << bestMove.first.second << ") -> ("
//          << bestMove.second.first << ", " << bestMove.second.second << ")" << endl;

//     return 0;
// }