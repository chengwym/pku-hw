#include "gogame.h"
#include "ui_gogame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>
#include <QIcon>

GoGame::GoGame(QWidget *parent, QString ipAddress_, int mode_) :
    QWidget(parent),
    ui(new Ui::GoGame),
    port(8081),
    ipAddress(ipAddress_),
    mode(mode_),
    row(19),
    column(19)
{
    ui->setupUi(this);
    Go = new GoBoard();
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(recieveGo()));
    if (mode == 1) {
        currentStone = BLACK;
        stone = BLACK;
    } else if (mode == 2) {
        currentStone = BLACK;
        stone = WHITE;
    }
    setWindowIcon(QIcon(":/go/img/go.png"));
    setWindowTitle("围棋");
}

GoGame::~GoGame()
{
    delete ui;
}

void GoGame::sendGo(GoMessageType type) {
    qDebug() << "Go send";
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << type;
    switch (type) {
    case GoState:{
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                out << Go->board[i][j];
            }
        }
        break;
    }
    case GoClose: {
        break;
    }
    }
    udpSocket->writeDatagram(data, data.length(), QHostAddress(ipAddress), port);
}

void GoGame::recieveGo() {
    while(udpSocket->hasPendingDatagrams()) {
        qDebug() << "Go recieve";
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int type;
        in >> type;
        switch (type) {
        case GoState:{
            for (int i = 0; i < 15; i++) {
                for (int j = 0; j < 15; j++) {
                    in >> Go->board[i][j];
                }
            }
            update();
            break;
        }
        case GoClose: {
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

void GoGame::paintEvent(QPaintEvent *) {
    const int row_pix = padding + interval * (row - 1) + padding;
    const int column_pix = padding + interval * (column - 1) +padding;

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
    painter.drawEllipse(QPoint(padding+3*interval,padding+center*interval),4,4);
    painter.drawEllipse(QPoint(padding+(row-4)*interval,padding+center*interval),4,4);
    painter.drawEllipse(QPoint(padding+center*interval,padding+3*interval),4,4);
    painter.drawEllipse(QPoint(padding+center*interval,padding+(row-4)*interval),4,4);

    for(int i = 0; i < row; i++){
        for(int j = 0; j < row; j++){
            StoneType current = Go->board[i][j];
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

void GoGame::mouseReleaseEvent(QMouseEvent *event) {
    if(this->currentStone!=this->stone){
       return;
    }
    int GridSize=(height()-padding*2)/(row-1);
    size_t row=(event->x()-17)/GridSize;
    size_t column=(event->y()-17)/GridSize;
    if (ifPress(row, column))
    {
        Go->putStone(row,column,stone);
        go_check(row,column);
        update();
        sendGo(GoState);
        if(currentStone==WHITE)
            currentStone=BLACK;
        else if(currentStone==BLACK)
            currentStone=WHITE;
    }
}

void GoGame::setStone(StoneType stone){
    this->stone = stone;

}

StoneType GoGame::getStone(){
    return stone;
}

void GoGame::putStone(int x, int y,StoneType stone){
    Go->putStone(x,y,stone);
    update();
}

void GoGame::removeStone(int x, int y){
    Go->removeStone(x,y);
    update();
}

void GoGame::setCurrentStone(StoneType stone){
    this->currentStone=stone;
}

StoneType GoGame::getCurrentStone(){
    return currentStone;
}

void GoGame::closeEvent(QCloseEvent *) {
    sendGo(GoClose);
    udpSocket->close();
}

bool GoGame::ifPress(int x, int y) {
    int dir[4][2]= {{1, 0},{-1, 0},{0, -1},{0, 1}};
    bool ans = true;
    for (int i=0;i<4;i++) {
        int a = x +dir[i][0];
        int b = y +dir[i][1];
        if (a < 0 || b < 0 || a >= 19 || b >= 19) continue;
        ans = ans && (Go->board[a][b] != Go->board[x][y] && Go->board[a][b] != NONE);
    }
    if (ans) return false;
    else return true;
}

bool GoGame::go_bfs(int x,int y,StoneType current,std::queue<point> &save){
    //广度优先搜索，如果当前一片区域的棋子没有气，则remove
    int dir[4][2]= {{-1,0},{1,0},{0,-1},{0,1}};//记录四个方向
    point temp;

    //把当前坐标加入队列
    temp.x=x,temp.y=y;
    q.push(temp);
    save.push(temp);
    while(!q.empty())
    {
        //获取队头坐标
        temp=q.front();
        q.pop();
        x=temp.x;
        y=temp.y;
        vis[x][y]=true;
        //对当前坐标的周围进行查看
        for(int i=0; i<4; i++)
        {
            int u,v;
            u=x+dir[i][0];
            v=y+dir[i][1];
            //坐标 u,v 没有棋子 代表有气存在 返回true
            if(this->Go->checkStone(u,v)!=BLACK&&this->Go->checkStone(u,v)!=WHITE)
                return true;
            //否则 将坐标u，v加入队列
            else if(this->Go->checkStone(u,v)==current&&!vis[u][v])
            {
                temp.x=u;
                temp.y=v;
                q.push(temp);
                save.push(temp);
            }
        }//end for
    }
    return false;

}

void GoGame::go_check(int x, int y){
    int dir[4][2]= {{-1,0},{1,0},{0,-1},{0,1}};//记录四个方向
    int u,v;
    //初始化vis
    for(int i=0;i<19;i++)
        for(int j=0;j<19;j++)
            vis[i][j]=false;

    std::queue<point> save;//保存要拿走的棋子

    //1.先看周围的对手的棋子是否有 没有气的区域，有的话则拿走
    for(int i=0; i<4; i++)
    {
        u=x+dir[i][0];
        v=y+dir[i][1];
        if(u<0||u>19||v<0||v>19)
            continue;

        //如果是没有访问过的对手的棋子，进行广搜
        if(Go->checkStone(u,v)!=NONE&&this->Go->checkStone(x,y)!=this->Go->checkStone(u,v)&&!vis[u][v]){

            if(go_bfs(u,v,this->Go->checkStone(u,v),save)){//当前区域有气，则把save和q清空
                while(!save.empty())
                    save.pop();
                while(!q.empty())
                    q.pop();
            }else
            {//当前区域没有气，则将save中的坐标加入json串中并拿走棋子

                while(!save.empty())
                {
                    point temp=save.front();
                    save.pop();
                    Go->removeStone(temp.x,temp.y);
                }
            }
        }
    }
   // qDebug()<<x<<y<<"after";
    //2.再查看当前棋子的区域有没有气，没有气则移除
    if(go_bfs(x,y,this->Go->checkStone(x,y),save)){
        while(!save.empty())
            save.pop();
        while(!q.empty())
            q.pop();
    }
    while(!save.empty())
    {
        point temp=save.front();
        save.pop();
        Go->removeStone(temp.x,temp.y);
    }
  //  qDebug()<<x<<y<<"end";
}
