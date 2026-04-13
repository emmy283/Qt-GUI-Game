#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QVector>

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    QRect player;
    QVector<QRect> enemies;

    QTimer *timer;
    int score;
    bool gameOver;

    void spawnEnemy();
};

#endif
