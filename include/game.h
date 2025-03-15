#pragma once
#include "piece.h"
#include "mcts.h"

// 游戏管理类
class ChessGame {
    private:
        
        Color currentPlayer;
        Color aiColor;
        
    public:
        ChessBoard *board;
        MCTSAI ai;
        ChessGame(ChessBoard *board, MCTSAI& ai);
        ~ChessGame();
    
        void Start();
    
    private:
        vector<pair<int, int>> ParseInput(const string& input);
    };