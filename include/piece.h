#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// 棋子类型枚举
enum PieceType {
    EMPTY,        // 空
    KING,        // 将/帅
    ADVISOR,     // 士/仕
    ELEPHANT,    // 象/相
    HORSE,       // 马/馬
    ROOK,        // 车/車
    CANNON,      // 炮/砲
    PAWN,        // 卒/兵
};

// 棋子颜色枚举
enum Color {
    NONE,
    RED,
    BLACK
};

// 棋子结构体
struct ChessPiece {
    PieceType type;
    Color color;
    string symbol; // Unicode符号
};

// 棋盘类
class ChessBoard {
private:
    vector<vector<ChessPiece>> board;
    map<pair<int, int>, string> positionSymbols;

public:
    string name;
    ChessBoard();
    ChessBoard(string name);
    ~ChessBoard();
    void InitializeBoard();
    void SetPiece(int row, int col, PieceType type, Color color);
    const ChessPiece* GetPiece(int row, int col) const;
    string GetSymbol(PieceType type, Color color);
    void Print();
    bool MovePiece(int fromRow, int fromCol, int toRow, int toCol);
    bool IsValidMove(int fromRow, int fromCol, int toRow, int toCol) const;
    void InitializeSymbols();

private:
    // 将/帅移动规则
    bool ValidateKingMove(int fromRow, int fromCol, int toRow, int toCol, Color color) const;

    // 士移动规则
    bool ValidateAdvisorMove(int fromRow, int fromCol, int toRow, int toCol, Color color) const;

    // 马移动规则
    bool ValidateHorseMove(int fromRow, int fromCol, int dx, int dy) const;

    // 象移动规则
    bool ValidateElephantMove(int fromRow, int fromCol, int toRow, int toCol, Color color) const;

    // 车移动规则
    bool ValidateRookMove(int fromRow, int fromCol, int toRow, int toCol) const;

    // 炮移动规则
    bool ValidateCannonMove(int fromRow, int fromCol, int toRow, int toCol) const;

    // 兵/卒移动规则
    bool ValidatePawnMove(int fromRow, int fromCol, int toRow, int toCol, Color color) const;
};

