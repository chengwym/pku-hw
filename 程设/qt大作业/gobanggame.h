#ifndef GOBANGGAME_H
#define GOBANGGAME_H

#include <QWidget>
#include <QUdpSocket>
#include "gogameboard.h"

enum GoBangMessageType {
    GoGameState, GoGameClose, GoGameWin
};

namespace Ui {
class GoBangGame;
}

class QUdpSocket;

class GoBangGame : public QWidget
{
    Q_OBJECT

public:
    explicit GoBangGame(QWidget *parent = nullptr, QString ipAddress_ = "", int mode = 1);
    void setStone(StoneType stone);
    void setCurrentStone(StoneType stone);
    StoneType getCurrentStone();
    StoneType getStone();

    void putStone(int x, int y, StoneType stone);
    ~GoBangGame();

signals:
    void clicked(int, int);

private slots:
    void recieveGoBang();

private:
    QUdpSocket * udpSocket;
    void sendGoBang(GoBangMessageType type);
    bool check_goGang(int x, int y);
    qint16 port;
    QString ipAddress;
    int mode;

    Ui::GoBangGame *ui;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    void closeEvent(QCloseEvent *event) override;
    int row;
    int column;



    static const int padding = 30;
    static const int interval = 35;

    StoneType stone;
    StoneType currentStone;
    FiveBoard * Five;
};

#endif // GOBANGGAME_H
