#include <QUdpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include <QDebug>
#include <QIcon>
#include "snakechoice.h"
#include "chinesechessgame.h"
#include "gobanggame.h"
#include "gogame.h"
#include "chat.h"
#include "ui_chat.h"

chat::chat(QWidget *parent, QString ipAddress_, qint16 port_) :
    QWidget(parent),
    ui(new Ui::chat),
    ipAddress(ipAddress_),
    port(port_)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    setWindowTitle("LoversGame");
    setWindowIcon(QIcon(":/chat/img/chat.png"));
}

chat::~chat()
{
    delete ui;
}

void chat::sendMessage(MessageType type){
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << type;
    switch (type) {
        case Message:{
            qDebug() << "send";
            if (ui->MessageEdit->toPlainText() == "") {
                QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
                return;
            }
            out << getMessage() << getUserName();
            ui->MessageBrowser->verticalScrollBar()->setValue(ui->MessageBrowser->verticalScrollBar()->maximum());
            break;
        }
        case Snake: {
            break;
        }
        case Gobang: {
            break;
        }
        case ChineseChess: {
            break;
        }
        case Go: {
            break;
        }
    }
    udpSocket->writeDatagram(data, data.length(), QHostAddress(ipAddress), port);
}

void chat::processPendingDatagrams(){
    while(udpSocket->hasPendingDatagrams()) {
        qDebug() << "123";
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int messageType;
        in >> messageType;

        switch (messageType) {
            case Message:{
                qDebug() << "recievedChat";
                QString message, userName;
                QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                in >> message >> userName;
                ui->MessageBrowser->setTextColor(Qt::blue);
                ui->MessageBrowser->setCurrentFont(QFont("Times New Roman", 12));
                ui->MessageBrowser->append("[" + userName + "]" + time);
                ui->MessageBrowser->append(message);
                break;
            }
            case Snake: {
                SnakeChoice * snakeChoice = new SnakeChoice();
                snakeChoice->show();
                break;
            }
            case Gobang: {
                GoBangGame * goBangGame = new GoBangGame(nullptr, ipAddress, 2);
                goBangGame->show();
                break;
            }
            case ChineseChess: {
                ChineseChessGame * chineseChessGame = new ChineseChessGame(nullptr, ipAddress, 2);
                chineseChessGame->show();
                break;
            }
            case Go: {
                GoGame * goGame = new GoGame(nullptr, ipAddress, 2);
                goGame->show();
                break;
            }
        }
    }
}

QString chat::getMessage() {
    QString msg = ui->MessageEdit->toHtml();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->MessageEdit->clear();
    ui->MessageEdit->setFocus();
    ui->MessageBrowser->setTextColor(Qt::blue);
    ui->MessageBrowser->setCurrentFont(QFont("Times New Roman", 12));
    ui->MessageBrowser->append("[" + getUserName() + "]" + time);
    ui->MessageBrowser->append(msg);
    return msg;
}

QString chat::getIp(){
    QList<QHostAddress>list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list){
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return "";
}

QString chat::getUserName(){
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*" << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach(QString string,envVariables){
        int index = environment.indexOf(QRegExp(string));
        if (index != -1){
            QStringList stringList = environment.at(index).split('=');
            if(stringList.size() == 2){
                return stringList.at(1);
            }
        }
    }
    return "";
}

void chat::on_Snake_clicked()
{
    sendMessage(Snake);
    SnakeChoice * snakeChoice = new SnakeChoice();
    snakeChoice->show();
}

void chat::on_ChineseChess_clicked()
{
    sendMessage(ChineseChess);
    ChineseChessGame * chineseChessGame = new ChineseChessGame(nullptr, ipAddress, 1);
    chineseChessGame->show();
}

void chat::on_GoBang_clicked()
{
    sendMessage(Gobang);
    GoBangGame * goBangGame = new GoBangGame(nullptr, ipAddress, 1);
    goBangGame->show();
}

void chat::on_Go_clicked()
{
    sendMessage(Go);
    GoGame * goGame = new GoGame(nullptr, ipAddress, 1);
    goGame->show();
}

void chat::on_sendMessage_clicked()
{
    sendMessage(Message);
}
