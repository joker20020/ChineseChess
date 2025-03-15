#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "piece.h"

using namespace std;

enum GameResult {
    RED_WIN,    // 红方获胜
    BLACK_WIN,  // 黑方获胜
    DRAW,       // 平局
    NOT_OVER    // 游戏未结束
};

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

    MCTSNode(const ChessBoard& board, Color currentPlayer, MCTSNode* parent = nullptr);

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

    // 判断游戏是否结束
    static GameResult IsGameOver(const ChessBoard& board, Color currentPlayer);

    // 评估棋盘状态
    static double EvaluateBoard(GameResult result, Color player);

    // 打印节点对应棋盘
    void Print();

private:
    // 生成合法移动
    vector<pair<pair<int, int>, pair<int, int>>> GenerateLegalMoves(const ChessBoard& board, Color player);
    
};

// MCTS AI
class MCTSAI {
public:
    MCTSNode* root;

    MCTSAI();
    MCTSAI(const ChessBoard board, Color player);

    ~MCTSAI();
    // 运行 MCTS
    void Run(int iterations);

    // 选择最佳移动
    pair<pair<int, int>, pair<int, int>> GetBestMove();

    // 自动更新节点
    void AutoUpdate();

    // 手动更新节点
    void Update(pair<pair<int, int>, pair<int, int>> move);

private:
    // 选择节点
    MCTSNode* Select(MCTSNode* node);

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