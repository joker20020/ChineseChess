#include "mcts.h"

MCTSNode::MCTSNode(ChessBoard board, Color currentPlayer, MCTSNode* parent)
        : board(board), currentPlayer(currentPlayer), parent(parent), visitCount(0), totalScore(0) {}

MCTSNode::~MCTSNode() {
    for (auto child : children) {
        delete child;
    }
}

// 判断是否为叶子节点
bool MCTSNode::IsLeaf() const {
    return children.empty();
}

// 判断是否为根节点
bool MCTSNode::IsRoot() const {
    return parent == nullptr;
}

// 计算 UCB1 值
double MCTSNode::UCB1(double explorationWeight) const {
    if (visitCount == 0) return numeric_limits<double>::max();
    return (totalScore / visitCount) + explorationWeight * sqrt(log(parent->visitCount) / visitCount);
}

// 选择最佳子节点
MCTSNode* MCTSNode::SelectBestChild() {
    return *max_element(children.begin(), children.end(), [](MCTSNode* a, MCTSNode* b) {
        return a->UCB1() < b->UCB1();
    });
}

// 扩展子节点
void MCTSNode::Expand() {
    vector<pair<pair<int, int>, pair<int, int>>> moves = GenerateLegalMoves(board, currentPlayer);
    for (const auto& move : moves) {
        ChessBoard newBoard = board;
        newBoard.MovePiece(move.first.first, move.first.second, move.second.first, move.second.second);
        MCTSNode* child = new MCTSNode(newBoard, (currentPlayer == RED) ? BLACK : RED, this);
        child->lastMove = move; // 记录移动
        children.push_back(child);
    }
}

// 随机模拟游戏
double MCTSNode::Simulate() {
    ChessBoard simBoard = board;
    Color simPlayer = currentPlayer;
    while (!IsGameOver(simBoard)) {
        vector<pair<pair<int, int>, pair<int, int>>> moves = GenerateLegalMoves(simBoard, simPlayer);
        if (moves.empty()) break;
        auto randomMove = moves[rand() % moves.size()];
        simBoard.MovePiece(randomMove.first.first, randomMove.first.second, randomMove.second.first, randomMove.second.second);
        simPlayer = (simPlayer == RED) ? BLACK : RED;
    }
    return EvaluateBoard(simBoard, currentPlayer);
}

// 回溯更新节点
void MCTSNode::Backpropagate(double score) {
    visitCount++;
    totalScore += score;
    if (!IsRoot()) parent->Backpropagate(score);
}

// 生成合法移动
vector<pair<pair<int, int>, pair<int, int>>> MCTSNode::GenerateLegalMoves(const ChessBoard& board, Color player) {
    vector<pair<pair<int, int>, pair<int, int>>> moves;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board.GetPiece(row, col) && board.GetPiece(row, col)->color == player) {
                for (int targetRow = 0; targetRow < 10; ++targetRow) {
                    for (int targetCol = 0; targetCol < 9; ++targetCol) {
                        if (board.IsValidMove(row, col, targetRow, targetCol)) {
                            moves.push_back({{row, col}, {targetRow, targetCol}});
                        }
                    }
                }
            }
        }
    }
    return moves;
}

// 判断游戏是否结束
bool MCTSNode::IsGameOver(const ChessBoard& board) {
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

// 评估棋盘状态
double MCTSNode::EvaluateBoard(const ChessBoard& board, Color player) {
    // 简单评估：将/帅是否存活
    bool redKingAlive = false, blackKingAlive = false;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board.GetPiece(row, col) && board.GetPiece(row, col)->type == KING) {
                if (board.GetPiece(row, col)->color == RED) redKingAlive = true;
                else blackKingAlive = true;
            }
        }
    }
    if (player == RED) return redKingAlive ? 1.0 : 0.0;
    else return blackKingAlive ? 1.0 : 0.0;
}

pair<pair<int, int>, pair<int, int>> MCTSNode::GetLastMove() const {
    return lastMove;
}