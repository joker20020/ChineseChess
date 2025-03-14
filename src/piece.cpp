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

// ��ʼ�����̸���
void ChessBoard::InitializeBoard() {
    board = vector<vector<ChessPiece*>>(10, vector<ChessPiece*>(9, nullptr));

    // ��ʼ���췽����
    SetPiece(0, 0, ROOK, RED);       // �e
    SetPiece(0, 1, HORSE, RED);      // ��
    SetPiece(0, 2, ELEPHANT, RED);   // ��
    SetPiece(0, 3, ADVISOR, RED);    // ��
    SetPiece(0, 4, KING, RED);       // ��
    SetPiece(0, 5, ADVISOR, RED);    // ��
    SetPiece(0, 6, ELEPHANT, RED);   // ��
    SetPiece(0, 7, HORSE, RED);      // ��
    SetPiece(0, 8, ROOK, RED);       // �e
    SetPiece(2, 1, CANNON, RED);     // ��
    SetPiece(2, 7, CANNON, RED);     // ��
    SetPiece(3, 0, PAWN, RED);       // ��
    SetPiece(3, 2, PAWN, RED);       // ��
    SetPiece(3, 4, PAWN, RED);       // ��
    SetPiece(3, 6, PAWN, RED);       // ��
    SetPiece(3, 8, PAWN, RED);       // ��

    // ��ʼ���ڷ�����
    SetPiece(9, 0, ROOK, BLACK);     // ܇
    SetPiece(9, 1, HORSE, BLACK);    // �R
    SetPiece(9, 2, ELEPHANT, BLACK); // ��
    SetPiece(9, 3, ADVISOR, BLACK);  // ʿ
    SetPiece(9, 4, KING, BLACK);     // ��
    SetPiece(9, 5, ADVISOR, BLACK);  // ʿ
    SetPiece(9, 6, ELEPHANT, BLACK); // ��
    SetPiece(9, 7, HORSE, BLACK);    // �R
    SetPiece(9, 8, ROOK, BLACK);     // ܇
    SetPiece(7, 1, CANNON, BLACK);   // �h
    SetPiece(7, 7, CANNON, BLACK);   // �h
    SetPiece(6, 0, PAWN, BLACK);     // ��
    SetPiece(6, 2, PAWN, BLACK);     // ��
    SetPiece(6, 4, PAWN, BLACK);     // ��
    SetPiece(6, 6, PAWN, BLACK);     // ��
    SetPiece(6, 8, PAWN, BLACK);     // ��
}

// ��������
void ChessBoard::SetPiece(int row, int col, PieceType type, Color color) {
    board[row][col] = new ChessPiece{type, color, GetSymbol(type, color)};
}

// ��ȡ���Ӷ�Ӧ����
char ChessBoard::GetSymbol(PieceType type, Color color) {
    static map<pair<Color, PieceType>, char> symbols = {
        {{RED, KING},    '��'}, {{BLACK, KING},    '��'},
        {{RED, ADVISOR}, '��'}, {{BLACK, ADVISOR}, 'ʿ'},
        {{RED, ELEPHANT},'��'}, {{BLACK, ELEPHANT},'��'},
        {{RED, HORSE},   '��'}, {{BLACK, HORSE},   '�R'},
        {{RED, ROOK},    '�e'}, {{BLACK, ROOK},    '܇'},
        {{RED, CANNON},  '��'}, {{BLACK, CANNON},  '�h'},
        {{RED, PAWN},    '��'}, {{BLACK, PAWN},    '��'}
    };
    return symbols[{color, type}];
}

// ��ӡ����
void ChessBoard::Print() {
    cout << "  a b c d e f g h i" << endl;
    for (int row = 0; row < 10; ++row) {
        cout << row << " ";
        for (int col = 0; col < 9; ++col) {
            if (board[row][col]) {
                cout << board[row][col]->symbol << " ";
            } else {
                // ��ʾ���ӷ���
                cout << (positionSymbols[{row, col}] ? positionSymbols[{row, col}] : ' ') << " ";
            }
        }
        cout << row << endl;
    }
    cout << "  a b c d e f g h i" << endl;
}

// �ƶ�����
bool ChessBoard::MovePiece(int fromRow, int fromCol, int toRow, int toCol) {
    if (!IsValidMove(fromRow, fromCol, toRow, toCol)) return false;
    
    ChessPiece* target = board[toRow][toCol];
    if (target) delete target;
    
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = nullptr;
    return true;
}

// ����ƶ��Ƿ�Ϸ�
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

// ��⽫/˧�Ƿ�Ϸ�
bool ChessBoard::ValidateKingMove(int fromRow, int fromCol, int toRow, int toCol, Color color) {
    // �ƶ���Χ����
    if (toCol < 3 || toCol > 5) return false; // ��/˧ֻ���ھŹ������ƶ�
    if (color == RED && (toRow < 0 || toRow > 2)) return false; // �췽��/˧ֻ�����·��Ź���
    if (color == BLACK && (toRow < 7 || toRow > 9)) return false; // �ڷ���/˧ֻ�����Ϸ��Ź���

    // �ƶ��������ƣ�ֻ����һ����
    return (abs(toCol - fromCol) + abs(toRow - fromRow)) == 1;
}

// ������Ƿ�Ϸ�
bool ChessBoard::ValidateHorseMove(int fromRow, int fromCol, int dx, int dy) {
    // �ƶ��������ƣ������֣�
    if (!((abs(dx) == 1 && abs(dy) == 2) || 
        (abs(dx) == 2 && abs(dy) == 1))) return false;

    // ���������
    int blockRow = fromRow + dy / 2;
    int blockCol = fromCol + dx / 2;
    return !board[blockRow][blockCol];
}

void ChessBoard::InitializeSymbols() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 9; ++col) {
            // �ӽ�
            if (row == 4 || row == 5) {
                positionSymbols[{row, col}] = '��'; // ʹ�ò����߱�ʾ�ӽ�
            }
            // �Ź���
            else if ((row >= 0 && row <= 2 && col >= 3 && col <= 5) || 
                     (row >= 7 && row <= 9 && col >= 3 && col <= 5)) {
                positionSymbols[{row, col}] = '��'; // ʹ�üӺű�ʾ�Ź���
            }
            // ��������
            else {
                positionSymbols[{row, col}] = ' '; // �ո��ʾ��ͨ����
            }
        }
    }
}


ChessGame::ChessGame() : currentPlayer(RED) {}

void ChessGame::Start() {
    while (true) {
        board.Print();
        cout << (currentPlayer == RED ? "�췽" : "�ڷ�") << "�Ļغ�" << endl;
        
        string input;
        cout << "�����ƶ������� a0 a1����";
        getline(cin, input);

        auto positions = ParseInput(input);
        if (positions.size() != 2) {
            cout << "��Ч���룡" << endl;
            continue;
        }

        if (board.MovePiece(positions[0].first, positions[0].second,
                           positions[1].first, positions[1].second)) {
            currentPlayer = (currentPlayer == RED) ? BLACK : RED;
        } else {
            cout << "�Ƿ��ƶ���" << endl;
        }
    }
}

vector<pair<int, int>> ChessGame::ParseInput(const string& input) {
    // ������ "a0 a1" ������ת��Ϊ����
    vector<pair<int, int>> result;
    // ... �����߼�
    return result;
}