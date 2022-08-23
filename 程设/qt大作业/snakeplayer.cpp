#include "snakeplayer.h"

SnakePlayer::SnakePlayer(int _width, int _height, Direction _dir, QVector2D head, QVector2D tail, QVector3D _front_color, QVector3D _back_color)
    : width(_width)
    , height(_height)
    , front_color(_front_color)
    , back_color(_back_color)
    , dir(_dir)
{
    over = false;
    turn = false;
    score = 0;
    snake.push_back(head);
    snake.push_back(tail);
}

void SnakePlayer::ProcessInput() {
    if (turn) {
        if (key.empty() == true) {
            return;
        }
        Direction turn_dir = key.top();

        if (turn_dir == Direction::UP) {
            if (dir == Direction::LEFT || dir == Direction::RIGHT) {
                dir = Direction::UP;
                turn = false;
            }
        }
        else if (turn_dir == Direction::DOWN) {
            if (dir == Direction::LEFT || dir == Direction::RIGHT) {
                dir = Direction::DOWN;
                turn = false;
            }
        }
        else if (turn_dir == Direction::LEFT) {
            if (dir == Direction::UP || dir == Direction::DOWN) {
                dir = Direction::LEFT;
                turn = false;
            }
        }
        else if (turn_dir == Direction::RIGHT) {
            if (dir == Direction::UP || dir == Direction::DOWN) {
                dir = Direction::RIGHT;
                turn = false;
            }
        }
        while(key.empty() == false) {
            key.pop();
        }
    }
}

void SnakePlayer::moveSnake() {
    snake.pop_back();
    QVector2D head = snake.front();
    if (dir == Direction::UP) {
        int y = static_cast<int>(head.y()) + 1;
        while (y > height) {
            y -= height;
        }
        head.setY(y);
    }
    else if (dir == Direction::DOWN) {
        int y = static_cast<int>(head.y()) - 1;
        while (y < 1) {
            y += height;
        }
        head.setY(y);
    }
    else if (dir == Direction::LEFT) {
        int x = static_cast<int>(head.x()) - 1;
        while (x < 1) {
            x += width;
        }
        head.setX(x);
    }
    else if (dir == Direction::RIGHT) {
        int x = static_cast<int>(head.x()) + 1;
        while (x > width) {
            x -= width;
        }
        head.setX(x);
    }
    snake.push_front(head);

    turn = true;
}

void SnakePlayer::addTile(QVector2D tile) {
    snake.push_back(tile);
}

void SnakePlayer::addScore(int val) {
    score += val;
}

void SnakePlayer::defeat() {
    score = -1;
}

int SnakePlayer::getScore() {
    return score;
}
