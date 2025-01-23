#include "mainwindow.h"
#include "ui_mainwindow.h"


BouncingButton::BouncingButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
    setStyleSheet(R"(
        QPushButton {
            color: white;
            font-size: 20px;
            font-weight: bold;
            padding: 15px 30px;
            background-color: #8B4513;
            border: 3px solid #DCA56A;
            border-radius: 20px;
            min-width: 200px;
            margin: 10px;
        }
        QPushButton:hover {
            background-color: #A0522D;
            border-color: #FFD700;
        }
        QPushButton:pressed {
            background-color: #CD853F;
            border-color: #DAA520;
            box-shadow: inset 0px 3px 6px rgba(0, 0, 0, 0.5);
        }
    )");

    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(700);
    animation->setLoopCount(-1);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
}

void BouncingButton::enterEvent(QEnterEvent *event) {
    QRect startRect = geometry();
    QRect endRect = startRect;
    endRect.moveTop(startRect.top() - 5);

    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->start();

    QPushButton::enterEvent(event);
}

void BouncingButton::leaveEvent(QEvent *event) {
    animation->stop();
    setGeometry(animation->startValue().toRect());
    QPushButton::leaveEvent(event);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Tower Defense", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; color: #FF4433; font-weight: bold; margin-bottom: 20px;");
    layout->addWidget(titleLabel);

    BouncingButton *startButton = new BouncingButton("Start", this);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    layout->addWidget(startButton);

    BouncingButton *exitButton = new BouncingButton("Exit", this);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitButtonClicked);
    layout->addWidget(exitButton);

    layout->setAlignment(Qt::AlignCenter);
    centralWidget->setLayout(layout);

    setCentralWidget(centralWidget);

    updateBackground();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateBackground() {
    QPixmap background(":/new/prefix2/back4.jpg");
    QPixmap scaledBackground = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(scaledBackground));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

void MainWindow::onStartButtonClicked() {
    GameMap *gamemap = new GameMap();
    gamemap->initialize();
    gamemap->show();
    gamemap->spawnEnemyWave();

    this->close();
    qDebug("Start Game!");
}

void MainWindow::onExitButtonClicked() {
    qDebug("Exit Game!");
    QApplication::quit();
}


