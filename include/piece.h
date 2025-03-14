#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// 棋子类型枚举
enum PieceType {
    KING,        // 将/帅
    ADVISOR,     // 士/仕
    ELEPHANT,    // 象/相
    HORSE,       // 马/馬
    ROOK,        // 车/車
    CANNON,      // 炮/砲
    PAWN         // 卒/兵
};

// 棋子颜色枚举
enum Color {
    RED,
    BLACK
};

// 棋子结构体
struct ChessPiece {
    PieceType type;
    Color color;
    char symbol; // Unicode符号
};

// 棋盘类
class ChessBoard {
private:
    vector<vector<ChessPiece*>> board;
    map<pair<int, int>, char> positionSymbols;

public:
    ChessBoard();
    ~ChessBoard();
    void InitializeBoard();
    void SetPiece(int row, int col, PieceType type, Color color);
    char GetSymbol(PieceType type, Color color);
    void Print();
    bool MovePiece(int fromRow, int fromCol, int toRow, int toCol);
    bool IsValidMove(int fromRow, int fromCol, int toRow, int toCol);
    void InitializeSymbols();

private:
    // 将/帅移动规则
    bool ValidateKingMove(int fromRow, int fromCol, int toRow, int toCol, Color color);

    // 马移动规则
    bool ValidateHorseMove(int fromRow, int fromCol, int dx, int dy);

    // ... 其他棋子的移动规则验证
};

// 游戏管理类
class ChessGame {
private:
    ChessBoard board;
    Color currentPlayer;

public:
    ChessGame();

    void Start();

private:
    vector<pair<int, int>> ParseInput(const string& input);
};


