#include "gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), score(0), gameOver(false)
{
    setFixedSize(800, 600);
    setWindowTitle("Falling Objects — Mini Game");

    player = QRect(380, 520, 40, 40);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateGame);

    timer->start(16);  // ~60 FPS
}

GameWindow::~GameWindow() {}

// -----------------------------
// Paint Everything
// -----------------------------
void GameWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (gameOver) {
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 36, QFont::Bold));
        p.drawText(rect(), Qt::AlignCenter,
                   QString("GAME OVER\nScore: %1").arg(score));
        return;
    }

    // Background
    p.fillRect(rect(), QColor(20, 20, 40));

    // Player
    p.setBrush(QColor(80, 200, 255));
    p.drawRect(player);

    // Enemies
    p.setBrush(QColor(255, 80, 80));
    for (auto &e : enemies)
        p.drawEllipse(e);

    // Score
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 18));
    p.drawText(10, 30, QString("Score: %1").arg(score));
}

// -----------------------------
// User Input
// -----------------------------
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (gameOver) return;

    int step = 20;

    if (event->key() == Qt::Key_Left && player.left() > 0)
        player.moveLeft(player.x() - step);

    if (event->key() == Qt::Key_Right && player.right() < width())
        player.moveLeft(player.x() + step);

    if (event->key() == Qt::Key_Up && player.top() > 0)
        player.moveTop(player.y() - step);

    if (event->key() == Qt::Key_Down && player.bottom() < height())
        player.moveTop(player.y() + step);

    update();
}

// -----------------------------
// Spawn new enemies
// -----------------------------
void GameWindow::spawnEnemy()
{
    int x = rand() % (width() - 30);
    enemies.append(QRect(x, 0, 30, 30));
}

// -----------------------------
// Main game loop
// -----------------------------
void GameWindow::updateGame()
{
    if (rand() % 15 == 0)
        spawnEnemy();

    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].moveTop(enemies[i].y() + 5);

        if (enemies[i].intersects(player)) {
            gameOver = true;
            timer->stop();
        }

        if (enemies[i].y() > height()) {
            enemies.removeAt(i);
            score++;
        }
    }

    update();
}
