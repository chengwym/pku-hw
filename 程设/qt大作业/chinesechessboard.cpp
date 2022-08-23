#include "chinesechessboard.h"

ChineseChessBoard::ChineseChessBoard()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 10; j++) {
            board[i][j] = empty;
         }
    }
    board[0][0] = redrook;
    board[1][0] = redknight;
    board[2][0] = redelephant;
    board[3][0] = redmandarin;
    board[4][0] = redking;
    board[5][0] = redmandarin;
    board[6][0] = redelephant;
    board[7][0] = redknight;
    board[8][0] = redrook;
    board[1][2] = redcannon;
    board[7][2] = redcannon;
    board[0][3] = redpawn;
    board[2][3] = redpawn;
    board[4][3] = redpawn;
    board[6][3] = redpawn;
    board[8][3] = redpawn;

    board[0][9] = blackrook;
    board[1][9] = blackknight;
    board[2][9] = blackelephant;
    board[3][9] = blackmandarin;
    board[4][9] = blackking;
    board[5][9] = blackmandarin;
    board[6][9] = blackelephant;
    board[7][9] = blackknight;
    board[8][9] = blackrook;
    board[1][7] = blackcannon;
    board[7][7] = blackcannon;
    board[0][6] = blackpawn;
    board[2][6] = blackpawn;
    board[4][6] = blackpawn;
    board[6][6] = blackpawn;
    board[8][6] = blackpawn;
    isFloat = false;
}

void ChineseChessBoard::changeChess(int x, int y, int i, int j) {
    board[i][j] = board[x][y];
    board[x][y] = empty;
}

void ChineseChessBoard::reStart(){
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 10; j++) {
            board[i][j] = empty;
         }
    }
    board[0][0] = redrook;
    board[1][0] = redknight;
    board[2][0] = redelephant;
    board[3][0] = redmandarin;
    board[4][0] = redking;
    board[5][0] = redmandarin;
    board[6][0] = redelephant;
    board[7][0] = redknight;
    board[8][0] = redrook;
    board[1][2] = redcannon;
    board[7][2] = redcannon;
    board[0][3] = redpawn;
    board[2][3] = redpawn;
    board[4][3] = redpawn;
    board[6][3] = redpawn;
    board[8][3] = redpawn;

    board[0][9] = blackrook;
    board[1][9] = blackknight;
    board[2][9] = blackelephant;
    board[3][9] = blackmandarin;
    board[4][9] = blackking;
    board[5][9] = blackmandarin;
    board[6][9] = blackelephant;
    board[7][9] = blackknight;
    board[8][9] = blackrook;
    board[1][7] = blackcannon;
    board[7][7] = blackcannon;
    board[0][6] = blackpawn;
    board[2][6] = blackpawn;
    board[4][6] = blackpawn;
    board[6][6] = blackpawn;
    board[8][6] = blackpawn;
    isFloat = false;
}
