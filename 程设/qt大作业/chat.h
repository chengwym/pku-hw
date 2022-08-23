#ifndef CHAT_H
#define CHAT_H

#include <QWidget>

namespace Ui {
class chat;
}

enum MessageType{
    Message, Snake, Gobang, ChineseChess, Go
};

class QUdpSocket;

class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(QWidget *parent = nullptr, QString ipAddress = "", qint16 port = 0);
    ~chat();

private slots:
    void on_Snake_clicked();

    void on_ChineseChess_clicked();

    void on_GoBang_clicked();

    void on_Go_clicked();

    void on_sendMessage_clicked();

    void processPendingDatagrams();

private:
    Ui::chat *ui;
    QString ipAddress;
    qint16 port;
    QUdpSocket * udpSocket;
    void sendMessage(MessageType type);
    QString getMessage();
    QString getIp();
    QString getUserName();
};

#endif // CHAT_H
