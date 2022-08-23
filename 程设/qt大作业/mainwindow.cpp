#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chat.h"
#include <QDebug>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("LoversGame");
    setWindowIcon(QIcon(":/login/img/login.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_join_clicked()
{
    ipAdress = ui->ipInput->toPlainText();
    port = ui->portInput->toPlainText().toInt();
    qDebug() << ipAdress;
    chat * Chat = new chat(nullptr, ipAdress, port);
    Chat->show();
    this->hide();
}
