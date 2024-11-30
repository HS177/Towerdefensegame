#ifndef MAINMENU_
#define MAINMENU_

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QLabel>
#include <QDebug>
#include <QPropertyAnimation>
#include <QEvent>
#include <QEnterEvent>
#include"Gamemap.h"
#include "Enemy.h"

#include <QResizeEvent>
#include <QPalette>
#include <QPixmap>

class BouncingButton : public QPushButton
{
    Q_OBJECT

public:

    BouncingButton(const QString &text, QWidget *parent = nullptr) : QPushButton(text, parent)
    {

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

protected:
    void enterEvent(QEnterEvent *event) override
    {

        QRect startRect = geometry();
        QRect endRect = startRect;
        endRect.moveTop(startRect.top() - 5);

        animation->setStartValue(startRect);
        animation->setEndValue(endRect);
        animation->start();

        QPushButton::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override
    {

        animation->stop();
        setGeometry(animation->startValue().toRect());

        QPushButton::leaveEvent(event);
    }

private:
    QPropertyAnimation *animation;


};



class MainMenu : public QWidget {
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);


        QLabel *titleLabel = new QLabel("Tower Defense", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 32px; color: #FF4433; font-weight: bold; margin-bottom: 20px;");
        layout->addWidget(titleLabel);


        BouncingButton *startButton = new BouncingButton("Start", this);
        connect(startButton, &QPushButton::clicked, this, &MainMenu::onStartButtonClicked);
        layout->addWidget(startButton);


        BouncingButton *exitButton = new BouncingButton("Exit", this);
        connect(exitButton, &QPushButton::clicked, this, &MainMenu::onExitButtonClicked);
        layout->addWidget(exitButton);


        setStyleSheet(R"(
            MainMenu {
                background-image: url(:/C:\Users\Administrator\Desktop\Screenshot (38).png);
                background-position: center;
                background-repeat: no-repeat;
                background-size: cover;
                color: #D8D8DF;
            }
        )");

        layout->setAlignment(Qt::AlignCenter);
        setLayout(layout);
    }

protected:

    void resizeEvent(QResizeEvent *event) {
        QWidget::resizeEvent(event);
        updateBackground();
    }

private:
    void updateBackground() {

        QPixmap background(":/new/prefix2/back4.jpg");


        QPixmap scaledBackground = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        QPalette palette;
        palette.setBrush(this->backgroundRole(), QBrush(scaledBackground));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }

private slots:
    void onStartButtonClicked() {
        GameMap *gamemap = new GameMap();

        gamemap->initialize();
        gamemap->show();
        gamemap->spawnEnemy();

        this->close();
        qDebug("Start Game!");
    }

    void onExitButtonClicked() {
        qDebug("Exit Game!");
        QApplication::quit();
    }
};


#endif
