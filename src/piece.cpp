#include "piece.h"

ChessBoard::ChessBoard(string name) {
    // cout << "ChessBoard constructor called" << endl;
    this->name = name;
    InitializeBoard();
    InitializeSymbols();
}

ChessBoard::ChessBoard() {
    // cout << "ChessBoard constructor called" << endl;
    this->name = "ChessBoard";
    InitializeBoard();
    InitializeSymbols();
}

ChessBoard::~ChessBoard() {
    // cout << this->name << " ChessBoard destructor called" << endl;
    // for (auto& row : board) {
    //     for (auto piece : row) {
    //         delete &piece;
    //     }
    // }
}

// 初始化棋盘格子
void ChessBoard::InitializeBoard() {
    // 初始化棋盘为 10 行 x 9 列
    board = vector<vector<ChessPiece>>(10, vector<ChessPiece>(9, ChessPiece{EMPTY, NONE, " "}));

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
    board[row][col] = ChessPiece{type, color, GetSymbol(type, color)}; // 分配新对象
}

const ChessPiece* ChessBoard::GetPiece(int row, int col) const{
    return &(board[row][col]);
}

// 获取棋子标识
string ChessBoard::GetSymbol(PieceType type, Color color) {
    static map<pair<Color, PieceType>, string> symbols = {
        {{RED, KING},    "帥"}, {{BLACK, KING},    "將"},
        {{RED, ADVISOR}, "仕"}, {{BLACK, ADVISOR}, "士"},
        {{RED, ELEPHANT},"相"}, {{BLACK, ELEPHANT},"象"},
        {{RED, HORSE},   "傌"}, {{BLACK, HORSE},   "馬"},
        {{RED, ROOK},    "俥"}, {{BLACK, ROOK},    "車"},
        {{RED, CANNON},  "炮"}, {{BLACK, CANNON},  "砲"},
        {{RED, PAWN},    "兵"}, {{BLACK, PAWN},    "卒"}
    };
    return symbols[{color, type}];
}

// 打印棋盘
void ChessBoard::Print(bool reverse) {
    

    // 打印棋盘
    if (reverse)
    {
        // 打印列标签
        cout << "  ";
        for (int col = BOARD_WIDTH - 1; col >= 0; --col) {
            cout << " " << static_cast<char>('a' + col) << "   "; // 列标签（i-a）
        }
        cout << endl;
        for (int row = BOARD_HEIGHT - 1; row >= 0; --row) {
            // 打印行标签
            cout << row << " "; // 行标签（0-9）
    
            // 打印棋盘内容
            for (int col = BOARD_WIDTH - 1; col >= 0; --col) {
                if (board[row][col].type != EMPTY) {
                    // 打印棋子符号（固定宽度为 2 个字符）
                    cout << " " << board[row][col].symbol << " ";
                } else {
                    // 打印棋盘格子符号（固定宽度为 2 个字符）
                    cout << " " << (positionSymbols[{row, col}] != " " ? positionSymbols[{row, col}] : "  ") << " ";
                }
                if (col > 0) cout << "|"; // 列分隔符
            }
            cout << " " << row << endl; // 行标签（0-9）
    
            // 打印行分隔线
            if (row >= 0) {
                cout << "  ";
                for (int col = 0; col < BOARD_WIDTH; ++col) {
                    cout << "----";
                    if (col < 8) cout << "+";
                }
                cout << endl;
            }
        }

        // 打印列标签
        cout << "  ";
        for (int col = BOARD_WIDTH - 1; col >= 0; --col) {
            cout << " " << static_cast<char>('a' + col) << "   "; // 列标签（i-a）
        }
    }
    else{
        // 打印列标签
        cout << "  ";
        for (int col = 0; col < BOARD_WIDTH; ++col) {
            cout << " " << static_cast<char>('a' + col) << "   "; // 列标签（a-i）
        }
        cout << endl;

        for (int row = 0; row < BOARD_HEIGHT; ++row) {
            // 打印行标签
            cout << row << " "; // 行标签（0-9）
    
            // 打印棋盘内容
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                if (board[row][col].type != EMPTY) {
                    // 打印棋子符号（固定宽度为 2 个字符）
                    cout << " " << board[row][col].symbol << " ";
                } else {
                    // 打印棋盘格子符号（固定宽度为 2 个字符）
                    cout << " " << (positionSymbols[{row, col}] != " " ? positionSymbols[{row, col}] : "  ") << " ";
                }
                if (col < 8) cout << "|"; // 列分隔符
            }
            cout << " " << row << endl; // 行标签（0-9）
    
            // 打印行分隔线
            if (row < BOARD_HEIGHT) {
                cout << "  ";
                for (int col = 0; col < BOARD_WIDTH; ++col) {
                    cout << "----";
                    if (col < 8) cout << "+";
                }
                cout << endl;
            }
        }

        // 打印列标签
        cout << "  ";
        for (int col = 0; col < BOARD_WIDTH; ++col) {
            cout << " " << static_cast<char>('a' + col) << "   "; // 列标签（a-i）
        }
        cout << endl;
    }  
    cout << endl;
    
}

// 移动棋子
bool ChessBoard::MovePiece(int fromRow, int fromCol, int toRow, int toCol) {
    if (!IsValidMove(fromRow, fromCol, toRow, toCol)) return false;
    
    // ChessPiece* target = board[toRow][toCol];
    // if (target) delete target;
    
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = ChessPiece{EMPTY, NONE, " "};
    return true;
}

// 移动验证（核心逻辑）
bool ChessBoard::IsValidMove(int fromRow, int fromCol, int toRow, int toCol) const{
    ChessPiece piece = board[fromRow][fromCol];
    if (piece.type == EMPTY) return false;
    if (fromRow == toRow && fromCol == toCol) return false;
    if (board[toRow][toCol].color == piece.color) return false;
    

    int dx = toCol - fromCol;
    int dy = toRow - fromRow;

    switch (piece.type) {
        case KING:
            return ValidateKingMove(fromRow, fromCol, toRow, toCol, piece.color);
        case HORSE:
            return ValidateHorseMove(fromRow, fromCol, dx, dy);
        case ADVISOR:
            return ValidateAdvisorMove(fromRow, fromCol, toRow, toCol, piece.color);
        case ELEPHANT:
            return ValidateElephantMove(fromRow, fromCol, toRow, toCol, piece.color);
        case ROOK:
            return ValidateRookMove(fromRow, fromCol, toRow, toCol);
        case CANNON:
            return ValidateCannonMove(fromRow, fromCol, toRow, toCol);
        case PAWN:
            return ValidatePawnMove(fromRow, fromCol, toRow, toCol, piece.color);
        default:
            return false;
    }
}

// 将/帅移动规则
bool ChessBoard::ValidateKingMove(int fromRow, int fromCol, int toRow, int toCol, Color color)  const{
    // 移动范围限制
    if (toCol < 3 || toCol > 5) return false;
    if (color == RED && (toRow < 0 || toRow > 2)) return false;
    if (color == BLACK && (toRow < 7 || toRow > 9)) return false;

    // 移动步长限制
    return (abs(toCol - fromCol) + abs(toRow - fromRow)) == 1;
}

// 马移动规则
bool ChessBoard::ValidateHorseMove(int fromRow, int fromCol, int dx, int dy)  const{
    // 马走日字
    if (!((abs(dx) == 1 && abs(dy) == 2) || 
    (abs(dx) == 2 && abs(dy) == 1))) return false;

    // 检查蹩马腿
    int blockRow = fromRow + dy/2;
    int blockCol = fromCol + dx/2;
    return board[blockRow][blockCol].type == EMPTY;
}

// 士移动规则
bool ChessBoard::ValidateAdvisorMove(int fromRow, int fromCol, int toRow, int toCol, Color color)  const{
    // 移动范围限制
    if (toCol < 3 || toCol > 5) return false; // 士只能在九宫格内移动
    if (color == RED && (toRow < 0 || toRow > 2)) return false; // 红方士只能在下方九宫格
    if (color == BLACK && (toRow < 7 || toRow > 9)) return false; // 黑方士只能在上方九宫格

    // 移动步长限制（只能斜走一步）
    return abs(toCol - fromCol) == 1 && abs(toRow - fromRow) == 1;
}

// 象移动规则
bool ChessBoard::ValidateElephantMove(int fromRow, int fromCol, int toRow, int toCol, Color color)  const{
    // 移动步长限制（走田字）
    if (abs(toCol - fromCol) != 2 || abs(toRow - fromRow) != 2) return false;

    // 检查象眼是否被堵
    int blockRow = fromRow + (toRow - fromRow) / 2;
    int blockCol = fromCol + (toCol - fromCol) / 2;
    if (board[blockRow][blockCol].type != EMPTY) return false;

    // 不能过河
    if (color == RED && toRow > 4) return false; // 红方象不能过河
    if (color == BLACK && toRow < 5) return false; // 黑方象不能过河

    return true;
}

// 车移动规则
bool ChessBoard::ValidateRookMove(int fromRow, int fromCol, int toRow, int toCol)  const{
    // 车只能直线移动
    if (fromRow != toRow && fromCol != toCol) return false;

    // 检查路径上是否有障碍物
    if (fromRow == toRow) { // 水平移动
        int step = (toCol > fromCol) ? 1 : -1;
        for (int col = fromCol + step; col != toCol; col += step) {
            if (board[fromRow][col].type != EMPTY) return false;
        }
    } else { // 垂直移动
        int step = (toRow > fromRow) ? 1 : -1;
        for (int row = fromRow + step; row != toRow; row += step) {
            if (board[row][fromCol].type != EMPTY) return false;
        }
    }

    return true;
}

// 炮移动规则
bool ChessBoard::ValidateCannonMove(int fromRow, int fromCol, int toRow, int toCol)  const{
    // 炮只能直线移动
    if (fromRow != toRow && fromCol != toCol) return false;

    int obstacleCount = 0;

    if (fromRow == toRow) { // 水平移动
        int step = (toCol > fromCol) ? 1 : -1;
        for (int col = fromCol + step; col != toCol; col += step) {
            if (board[fromRow][col].type != EMPTY) obstacleCount++;
        }
    } else { // 垂直移动
        int step = (toRow > fromRow) ? 1 : -1;
        for (int row = fromRow + step; row != toRow; row += step) {
            if (board[row][fromCol].type != EMPTY) obstacleCount++;
        }
    }

    // 炮的规则：移动时不能有障碍物，吃子时必须有一个障碍物
    ChessPiece target = board[toRow][toCol];
    if (target.type != EMPTY) { // 吃子
        return obstacleCount == 1;
    } else { // 移动
        return obstacleCount == 0;
    }
}

// 兵/卒移动规则
bool ChessBoard::ValidatePawnMove(int fromRow, int fromCol, int toRow, int toCol, Color color)  const{
    int dx = toCol - fromCol;
    int dy = toRow - fromRow;

    if (color == RED) { // 红方兵
        if (fromRow < 5) { // 未过河
            if (dy != 1 || dx != 0) return false; // 只能向前走一步
        } else { // 已过河
            if (!((dy == 1 && dx == 0) || (dy == 0 && abs(dx) == 1))) return false; // 可以向前或左右走一步
        }
    } else { // 黑方卒
        if (fromRow > 4) { // 未过河
            if (dy != -1 || dx != 0) return false; // 只能向前走一步
        } else { // 已过河
            if (!((dy == -1 && dx == 0) || (dy == 0 && abs(dx) == 1))) return false; // 可以向前或左右走一步
        }
    }

    return true;
}

// 初始化棋盘格子符号
void ChessBoard::InitializeSymbols() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 9; ++col) {
            // 河界
            if (row == 4 || row == 5) {
                positionSymbols[{row, col}] = "～"; // 使用波浪线表示河界
            }
            // 九宫格
            else if ((row >= 0 && row <= 2 && col >= 3 && col <= 5) || 
                     (row >= 7 && row <= 9 && col >= 3 && col <= 5)) {
                positionSymbols[{row, col}] = "＋"; // 使用加号表示九宫格
            }
            // 其他格子
            else {
                positionSymbols[{row, col}] = " "; // 空格表示普通格子
            }
        }
    }
}
