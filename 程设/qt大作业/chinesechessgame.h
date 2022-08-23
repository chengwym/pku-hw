#ifndef CHINESECHESSGAME_H
#define CHINESECHESSGAME_H

#include <QWidget>
#include <QUdpSocket>
#include "chinesechessboard.h"

enum BoardMessageType {
    BoardState, BoardColor, BoardTurnFalse, BoardClose
};

namespace Ui {
class ChineseChessGame;
}

class QUdpSocket;

class ChineseChessGame : public QWidget
{
    Q_OBJECT

public:
    explicit ChineseChessGame(QWidget *parent = nullptr, QString ipAddress_ = "", int mode_ = 1);
    ~ChineseChessGame();
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
private slots:
    void recieveChinese();

private:
    Ui::ChineseChessGame *ui;
    ChineseChessBoard chessBoard;
    QUdpSocket * udpSocket;
    QString ipAddress;
    qint16 port;
    int mode;
    void sendBoardMessage(BoardMessageType type);
};

#endif // CHINESECHESSGAME_H
