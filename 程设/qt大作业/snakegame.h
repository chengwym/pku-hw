#ifndef SNAKEGAME_H
#define SNAKEGAME_H
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QKeyEvent>
#include <QVector2D>
#include <QVector3D>
#include <QTime>
#include <vector>
#include <QUdpSocket>
#include "snakeshader.h"
#include "snakeplayer.h"

class QUdpSocket;

class SnakeGame : public QOpenGLWidget
{
    Q_OBJECT
public:
    SnakeGame(QWidget * parent, int size_, float speed_);
    ~SnakeGame();

signals:
    void quitSnakeGame();

private slots:
    void recieveSnake();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent* event) override;

private:

    QUdpSocket * udpSocket;
    QString ipAddress;
    int mode;
    qint16 port;
    void sendState();

    bool status;
    bool first;

    unsigned int vao;
    int x_tile_count;
    int y_tile_count;
    float tile_width;
    float tile_height;
    float speed;

    std::vector<QVector2D> empty_tile;

    QVector3D* food;

    SnakePlayer * red_player;
    SnakePlayer * blue_player;

    QTime * time;
    float last_update;

    SnakeShader* shader;
    QOpenGLTexture* tile_texture;
    QOpenGLTexture* head_texture;
    QOpenGLFunctions_4_5_Core* core;

    void generateFood();
    void drawBackground();
    void drawSnake();
    void drawTile(QVector2D pos, QVector4D color, bool head = false);
    void drawFood();
    void drawScore();
    bool removeFromEmptyTile(QVector2D val);
    void updateGame();

};

#endif // SNAKEGAME_H
