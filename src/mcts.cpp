#include "mcts.h"

MCTSNode::MCTSNode(const ChessBoard& board, Color currentPlayer, MCTSNode* parent){
    this->board = board;
    this->board.name = "in chessboard";
    this->currentPlayer = currentPlayer;
    this->parent = parent;
    this->visitCount = 0;
    this->totalScore = 0;
}

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
    GameResult result = IsGameOver(simBoard, simPlayer);
    int noEatCount = 0;
    while (result == NOT_OVER) {
        vector<pair<pair<int, int>, pair<int, int>>> moves = GenerateLegalMoves(simBoard, simPlayer);
        if (moves.empty()) break;
        auto randomMove = moves[rand() % moves.size()];
        if (simBoard.GetPiece(randomMove.second.first, randomMove.second.second)->type == EMPTY) noEatCount++;
        else noEatCount = 0;
        if (noEatCount >= 40){
            result = DRAW;
            break;
        }
        simBoard.MovePiece(randomMove.first.first, randomMove.first.second, randomMove.second.first, randomMove.second.second);
        // cout << "move:" << randomMove.first.first << "," << randomMove.first.second << "->" << randomMove.second.first << "," << randomMove.second.second << endl;
        // cout << "noEatCount:" << noEatCount << endl;
        simPlayer = (simPlayer == RED) ? BLACK : RED;
        // simBoard.Print();
        result = IsGameOver(simBoard, simPlayer);
    }
    return EvaluateBoard(result, currentPlayer);
}

// 回溯更新节点
void MCTSNode::Backpropagate(double score) {
    visitCount++;
    totalScore += score;
    if (!IsRoot()) parent->Backpropagate(-score);
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
GameResult MCTSNode::IsGameOver(const ChessBoard& board, Color currentPlayer) {
    bool redKingAlive = false, blackKingAlive = false;
    pair<int, int> redKingPos, blackKingPos;

    // 检查将/帅是否存活，并记录位置
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 9; ++col) {
            const ChessPiece* piece = board.GetPiece(row, col);
            if (piece && piece->type == KING) {
                if (piece->color == RED) {
                    redKingAlive = true;
                    redKingPos = {row, col};
                } else {
                    blackKingAlive = true;
                    blackKingPos = {row, col};
                }
            }
        }
    }

    // 判断将/帅是否存活
    if (!redKingAlive && !blackKingAlive) {
        return DRAW; // 双方将/帅都被吃掉，平局
    } else if (!redKingAlive) {
        return BLACK_WIN; // 红方将/帅被吃掉，黑方获胜
    } else if (!blackKingAlive) {
        return RED_WIN; // 黑方将/帅被吃掉，红方获胜
    }

    // 检查将/帅是否相对且中间无棋子
    if (redKingPos.second == blackKingPos.second) { // 在同一列
        bool hasObstacle = false;
        int startRow = min(redKingPos.first, blackKingPos.first) + 1;
        int endRow = max(redKingPos.first, blackKingPos.first);
        for (int row = startRow; row < endRow; ++row) {
            if (board.GetPiece(row, redKingPos.second)) {
                hasObstacle = true;
                break;
            }
        }
        if (!hasObstacle) {
            // 将/帅相对且中间无棋子，违规方判负
            return (currentPlayer == BLACK) ? BLACK_WIN : RED_WIN;
        }
    }

    // 检查双方是否只剩下将/帅
    bool redHasOtherPieces = false, blackHasOtherPieces = false;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 9; ++col) {
            const ChessPiece* piece = board.GetPiece(row, col);
            if (piece && piece->type != KING) {
                if (piece->color == RED) redHasOtherPieces = true;
                else blackHasOtherPieces = true;
            }
        }
    }

    if (!redHasOtherPieces && !blackHasOtherPieces) {
        return DRAW; // 双方都只剩下将/帅，无法将死对方，平局
    }

    // // 检查双方是否有合法移动
    // bool redHasMoves = false, blackHasMoves = false;
    // for (int row = 0; row < 10; ++row) {
    //     for (int col = 0; col < 9; ++col) {
    //         const ChessPiece* piece = board.GetPiece(row, col);
    //         if (piece) {
    //             if (piece->color == RED) {
    //                 for (int targetRow = 0; targetRow < 10; ++targetRow) {
    //                     for (int targetCol = 0; targetCol < 9; ++targetCol) {
    //                         if (board.IsValidMove(row, col, targetRow, targetCol)) {
    //                             redHasMoves = true;
    //                             break;
    //                         }
    //                     }
    //                     if (redHasMoves) break;
    //                 }
    //             } else {
    //                 for (int targetRow = 0; targetRow < 10; ++targetRow) {
    //                     for (int targetCol = 0; targetCol < 9; ++targetCol) {
    //                         if (board.IsValidMove(row, col, targetRow, targetCol)) {
    //                             blackHasMoves = true;
    //                             break;
    //                         }
    //                     }
    //                     if (blackHasMoves) break;
    //                 }
    //             }
    //         }
    //     }
    // }

    // // 判断是否无棋可走
    // if (!redHasMoves && !blackHasMoves) {
    //     return DRAW; // 双方都无棋可走，平局
    // } else if (!redHasMoves) {
    //     return BLACK_WIN; // 红方无棋可走，黑方获胜
    // } else if (!blackHasMoves) {
    //     return RED_WIN; // 黑方无棋可走，红方获胜
    // }

    return NOT_OVER; // 游戏未结束
}

// 评估棋盘状态
double MCTSNode::EvaluateBoard(GameResult result, Color currentPlayer) {
    if (result == BLACK_WIN && currentPlayer == BLACK) return -1.0;
    else if (result == RED_WIN && currentPlayer == RED) return -1.0;
    else if (result == RED_WIN && currentPlayer == BLACK) return 1.0;
    else if (result == BLACK_WIN && currentPlayer == BLACK) return 1.0;
    else if (result == DRAW) return 0.0;
    else return 0.0;
    
}

// 打印节点对应棋盘
void MCTSNode::Print(){
    board.Print();
}

// 获取上一次移动
pair<pair<int, int>, pair<int, int>> MCTSNode::GetLastMove() const {
    return lastMove;
}

MCTSAI::MCTSAI(){}

MCTSAI::MCTSAI(const ChessBoard board, Color player) {
    root = new MCTSNode(board, player);
}

MCTSAI::~MCTSAI() {
    delete root;
}

// 运行 MCTS
void MCTSAI::Run(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        cout << "Iteration " << i + 1 << endl;
        MCTSNode* node = Select(root);
        if (!node->IsLeaf()) {
            node = node->SelectBestChild();
        }
        if (node->IsGameOver(node->board, node->currentPlayer) == NOT_OVER) {
            node->Expand();
            node = node->children[rand() % node->children.size()];
        }
        double score = node->Simulate();
        node->Backpropagate(score);
    }
}

// 选择最佳移动
pair<pair<int, int>, pair<int, int>> MCTSAI::GetBestMove() {
    MCTSNode* bestChild = *max_element(root->children.begin(), root->children.end(), [](MCTSNode* a, MCTSNode* b) {
        return a->visitCount < b->visitCount;
    });
    return bestChild->GetLastMove();
}

// 选择节点
MCTSNode* MCTSAI::Select(MCTSNode* node) {
    while (!node->IsLeaf()) {
        node = node->SelectBestChild();
    }
    return node;
}

// 更新节点
void MCTSAI::AutoUpdate() {
    if (root->children.size() == 0){
        Run(1);
    }
    vector<MCTSNode*>::iterator bestChildIterator = max_element(root->children.begin(), root->children.end(), [](MCTSNode* a, MCTSNode* b) {
        return a->visitCount < b->visitCount;
    });
    MCTSNode* bestChild = *bestChildIterator;
    int bestChildIndex = bestChildIterator - root->children.begin();

    root->children.erase(root->children.begin() + bestChildIndex);
    delete root;
    root = bestChild;
    root->parent = nullptr;
}

void MCTSAI::Update(pair<pair<int, int>, pair<int, int>> move) {
    size_t i = 0;
    if (root->children.size() == 0){
        Run(1);
    }
    for(;i < root->children.size();i++){
        if(root->children[i]->GetLastMove() == move){
            break;
        }
    }
    MCTSNode* bestChild = root->children[i];

    root->children.erase(root->children.begin() + i);
    delete root;
    root = bestChild;
    root->parent = nullptr;
}