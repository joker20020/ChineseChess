#include "mcts.h"

MCTSNode::MCTSNode(const ChessBoard& board, Color currentPlayer, MCTSNode* parent){
    this->board = board;
    this->board.name = "in chessboard";
    this->currentPlayer = currentPlayer;
    this->parent = parent;
    this->visitCount.store(0);
    this->totalScore.store(0);
    this->virtualLoss.store(0);
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
double MCTSNode::UCB1(double explorationWeight) const{
    if (visitCount == 0) return numeric_limits<double>::max();
    return (totalScore.load() / visitCount.load()) + explorationWeight * sqrt(log(parent->visitCount.load()) / visitCount.load()) + virtualLoss.load();
}

// 选择最佳子节点
MCTSNode* MCTSNode::SelectBestChild() {
    lock_guard<mutex> lock(mtx);
    MCTSNode* bestChild = *max_element(children.begin(), children.end(), [](MCTSNode* a, MCTSNode* b) {
        return a->UCB1() < b->UCB1();
    });
    bestChild->virtualLoss.store(bestChild->virtualLoss.load() - 1);
    return bestChild;
}

// 扩展子节点
void MCTSNode::Expand() {
    lock_guard<mutex> lock(mtx);
    if (!IsLeaf()) return;
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
    totalScore.store(totalScore + score);
    virtualLoss.store(virtualLoss + 1);
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
    return board.IsGameOver(board, currentPlayer);
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

MCTSAI::MCTSAI(const MCTSAI &other) {
    root = other.root;
}
MCTSAI& MCTSAI::operator=(const MCTSAI& other){
    root = other.root;
    return *this;
}
MCTSAI::~MCTSAI() {
    delete root;
}

// 运行 MCTS
void MCTSAI::Run(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        // cout << "Iteration: " << i + 1 << "/"  << iterations << '\r';
        MCTSNode* node = Select(root);
        if (!node->IsLeaf()) {
            node = node->SelectBestChild();
        }
        if (node->IsGameOver(node->board, node->currentPlayer) == NOT_OVER && node->IsLeaf()) {
            node->Expand();
            node = node->children[rand() % node->children.size()];
        }
        double score = node->Simulate();
        node->Backpropagate(score);
    }
}

// 多线程运行 MCTS
void MCTSAI::ParallelRun(int iterations, int threadNum) {
    vector<thread> threads;
    int threadIterations = iterations / threadNum;
    for (int i = 0; i < threadNum; i++)
    {
        threads.push_back(
            thread(&MCTSAI::Run, this, threadIterations)
        );
    }
    for(auto &thread : threads){
        thread.join();
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