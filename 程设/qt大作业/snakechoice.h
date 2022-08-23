#ifndef SNAKECHOICE_H
#define SNAKECHOICE_H

#include <QWidget>
#include <QSound>
#include "snakegame.h"

namespace Ui {
class SnakeChoice;
}

class SnakeChoice : public QWidget
{
    Q_OBJECT

public:
    explicit SnakeChoice(QWidget *parent = nullptr);
    ~SnakeChoice();

    void paintEvent(QPaintEvent *) override;

signals:
    void deleteSnakeGame();

private:
    Ui::SnakeChoice *ui;
    QSound* bg_Sound;
    void closeEvent(QCloseEvent *event) override;
};

#endif // SNAKECHOICE_H
