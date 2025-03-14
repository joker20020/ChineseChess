#include "piece.h"

ChessBoard::ChessBoard() {
    InitializeBoard();
    InitializeSymbols();
}

ChessBoard::~ChessBoard() {
    for (auto& row : board) {
        for (auto piece : row) {
            delete piece;
        }
    }
}

// 初始化棋盘格子
void ChessBoard::InitializeBoard() {
    board = vector<vector<ChessPiece*>>(10, vector<ChessPiece*>(9, nullptr));

    // 初始化红方棋子
    SetPiece(0, 0, ROOK, RED);       // 俥
    SetPiece(0, 1, HORSE, RED);      // 傌
    SetPiece(0, 2, ELEPHANT, RED);   // 相
    SetPiece(0, 3, ADVISOR, RED);    // 仕
    SetPiece(0, 4, KING, RED);       // 帥
    SetPiece(0, 5, ADVISOR, RED);    // 仕
    SetPiece(0, 6, ELEPHANT, RED);   // 相
    SetPiece(0, 7, HORSE, RED);      // 傌
    SetPiece(0, 8, ROOK, RED);       // 俥
    SetPiece(2, 1, CANNON, RED);     // 炮
    SetPiece(2, 7, CANNON, RED);     // 炮
    SetPiece(3, 0, PAWN, RED);       // 兵
    SetPiece(3, 2, PAWN, RED);       // 兵
    SetPiece(3, 4, PAWN, RED);       // 兵
    SetPiece(3, 6, PAWN, RED);       // 兵
    SetPiece(3, 8, PAWN, RED);       // 兵

    // 初始化黑方棋子
    SetPiece(9, 0, ROOK, BLACK);     // 車
    SetPiece(9, 1, HORSE, BLACK);    // 馬
    SetPiece(9, 2, ELEPHANT, BLACK); // 象
    SetPiece(9, 3, ADVISOR, BLACK);  // 士
    SetPiece(9, 4, KING, BLACK);     // 將
    SetPiece(9, 5, ADVISOR, BLACK);  // 士
    SetPiece(9, 6, ELEPHANT, BLACK); // 象
    SetPiece(9, 7, HORSE, BLACK);    // 馬
    SetPiece(9, 8, ROOK, BLACK);     // 車
    SetPiece(7, 1, CANNON, BLACK);   // 砲
    SetPiece(7, 7, CANNON, BLACK);   // 砲
    SetPiece(6, 0, PAWN, BLACK);     // 卒
    SetPiece(6, 2, PAWN, BLACK);     // 卒
    SetPiece(6, 4, PAWN, BLACK);     // 卒
    SetPiece(6, 6, PAWN, BLACK);     // 卒
    SetPiece(6, 8, PAWN, BLACK);     // 卒
}

// 设置棋子
void ChessBoard::SetPiece(int row, int col, PieceType type, Color color) {
    board[row][col] = new ChessPiece{type, color, GetSymbol(type, color)};
}

// 获取棋子对应符号
char ChessBoard::GetSymbol(PieceType type, Color color) {
    static map<pair<Color, PieceType>, char> symbols = {
        {{RED, KING},    '帥'}, {{BLACK, KING},    '將'},
        {{RED, ADVISOR}, '仕'}, {{BLACK, ADVISOR}, '士'},
        {{RED, ELEPHANT},'相'}, {{BLACK, ELEPHANT},'象'},
        {{RED, HORSE},   '傌'}, {{BLACK, HORSE},   '馬'},
        {{RED, ROOK},    '俥'}, {{BLACK, ROOK},    '車'},
        {{RED, CANNON},  '炮'}, {{BLACK, CANNON},  '砲'},
        {{RED, PAWN},    '兵'}, {{BLACK, PAWN},    '卒'}
    };
    return symbols[{color, type}];
}

// 打印棋盘
void ChessBoard::Print() {
    cout << "  a b c d e f g h i" << endl;
    for (int row = 0; row < 10; ++row) {
        cout << row << " ";
        for (int col = 0; col < 9; ++col) {
            if (board[row][col]) {
                cout << board[row][col]->symbol << " ";
            } else {
                // 显示格子符号
                cout << (positionSymbols[{row, col}] ? positionSymbols[{row, col}] : ' ') << " ";
            }
        }
        cout << row << endl;
    }
    cout << "  a b c d e f g h i" << endl;
}

// 移动棋子
bool ChessBoard::MovePiece(int fromRow, int fromCol, int toRow, int toCol) {
    if (!IsValidMove(fromRow, fromCol, toRow, toCol)) return false;
    
    ChessPiece* target = board[toRow][toCol];
    if (target) delete target;
    
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = nullptr;
    return true;
}

// 检测移动是否合法
bool ChessBoard::IsValidMove(int fromRow, int fromCol, int toRow, int toCol) {
    ChessPiece* piece = board[fromRow][fromCol];
    if (!piece) return false;

    int dx = toCol - fromCol;
    int dy = toRow - fromRow;

    switch (piece->type) {
        case KING:
            return ValidateKingMove(fromRow, fromCol, toRow, toCol, piece->color);
        case HORSE:
            return ValidateHorseMove(fromRow, fromCol, dx, dy);
        
        default:
            return false;
    }
}

// 检测将/帅是否合法
bool ChessBoard::ValidateKingMove(int fromRow, int fromCol, int toRow, int toCol, Color color) {
    // 移动范围限制
    if (toCol < 3 || toCol > 5) return false; // 将/帅只能在九宫格内移动
    if (color == RED && (toRow < 0 || toRow > 2)) return false; // 红方将/帅只能在下方九宫格
    if (color == BLACK && (toRow < 7 || toRow > 9)) return false; // 黑方将/帅只能在上方九宫格

    // 移动步长限制（只能走一步）
    return (abs(toCol - fromCol) + abs(toRow - fromRow)) == 1;
}

// 检测马是否合法
bool ChessBoard::ValidateHorseMove(int fromRow, int fromCol, int dx, int dy) {
    // 移动步长限制（走日字）
    if (!((abs(dx) == 1 && abs(dy) == 2) || 
        (abs(dx) == 2 && abs(dy) == 1))) return false;

    // 检查蹩马腿
    int blockRow = fromRow + dy / 2;
    int blockCol = fromCol + dx / 2;
    return !board[blockRow][blockCol];
}

void ChessBoard::InitializeSymbols() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 9; ++col) {
            // 河界
            if (row == 4 || row == 5) {
                positionSymbols[{row, col}] = '～'; // 使用波浪线表示河界
            }
            // 九宫格
            else if ((row >= 0 && row <= 2 && col >= 3 && col <= 5) || 
                     (row >= 7 && row <= 9 && col >= 3 && col <= 5)) {
                positionSymbols[{row, col}] = '＋'; // 使用加号表示九宫格
            }
            // 其他格子
            else {
                positionSymbols[{row, col}] = ' '; // 空格表示普通格子
            }
        }
    }
}


ChessGame::ChessGame() : currentPlayer(RED) {}

void ChessGame::Start() {
    while (true) {
        board.Print();
        cout << (currentPlayer == RED ? "红方" : "黑方") << "的回合" << endl;
        
        string input;
        cout << "输入移动（例如 a0 a1）：";
        getline(cin, input);

        auto positions = ParseInput(input);
        if (positions.size() != 2) {
            cout << "无效输入！" << endl;
            continue;
        }

        if (board.MovePiece(positions[0].first, positions[0].second,
                           positions[1].first, positions[1].second)) {
            currentPlayer = (currentPlayer == RED) ? BLACK : RED;
        } else {
            cout << "非法移动！" << endl;
        }
    }
}

vector<pair<int, int>> ChessGame::ParseInput(const string& input) {
    // 将类似 "a0 a1" 的输入转换为坐标
    vector<pair<int, int>> result;
    // ... 解析逻辑
    return result;
}