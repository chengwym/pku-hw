#include "snakechoice.h"
#include "ui_snakechoice.h"
#include "snakegame.h"
#include <QPainter>

int sizeSelect(int idx) {
    return 1 << (idx + 3);
}

float speedSelect(int idx) {
    return 0.5f * (idx + 1);
}

SnakeChoice::SnakeChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnakeChoice)
{
    ui->setupUi(this);
    bg_Sound = new QSound(":/snake/audio/bg.wav");
    bg_Sound->setLoops(QSound::Infinite);

    setFixedSize(1280, 720);
    setWindowIcon(QIcon(":/snake/img/snake_icon.png"));
    setWindowTitle("贪吃蛇");

    connect(ui->enter_button, &QPushButton::clicked, this, [=]() {
        SnakeGame * snakeGame = new SnakeGame(this, sizeSelect(ui->size_combox->currentIndex()), speedSelect(ui->speed_combox->currentIndex()));
        snakeGame->setFocus();
        snakeGame->show();
        bg_Sound->play();

        connect(snakeGame, &SnakeGame::quitSnakeGame, this, [=]() {
           bg_Sound->stop();
           emit this->deleteSnakeGame();
        });

        connect(this, &SnakeChoice::deleteSnakeGame, snakeGame, &QOpenGLWidget::close);
    });
}

SnakeChoice::~SnakeChoice()
{
    delete ui;
}

void SnakeChoice::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/snake/img/snake_bg.jpg");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
}

void SnakeChoice::closeEvent(QCloseEvent *) {
    bg_Sound->stop();
    close();
}
