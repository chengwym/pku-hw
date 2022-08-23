#ifndef CHINESECHESSBOARD_H
#define CHINESECHESSBOARD_H
#include <QtGlobal>

enum chess{
    redrook = -1,
    redknight = -2,
    redelephant = -3,
    redmandarin = -4,
    redking = -5,
    redcannon = -6,
    redpawn = -7,
    empty = 0,
    blackrook = 1,//车
    blackknight = 2,//马
    blackelephant = 3,//象
    blackmandarin = 4,//士
    blackking = 5,//将
    blackcannon = 6,//炮
    blackpawn = 7//卒
};

class ChineseChessBoard
{
public:
    ChineseChessBoard();
    void changeChess(int x,int y,int i,int j);
    void reStart();
    qint16 board[9][10];
    qint16 userColor;
    bool isFloat;
    int xFloat;
    int yFloat;
    bool myTurn;
};

#endif // CHINESECHESSBOARD_H
