#ifndef GOGAME_H
#define GOGAME_H

#include <QWidget>
#include <QUdpSocket>
#include <queue>
#include "gogameboard.h"

enum GoMessageType {
    GoState, GoClose
};

struct point{
    int x;
    int y;
};

namespace Ui {
class GoGame;
}

class QUdpSocket;

class GoGame : public QWidget
{
    Q_OBJECT

public:
    explicit GoGame(QWidget *parent = nullptr, QString ipAddress_ = "", int mode_ = 1);
    void setStone(StoneType stone);
    void setCurrentStone(StoneType stone);
    StoneType getStone();
    StoneType getCurrentStone();
    void putStone(int x, int y, StoneType stone);
    ~GoGame();

signals:
    void clicked(int, int);

private slots:
    void recieveGo();

private:
    Ui::GoGame *ui;

    QUdpSocket * udpSocket;
    void sendGo(GoMessageType type);
    qint16 port;
    QString ipAddress;
    int mode;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    void closeEvent(QCloseEvent *event) override;
    int row;
    int column;
    void removeStone(int x,int y);

    static const int padding = 30;
    static const int interval = 35;

    StoneType stone;
    StoneType currentStone;
    GoBoard * Go;

    bool ifPress(int x, int y);
    bool go_bfs(int x,int y,StoneType current,std::queue<point> &save);
    bool vis[20][20];//围棋算法访问标记
    std::queue<point> q;
    void go_check(int x, int y);
};

#endif // GOGAME_H
