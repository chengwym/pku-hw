#include "chinesechessgame.h"
#include "ui_chinesechessgame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QIcon>

ChineseChessGame::ChineseChessGame(QWidget *parent, QString ipAddress_, int mode_) :
    QWidget(parent),
    ui(new Ui::ChineseChessGame),
    ipAddress(ipAddress_),
    port(8081),
    mode(mode_)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(recieveChinese()));
    if (mode == 1) {
        chessBoard.userColor = -1;
        chessBoard.myTurn = true;
    } else if (mode == 2) {
        chessBoard.userColor = 1;
        chessBoard.myTurn = false;
    }
    setWindowIcon(QIcon(":/chinese/img/chess.png"));
    setWindowTitle("中国象棋");
}

ChineseChessGame::~ChineseChessGame()
{
    delete ui;
}

void ChineseChessGame::sendBoardMessage(BoardMessageType type) {
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    qDebug() << "sendchinese";
    out << type;
    switch (type) {
        case BoardState: {
            qint16 tempBoardcolor = -chessBoard.userColor;
            out << tempBoardcolor;
            for (int i=0;i<9;i++){
                for (int j=0;j<10;j++){
                    out << chessBoard.board[i][j];
                }
            }
            break;
        }
        case BoardColor: {
            break;
        }
        case BoardTurnFalse: {
            out << chessBoard.userColor;
            break;
        }
        case BoardClose:{
            break;
        }
    }
    udpSocket->writeDatagram(data, data.length(), QHostAddress(ipAddress), port);
}

void ChineseChessGame::recieveChinese() {
    while(udpSocket->hasPendingDatagrams()) {
        qDebug() << "recieved Chinese";
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int type;
        in >> type;
        switch (type) {
            case BoardState: {
                qint16 tempUserColor;
                in >> tempUserColor;
                for (int i=0;i<9;i++){
                    for (int j=0;j<10;j++){
                        in >> chessBoard.board[i][j];
                    }
                }
                update();
                if(tempUserColor == chessBoard.userColor)
                    chessBoard.myTurn = true;
                break;
            }
            case BoardColor: {
                if(chessBoard.userColor == 0) {
                    chessBoard.userColor = 1;
                    chessBoard.myTurn = false;
                }
                break;
            }
            case BoardTurnFalse: {
                qint16 tempUserColor;
                in >> tempUserColor;
                if(tempUserColor != chessBoard.userColor){
                    chessBoard.myTurn = false;
                }
                break;
            }
            case BoardClose: {
                udpSocket->close();
                close();
                break;
            }
        }
    }
}

void ChineseChessGame::paintEvent(QPaintEvent *) {
    int offx = 55, offy = 15, size = 67;
    QPixmap chessRed(":/chinese/img/redChess.png");
    QPixmap chessBlack(":/chinese/img/blackChess.png");
    QPixmap chessBoard_img(":/chinese/img/board.png");
    QPainter painter(this);
    painter.drawPixmap(0, 0, chessBoard_img);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 10; j++) {
            if (chessBoard.board[i][j] < 0) painter.drawPixmap(i*size+offx,j*size+offy+(j>4?15:0),chessRed,(-chessBoard.board[i][j]-1)*size,0,size,size);
            else if (chessBoard.board[i][j] > 0) painter.drawPixmap(i*size+offx,j*size+offy+(j>4?15:0),chessBlack,(chessBoard.board[i][j]-1)*size,0,size,size);
         }
    }
}

void ChineseChessGame::mousePressEvent(QMouseEvent *event) {
    int px = event->x()-51.5;
    int py = event->y()-10.5;
    int i = px / 67;
    int j;
    if (py < 303.5) {
        j = py / 67;
    } else {
        j = (py - 359.5) / 67 + 5;
    }
    if(event->button() == Qt::LeftButton) {
        if(chessBoard.isFloat == true) {
            if(chessBoard.board[i][j] == 0 || chessBoard.board[chessBoard.xFloat][chessBoard.yFloat] * chessBoard.board[i][j] < 0) {
                bool canMove = false;
                switch (abs(chessBoard.board[chessBoard.xFloat][chessBoard.yFloat])) {
                case 1:
                {
                    int dx = abs(i - chessBoard.xFloat);
                    int dy = abs(j - chessBoard.yFloat);
                    int count = -1;
                    if(dx == 0&&dy!=0) {
                        count = 0;
                        int step = (j>chessBoard.yFloat)?1:-1;
                        for (int ty = chessBoard.yFloat+step; (ty-j)*step<0;ty+=step){
                            if(chessBoard.board[i][ty] != 0) count++;
                        }
                    }
                    if(dy == 0&&dx!=0) {
                        count = 0;
                        int step = (i>chessBoard.xFloat)?1:-1;
                        for (int tx = chessBoard.xFloat+step; (tx-i)*step<0;tx+=step){
                            if(chessBoard.board[tx][j] != 0) count++;
                        }
                    }
                    if(count==0){
                        canMove = true;
                    }
                    break;
                }
                case 2:
                {
                    int dx = abs(i - chessBoard.xFloat);
                    int dy = abs(j - chessBoard.yFloat);
                    if((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
                        canMove = true;
                    }
                    break;
                }
                case 3:
                {
                    int dx = abs(i - chessBoard.xFloat);
                    int dy = abs(j - chessBoard.yFloat);
                    bool ifMove = false;
                    if(dx == 2 && dy == 2){
                        ifMove = (chessBoard.board[(i + chessBoard.xFloat)/2][(j + chessBoard.yFloat)/2] == 0);
                    }
                    if (ifMove && (chessBoard.board[chessBoard.xFloat][chessBoard.yFloat] > 0 ? j > 4:j <= 4)){
                        canMove = true;
                    }
                    break;
                }
                case 4:
                {
                    int dx = abs(i - chessBoard.xFloat);
                    int dy = abs(j - chessBoard.yFloat);
                    if((i>=3&&i<=5)&&(dx == 1 && dy == 1)&&(chessBoard.board[chessBoard.xFloat][chessBoard.yFloat] > 0 ? (j<=9&&j>=7):(j>=0&&j<=2))){
                        canMove = true;
                    }
                    break;
                }
                case 5:
                {
                    int dx = abs(i - chessBoard.xFloat);
                    int dy = abs(j - chessBoard.yFloat);
                    if((i>=3&&i<=5)&&((dx == 1 && dy == 0)||(dx == 0 && dy == 1))&&(chessBoard.board[chessBoard.xFloat][chessBoard.yFloat] > 0 ? (j<=9&&j>=7):(j>=0&&j<=2))){
                        canMove = true;
                    }
                    break;
                }
                case 6:
                {
                    int dx = abs(i - chessBoard.xFloat);
                    int dy = abs(j - chessBoard.yFloat);
                    int count = -1;
                    if(dx == 0&&dy!=0) {
                        count = 0;
                        int step = (j>chessBoard.yFloat)?1:-1;
                        for (int ty = chessBoard.yFloat+step; (ty-j)*step<0;ty+=step){
                            if(chessBoard.board[i][ty] != 0) count++;
                        }
                    }
                    if(dy == 0&&dx!=0) {
                        count = 0;
                        int step = (i>chessBoard.xFloat)?1:-1;
                        for (int tx = chessBoard.xFloat+step; (tx-i)*step<0;tx+=step){
                            if(chessBoard.board[tx][j] != 0) count++;
                        }
                    }
                    if((count==0&&chessBoard.board[i][j]==0)||(count==1&&(chessBoard.board[chessBoard.xFloat][chessBoard.yFloat] * chessBoard.board[i][j] < 0))){
                        canMove = true;
                    }
                    break;
                }
                case 7:
                {
                    int dx = abs(i - chessBoard.xFloat);
                    int dy = (j - chessBoard.yFloat);
                    int step = chessBoard.board[chessBoard.xFloat][chessBoard.yFloat] > 0 ? -1:1;
                    if((dy==step&&dx==0)||(dx==1&&dy==0)) {
                        canMove = true;
                    }
                    break;
                }
                }
                if(canMove) {
                    QFile file("chess.txt");
                    file.open(QIODevice::ReadWrite);
                    QDataStream out(&file);
                    for (int i=0;i<9;i++){
                        for (int j=0;j<10;j++){
                            out << chessBoard.board[i][j];
                        }
                    }
                    chessBoard.changeChess(chessBoard.xFloat,chessBoard.yFloat,i,j);
                    update();
                    chessBoard.isFloat = false;
                    chessBoard.myTurn = false;
                    sendBoardMessage(BoardState);

                }
            } else if(chessBoard.board[chessBoard.xFloat][chessBoard.yFloat] * chessBoard.board[i][j] > 0){
                chessBoard.xFloat = i;
                chessBoard.yFloat = j;
            }
        } else if (chessBoard.board[i][j]*chessBoard.userColor>0 && chessBoard.myTurn){
            chessBoard.xFloat = i;
            chessBoard.yFloat = j;
            chessBoard.isFloat = true;
        }
    }
}

void ChineseChessGame::closeEvent(QCloseEvent *) {
    sendBoardMessage(BoardClose);
    udpSocket->close();
}
