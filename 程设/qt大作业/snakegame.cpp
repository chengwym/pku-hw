#include "snakegame.h"
#include <QIcon>
#include <QImage>
#include <random>
#include <QTime>
#include <QMessageBox>
#include <QPainter>
#include <QSound>

SnakeGame::SnakeGame(QWidget * parent, int size_, float speed_)
    : QOpenGLWidget(parent)
    , port(8081)
    , x_tile_count(size_)
    , speed(speed_)
{
    first = true;
    status = true;
    setFixedSize(1280, 720);
    setWindowIcon(QIcon(":/snake/img/snake_icon.png"));
    setWindowTitle("贪吃蛇");
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(recieveSnake()));
}

SnakeGame::~SnakeGame() {
    delete food;
    delete red_player;
    delete blue_player;
    delete time;
    delete shader;
    delete tile_texture;
    delete head_texture;
    delete core;
}

void SnakeGame::sendState() {
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);

    udpSocket->writeDatagram(data, data.length(), QHostAddress(ipAddress), port);
}

void SnakeGame::recieveSnake() {
    while(udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);

    }
}

void SnakeGame::initializeGL() {
    this->y_tile_count = this->x_tile_count * (static_cast<float>(this->height()) / static_cast<float>(this->width()));
    red_player = new SnakePlayer(x_tile_count, y_tile_count, Direction::UP, QVector2D(1, 2), QVector2D(1, 1), QVector3D(1.0f, 0.258823f, 0.0f), QVector3D(1.0f, 0.741177f, 0.0f));
    blue_player = new SnakePlayer(x_tile_count, y_tile_count, Direction::DOWN, QVector2D(x_tile_count, y_tile_count - 1), QVector2D(x_tile_count, y_tile_count), QVector3D(0.0f, 0.352941f, 1.0f), QVector3D(0.0f, 1.0f, 0.352941f));
    tile_width = static_cast<float>(this->width()) / static_cast<float>(x_tile_count);
    tile_height = static_cast<float>(this->height()) / static_cast<float>(y_tile_count);

    empty_tile.clear();
    for (int y = 1; y <= y_tile_count; ++y) {
        for (int x = 1; x <= x_tile_count; ++x) {
            if (x == 1 && (y == 1 || y == 2)) {
                continue;
            }
            if (x == x_tile_count && (y == y_tile_count || y == y_tile_count - 1)) {
                continue;
            }
            empty_tile.push_back(QVector2D(x, y));
        }
    }

    food = new QVector3D(x_tile_count / 2, y_tile_count / 2, 1);

    removeFromEmptyTile(QVector2D(food->x(), food->y()));

    time = new QTime();
    core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();
    shader = new SnakeShader(":/snake/shader/shader.vs", ":/snake/shader/shader.fs");

    tile_texture = new QOpenGLTexture(QImage(":/snake/img/tile.png"));
    if (!tile_texture->isCreated()) {
        qDebug() << "Failed to load texture" << endl;
    }
    tile_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    tile_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    tile_texture->setMinificationFilter(QOpenGLTexture::NearestMipMapNearest);
    tile_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    tile_texture->generateMipMaps();

    head_texture = new QOpenGLTexture(QImage(":/snake/img/tile.png"));
    if (!head_texture->isCreated()) {
        qDebug() << "Failed to load texture" << endl;
    }
    head_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    head_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    head_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    head_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    head_texture->generateMipMaps();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };
    core->glGenVertexArrays(1, &vao);
    core->glBindVertexArray(vao);
    unsigned int vbo;
    core->glGenBuffers(1, &vbo);
    core->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    core->glBindVertexArray(0);

    core->glEnable(GL_CULL_FACE);

    core->glEnable(GL_BLEND);
    core->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    core->glEnable(GL_DEPTH_TEST);
    core->glDepthMask(GL_TRUE);
    core->glDepthFunc(GL_LEQUAL);
    core->glDepthRange(0.0f, 1.0f);

    time->start();

    last_update = 0.0f;
}

void SnakeGame::resizeGL(int w, int h) {
    core->glViewport(0, 0, w, h);
}

void SnakeGame::paintGL() {
    core->glClearDepth(1.0f);
    core->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    red_player->ProcessInput();
    blue_player->ProcessInput();

    if (status) {
        updateGame();
    }

    drawBackground();

    drawSnake();

    drawFood();

    drawScore();

    if (status == false) {
        if (first) {
            first = false;
            if (red_player->getScore() > blue_player->getScore()) {
                QMessageBox::information(this, "游戏结束",  "红色方胜利");
            }
            else if (red_player->getScore() == blue_player->getScore()) {
                QMessageBox::information(this, "游戏结束",  "平局");
            }
            else {
                QMessageBox::information(this, "游戏结束",  "蓝色方胜利");
            }

            emit this->quitSnakeGame();
        }
    }
    else {
        update();
    }
}

void SnakeGame::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_W) {
        red_player->key.push(Direction::UP);
    }
    else if (event->key() == Qt::Key_S) {
        red_player->key.push(Direction::DOWN);
    }
    else if (event->key() == Qt::Key_A) {
        red_player->key.push(Direction::LEFT);
    }
    else if (event->key() == Qt::Key_D) {
        red_player->key.push(Direction::RIGHT);
    }

    if (event->key() == Qt::Key_Up) {
        blue_player->key.push(Direction::UP);
    }
    else if (event->key() == Qt::Key_Down) {
        blue_player->key.push(Direction::DOWN);
    }
    else if (event->key() == Qt::Key_Left) {
        blue_player->key.push(Direction::LEFT);
    }
    else if (event->key() == Qt::Key_Right) {
        blue_player->key.push(Direction::RIGHT);
    }
}

int Dis(QVector2D x, QVector2D y) {
    return qAbs(x.x() - y.x()) + qAbs(x.y() - y.y());
}

void SnakeGame::generateFood() {
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    unsigned int idx = rng() % empty_tile.size();
    while (Dis(empty_tile[idx], red_player->snake.front()) <= 2 || Dis(empty_tile[idx], blue_player->snake.front()) <= 2) {
        idx = rng() % empty_tile.size();
    }
    std::swap(empty_tile[idx], empty_tile[empty_tile.size() - 1]);
    food->setX(empty_tile.back().x());
    food->setY(empty_tile.back().y());
    food->setZ(food->z() + 1);
    empty_tile.pop_back();
}

void SnakeGame::drawBackground() {
    core->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QMatrix4x4 model_matrix;
    model_matrix.translate(this->width() * 0.5f, this->height() * 0.5f, 0.0f);
    model_matrix.scale(this->width(), this->height(), 1.0f);
    QMatrix4x4 projection_matrix;
    projection_matrix.ortho(0.0f, this->width(), 0.0f, this->height(), -1.0f, 1.0f);

    shader->use();
    shader->setUniformMat4("model", model_matrix);
    shader->setUniformMat4("projection", projection_matrix);
    shader->setUniformVec2("tiling", QVector2D(x_tile_count, y_tile_count));
    shader->setUniformVec4("color", QVector4D(0.3f, 0.3f, 0.3f, 1.0f));

    core->glActiveTexture(GL_TEXTURE0);
    tile_texture->bind();
    core->glBindVertexArray(vao);

    core->glDrawArrays(GL_TRIANGLES, 0, 6);
    core->glBindVertexArray(0);
}

QVector3D Mix(QVector3D x, QVector3D y, float a) {
    return QVector3D(x * (1.0f - a) + y * a);
}

void SnakeGame::drawSnake() {
    bool head;
    int length, idx;

    head = true;
    length = red_player->snake.size();
    idx = 0;
    for (auto coord : red_player->snake) {
        float percent = static_cast<float>(idx) / static_cast<float>(length);
        QVector2D scale(tile_width, tile_height);
        QVector3D color = Mix(red_player->front_color, red_player->back_color, percent);
        drawTile(QVector2D(coord.x() * scale.x() - scale.x() * 0.5f, coord.y() * scale.y() - scale.y() * 0.5f), QVector4D(color, 1.0f), head);

        head = false;
        ++idx;
    }

    head = true;
    length = blue_player->snake.size();
    idx = 0;
    for (auto coord : blue_player->snake) {
        float percent = static_cast<float>(idx) / static_cast<float>(length);
        QVector2D scale(tile_width, tile_height);
        QVector3D color = Mix(blue_player->front_color, blue_player->back_color, percent);
        drawTile(QVector2D(coord.x() * scale.x() - scale.x() * 0.5f, coord.y() * scale.y() - scale.y() * 0.5f), QVector4D(color, 1.0f), head);

        head = false;
        ++idx;
    }
}

void SnakeGame::drawTile(QVector2D pos, QVector4D color, bool head) {
    core->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QMatrix4x4 model_matrix;
    model_matrix.translate(QVector3D(pos, 1.0f));
    if (head) {
        model_matrix.scale(QVector3D(tile_width * 0.95f, tile_height * 0.95f, 1.0f));
    }
    else {
        model_matrix.scale(QVector3D(tile_width * 0.9f, tile_height * 0.9f, 1.0f));
    }
    QMatrix4x4 projection_matrix;
    projection_matrix.ortho(0.0f, this->width(), 0.0f, this->height(), -1.0f, 1.0f);

    shader->use();
    shader->setUniformMat4("model", model_matrix);
    shader->setUniformMat4("projection", projection_matrix);
    if (head) {
        shader->setUniformVec2("tiling", QVector2D(2.0f, 2.0f));
    }
    else {
        shader->setUniformVec2("tiling", QVector2D(1.0f, 1.0f));
    }
    shader->setUniformVec4("color", color);

    core->glActiveTexture(GL_TEXTURE0);
    if (head) {
        head_texture->bind();
    }
    else {
        tile_texture->bind();
    }
    core->glBindVertexArray(vao);

    core->glDrawArrays(GL_TRIANGLES, 0, 6);
    core->glBindVertexArray(0);
}

void SnakeGame::drawFood() {
    core->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QMatrix4x4 model_matrix;
    QVector2D scale(tile_width, tile_height);
    model_matrix.translate(QVector3D(food->x() * scale.x() - scale.x() * 0.5f, food->y() * scale.y() - scale.y() * 0.5f, 1.0f));
    model_matrix.scale(QVector3D(tile_width * 0.9f, tile_height * 0.9f, 1.0f));
    QMatrix4x4 projection_matrix;
    projection_matrix.ortho(0.0f, this->width(), 0.0f, this->height(), -1.0f, 1.0f);

    shader->use();
    shader->setUniformMat4("model", model_matrix);
    shader->setUniformMat4("projection", projection_matrix);
    shader->setUniformVec2("tiling", QVector2D(1.0f, 1.0f));
    shader->setUniformVec4("color", QVector4D(1.0f, 0.0f, 1.0f, 1.0f));

    core->glActiveTexture(GL_TEXTURE0);
    tile_texture->bind();
    core->glBindVertexArray(vao);

    core->glDrawArrays(GL_TRIANGLES, 0, 6);
    core->glBindVertexArray(0);
}

void SnakeGame::drawScore() {
    core->glDisable(GL_BLEND);

    core->glDisable(GL_DEPTH_TEST);

    QPainter painter(this);

    painter.setPen(QColor(255, 66, 0));
    painter.setFont(QFont("等线", 30, QFont::Bold));
    painter.drawText(300, 100, std::to_string(red_player->getScore()).c_str());

    painter.setPen(QColor(0, 90, 255));
    painter.setFont(QFont("等线", 30, QFont::Bold));
    painter.drawText(this->width() - 300, 100, std::to_string(blue_player->getScore()).c_str());

    painter.end();

    core->glEnable(GL_BLEND);
    core->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    core->glEnable(GL_DEPTH_TEST);
    core->glDepthMask(GL_TRUE);
    core->glDepthFunc(GL_LEQUAL);
    core->glDepthRange(0.0f, 1.0f);
}

bool SnakeGame::removeFromEmptyTile(QVector2D val) {
    auto it = std::find(empty_tile.begin(), empty_tile.end(), val);
    if (it == empty_tile.end()) {
        return false;
    }
    std::swap(*it, empty_tile[empty_tile.size() - 1]);
    empty_tile.pop_back();
    return true;
}

void SnakeGame::updateGame() {
    float current_time = static_cast<float>(time->elapsed()) / 100.0f;
    float delta_time = current_time - last_update;
    float step_timer = delta_time * speed;

    if (step_timer >= 1.0f) {
        last_update = current_time;

        QVector2D buf;
        bool red_eat_flag = false, blue_eat_flag = false;

        buf = red_player->snake.back();
        red_player->moveSnake();
        if (red_player->snake.front() == QVector2D(food->x(), food->y())) {
            QSound::play(":/snake/audio/big_bite.wav");
            red_eat_flag = true;
            red_player->addScore(food->z());
            generateFood();
            red_player->addTile(buf);
        }
        else {
            empty_tile.push_back(buf);
        }

        buf = blue_player->snake.back();
        blue_player->moveSnake();
        if (blue_player->snake.front() == QVector2D(food->x(), food->y())) {
            QSound::play(":/snake/audio/big_bite.wav");
            blue_eat_flag = true;
            blue_player->addScore(food->z());
            generateFood();
            blue_player->addTile(buf);
        }
        else {
            empty_tile.push_back(buf);
        }

        if (removeFromEmptyTile(red_player->snake.front()) == false && red_eat_flag == false) {
            if (red_player->snake.front() != blue_player->snake.front()) {
                red_player->over = true;
            }
            status = false;
            QSound::play(":/snake/audio/button.wav");
        }
        if (removeFromEmptyTile(blue_player->snake.front()) == false && blue_eat_flag == false) {
            if (blue_player->snake.front() != red_player->snake.front()) {
                blue_player->over = true;
            }
            status = false;
            QSound::play(":/snake/audio/button.wav");
        }

        if (red_player->over == true && blue_player->over == false) {
            red_player->defeat();
        }
        else if (blue_player->over == true && red_player->over == false) {
            blue_player->defeat();
        }
    }
}
