#ifndef SNAKEPLAYER_H
#define SNAKEPLAYER_H
#include <stack>
#include <QVector2D>
#include <QVector3D>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class SnakePlayer
{
public:
    bool over;

    int width;
    int height;

    bool turn;

    QVector3D front_color;
    QVector3D back_color;

    std::stack<Direction> key;

    std::list<QVector2D> snake;

    SnakePlayer(int _width, int _height, Direction _dir, QVector2D head, QVector2D tail, QVector3D _front_color, QVector3D _back_color);
    void ProcessInput();
    void moveSnake();
    void addTile(QVector2D tile);
    void addScore(int val);
    void defeat();
    int getScore();

private:
    int score;
    Direction dir;

};

#endif // SNAKEPLAYER_H
