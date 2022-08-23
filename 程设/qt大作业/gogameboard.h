#ifndef GOBANGBOARD_H
#define GOBANGBOARD_H

enum StoneType {
    NONE, BLACK, WHITE
};

template <int N>
class GoGameBoard
{
public:
    StoneType board[N][N];
    int size;
    GoGameBoard();
    bool putStone(int x, int y, StoneType stone);
    bool removeStone(int x, int y);
    StoneType checkStone(int x, int y);
    static int boardSize();
};

typedef GoGameBoard<19> GoBoard;
typedef GoGameBoard<15> FiveBoard;

template <int N>
GoGameBoard<N>::GoGameBoard()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = NONE;
        }
    }
    size = N;
}

template <int N>
bool GoGameBoard<N>::putStone(int x, int y, StoneType stone) {
    if (x < N && y < N && stone != NONE && checkStone(x, y) == NONE) {
        board[x][y] = stone;
        return true;
    }
    return false;
}

template <int N>
bool GoGameBoard<N>::removeStone(int x, int y) {
    if (x < N && y < N) {
        board[x][y] = NONE;
        return true;
    }
    return false;
}

template <int N>
StoneType GoGameBoard<N>::checkStone(int x, int y) {
    return board[x][y];
}

template <int N>
int GoGameBoard<N>::boardSize(){
    return N;
}


#endif // GOBANGBOARD_H
