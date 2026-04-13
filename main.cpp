#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include <vector>
#include <random>

// ---------------- Ball Class ----------------
class Ball {
public:
    Ball(int w, int h) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> radiusDist(20,50);
        std::uniform_int_distribution<> posXDist(0, w);
        std::uniform_int_distribution<> posYDist(0, h);
        std::uniform_int_distribution<> velDist(2,6);
        std::uniform_int_distribution<> colorDist(0,255);
        std::uniform_int_distribution<> signDist(0,1);

        radius = radiusDist(gen);
        x = radius + posXDist(gen) % (w - 2*radius);
        y = radius + posYDist(gen) % (h - 2*radius);
        vx = velDist(gen) * (signDist(gen) ? 1 : -1);
        vy = velDist(gen) * (signDist(gen) ? 1 : -1);
        color.setRgb(colorDist(gen), colorDist(gen), colorDist(gen));
    }

    void move(int w, int h) {
        x += vx;
        y += vy;

        if (x - radius < 0) { x = radius; vx = -vx; }
        if (x + radius > w) { x = w - radius; vx = -vx; }
        if (y - radius < 0) { y = radius; vy = -vy; }
        if (y + radius > h) { y = h - radius; vy = -vy; }
    }

    bool contains(int mx, int my) {
        int dx = mx - x;
        int dy = my - y;
        return dx*dx + dy*dy <= radius*radius;
    }

    int x, y, vx, vy, radius;
    QColor color;
};

// ---------------- Game Canvas ----------------
class GameCanvas : public QWidget {
    Q_OBJECT
public:
    GameCanvas(QWidget *parent=nullptr) : QWidget(parent), score(0), paused(false), speedFactor(1.0) {
        setFixedSize(600, 400);

        for(int i=0;i<5;i++) balls.push_back(Ball(width(), height()));

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &GameCanvas::updateAnimation);
        timer->start(16); // ~60 FPS
    }

    void setSpeedFactor(double factor) { speedFactor = factor; }

    int getScore() const { return score; }
    void togglePause() { paused = !paused; }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Background gradient
        QLinearGradient grad(0,0,0,height());
        grad.setColorAt(0, QColor(50,50,80));
        grad.setColorAt(1, QColor(30,30,50));
        p.fillRect(rect(), grad);

        // Draw balls
        for(auto &b : balls){
            p.setBrush(b.color);
            p.setPen(Qt::NoPen);
            p.drawEllipse(b.x - b.radius, b.y - b.radius, b.radius*2, b.radius*2);
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        if(paused) return;
        for(size_t i=0;i<balls.size();i++){
            if(balls[i].contains(event->x(), event->y())){
                balls.erase(balls.begin() + i);
                score += 1;
                balls.push_back(Ball(width(), height())); // spawn new ball
                emit scoreChanged(score);
                break;
            }
        }
    }

private slots:
    void updateAnimation() {
        if(paused) return;
        for(auto &b : balls){
            b.x += int(b.vx * speedFactor);
            b.y += int(b.vy * speedFactor);

            if (b.x - b.radius < 0) { b.x = b.radius; b.vx = -b.vx; }
            if (b.x + b.radius > width()) { b.x = width() - b.radius; b.vx = -b.vx; }
            if (b.y - b.radius < 0) { b.y = b.radius; b.vy = -b.vy; }
            if (b.y + b.radius > height()) { b.y = height() - b.radius; b.vy = -b.vy; }
        }
        update();
    }

signals:
    void scoreChanged(int);

private:
    std::vector<Ball> balls;
    QTimer *timer;
    int score;
    bool paused;
    double speedFactor;
};

// ---------------- Main Window ----------------
class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent=nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        canvas = new GameCanvas(this);
        layout->addWidget(canvas);

        QHBoxLayout *controls = new QHBoxLayout();

        QLabel *scoreLabel = new QLabel("Score: 0", this);
        controls->addWidget(scoreLabel);

        QPushButton *pauseBtn = new QPushButton("Pause/Resume", this);
        controls->addWidget(pauseBtn);

        QSlider *speedSlider = new QSlider(Qt::Horizontal, this);
        speedSlider->setRange(50,200); // speed factor 0.5x - 2x
        speedSlider->setValue(100);
        controls->addWidget(new QLabel("Speed:"));
        controls->addWidget(speedSlider);

        layout->addLayout(controls);

        connect(canvas, &GameCanvas::scoreChanged, [scoreLabel](int score){
            scoreLabel->setText("Score: " + QString::number(score));
        });
        connect(pauseBtn, &QPushButton::clicked, canvas, &GameCanvas::togglePause);
        connect(speedSlider, &QSlider::valueChanged, [this](int val){
            canvas->setSpeedFactor(val / 100.0);
        });
    }

private:
    GameCanvas *canvas;
};

// ---------------- Main ----------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"
