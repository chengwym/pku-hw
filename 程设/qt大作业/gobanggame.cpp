#include "gobanggame.h"
#include "ui_gobanggame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QIcon>

bool GoBangGame::check_goGang(int x, int y) {
    for (int i = 0; i < 5; i++)
    {
        //纵向
        if(y - i >= 0 &&
                y + 4 - i <=15 &&
                Five->checkStone(x,y-i)==Five->checkStone(x,y + 1 - i)  &&
                Five->checkStone(x,y-i)==Five->checkStone(x,y + 2 - i) &&
                Five->checkStone(x,y-i)==Five->checkStone(x,y + 3 - i) &&
                Five->checkStone(x,y-i)==Five->checkStone(x,y + 4 - i))
            return true;

        //横行
        if(x - i >= 0 &&
                x + 4 - i <= 15 &&
                Five->checkStone(x - i,y)==Five->checkStone(x + 1 - i,y)  &&
                Five->checkStone(x - i,y)==Five->checkStone(x + 2 - i,y) &&
                Five->checkStone(x - i,y)==Five->checkStone(x + 3 - i,y) &&
                Five->checkStone(x - i,y)==Five->checkStone(x + 4 - i,y))
            return true;

        //左上到右下
        if(x - i >= 0 &&
                y - i >= 0 &&
                x + 4 - i <= 15 &&
                y + 4 - i <= 15 &&
                Five->checkStone(x - i,y - i)==Five->checkStone(x + 1 - i,y + 1 - i)  &&
                Five->checkStone(x - i,y - i)==Five->checkStone(x + 2 - i,y + 2 - i) &&
                Five->checkStone(x - i,y - i)==Five->checkStone(x + 3 - i,y + 3 - i) &&
                Five->checkStone(x - i,y - i)==Five->checkStone(x + 4 - i,y + 4 - i))
            return true;

        //从左下到右上
        if(x + i <= 0xF &&
                y - i >= 0 &&
                x - 4 + i >= 0 &&
                y + 4 - i <= 15 &&
                Five->checkStone(x + i,y - i)==Five->checkStone(x - 1 + i,y + 1 - i)  &&
                Five->checkStone(x + i,y - i)==Five->checkStone(x - 2 + i,y + 2 - i) &&
                Five->checkStone(x + i,y - i)==Five->checkStone(x - 3 + i,y + 3 - i) &&
                Five->checkStone(x + i,y - i)==Five->checkStone(x - 4 + i,y + 4 - i))
            return true;
    }

    return false;
}

GoBangGame::GoBangGame(QWidget *parent, QString ipAddress_, int mode_) :
    QWidget(parent),
    port(8081),
    ipAddress(ipAddress_),
    mode(mode_),
    ui(new Ui::GoBangGame),
    row(15),
    column(15)

{
    ui->setupUi(this);
    Five = new FiveBoard();
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(recieveGoBang()));
    if (mode == 1) {
        currentStone = BLACK;
        stone = BLACK;
    } else if (mode == 2) {
        currentStone = BLACK;
        stone = WHITE;
    }
    setWindowIcon(QIcon(":/go/img/go.png"));
    setWindowTitle("五子棋");
}

GoBangGame::~GoBangGame()
{
    delete ui;
}

void GoBangGame::sendGoBang(GoBangMessageType type) {
    qDebug() << "GOsend";
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << type;
    switch (type) {
    case GoGameState:{
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                out << Five->board[i][j];
            }
        }
        break;
    }
    case GoGameClose: {
        break;
    }
    case GoGameWin: {
        break;
    }
    }
    udpSocket->writeDatagram(data, data.length(), QHostAddress(ipAddress), port);
}

void GoBangGame::recieveGoBang() {
    while(udpSocket->hasPendingDatagrams()) {
        qDebug() << "Gorecieve";
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int type;
        in >> type;
        switch (type) {
        case GoGameState:{
            for (int i = 0; i < 15; i++) {
                for (int j = 0; j < 15; j++) {
                    in >> Five->board[i][j];
                }
            }
            break;
        }
        case GoGameClose: {
            udpSocket->close();
            close();
            break;
        }
        case GoGameWin: {
            QMessageBox::information(this, "游戏结束",  "失败");
            udpSocket->close();
            close();
            break;
        }
        }
        update();
        if(currentStone==WHITE)
            currentStone=BLACK;
        else if(currentStone==BLACK)
            currentStone=WHITE;
    }
}

void GoBangGame::paintEvent(QPaintEvent *) {
    const int row_pix = padding + interval * (row - 1) + padding;
    const int column_pix = padding + interval * (column - 1) +padding;
    qDebug() << "paint";
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawRect(padding, padding, interval * (row - 1), interval * (row - 1));

    pen.setWidth(1);
    painter.setPen(pen);
    for (int i = 0; i < row; i++) {
        painter.drawLine(padding, padding + i * interval, column_pix - padding, padding + i * interval);
    }
    for (int j = 0; j < column; j++) {
        painter.drawLine(padding + j * interval, padding, padding + j * interval, row_pix - padding);
    }

    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    int center = (row - 1) / 2;
    painter.drawEllipse(QPoint(padding + center * interval, padding + center * interval), 4, 4);
    painter.drawEllipse(QPoint(padding+3*interval,padding+3*interval),4,4);
    painter.drawEllipse(QPoint(padding+3*interval,padding+(row-4)*interval),4,4);
    painter.drawEllipse(QPoint(padding+(row-4)*interval,padding+(row-4)*interval),4,4);
    painter.drawEllipse(QPoint(padding+(row-4)*interval,padding+3*interval),4,4);

    for(int i = 0; i < row; i++){
        for(int j = 0; j < row; j++){
            StoneType current = Five->board[i][j];
            if(current == BLACK) {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint((i)*interval+padding,(j)*interval+padding),15,15);
            }
            else if(current == WHITE) {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint((i)*interval+padding,(j)*interval+padding),15,15);
            }
        }
    }
}

void GoBangGame::putStone(int x, int y, StoneType stone) {
    Five->putStone(x, y, stone);
    update();
}

StoneType GoBangGame::getStone() {
    return stone;
}

void GoBangGame::mouseReleaseEvent(QMouseEvent *event) {
    if(this->currentStone!=this->stone){
       return;
    }
    int GridSize=(height()-padding*2)/(row-1);
    size_t row=(event->x()-17)/GridSize;
    size_t column=(event->y()-17)/GridSize;
    Five->putStone(row,column,stone);
    update();
    sendGoBang(GoGameState);
    if (check_goGang(row, column)) {
        QMessageBox::information(this, "游戏结束",  "胜利");
        sendGoBang(GoGameWin);
        udpSocket->close();
        close();
    }
    if(currentStone==WHITE)
        currentStone=BLACK;
    else if(currentStone==BLACK)
        currentStone=WHITE;
}

void GoBangGame::setStone(StoneType stone) {
    this->stone = stone;
}

void GoBangGame::setCurrentStone(StoneType stone) {
    this->currentStone = stone;
}

StoneType GoBangGame::getCurrentStone() {
    return currentStone;
}

void GoBangGame::closeEvent(QCloseEvent *) {
    udpSocket->close();
    sendGoBang(GoGameClose);
}
