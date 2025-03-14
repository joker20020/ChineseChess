#include <stdio.h>
#include "piece.h"


int main() {
    ChessBoard board = ChessBoard();
    ChessGame game = ChessGame(&board);
    game.Start();
    return 0;
}