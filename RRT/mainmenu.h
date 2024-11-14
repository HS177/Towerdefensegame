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

class BouncingButton : public QPushButton
{
    Q_OBJECT

public:
    BouncingButton(const QString &text, QWidget *parent = nullptr) : QPushButton(text, parent)
    {
        // Default styling for button
        setStyleSheet(R"(
            QPushButton {
                color: white;
                font-size: 20px;
                font-weight: bold;
                padding: 15px 30px;
                background-color: #8B4513; /* SaddleBrown color for wood effect */
                border: 3px solid #DCA56A; /* Gold border */
                border-radius: 20px;
                min-width: 200px;
                margin: 10px;
            }
            QPushButton:hover {
                background-color: #A0522D; /* Darker shade for hover */
                border-color: #FFD700; /* Brighter gold on hover */
            }
            QPushButton:pressed {
                background-color: #CD853F; /* Lighter shade when pressed */
                border-color: #DAA520; /* Soft gold border */
                box-shadow: inset 0px 3px 6px rgba(0, 0, 0, 0.5); /* Inner shadow effect */
            }
        )");

        // Initialize the animation
        animation = new QPropertyAnimation(this, "geometry");
        animation->setDuration(700);
        animation->setLoopCount(-1); // Infinite loop for continuous bounce
        animation->setEasingCurve(QEasingCurve::InOutQuad);
    }

protected:
    void enterEvent(QEnterEvent *event) override
    {
        // Start the bounce animation when the mouse enters
        QRect startRect = geometry();
        QRect endRect = startRect;
        endRect.moveTop(startRect.top() - 5); // Move up by 5 pixels

        animation->setStartValue(startRect);
        animation->setEndValue(endRect);
        animation->start();

        QPushButton::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override
    {
        // Stop the bounce animation when the mouse leaves
        animation->stop();
        setGeometry(animation->startValue().toRect()); // Reset to original position

        QPushButton::leaveEvent(event);
    }

private:
    QPropertyAnimation *animation;


};

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr) : QWidget(parent)
    {
        // Set up the main layout
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Title Label
        QLabel *titleLabel = new QLabel("Tower Defense", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 32px; color: #FFD700; font-weight: bold; margin-bottom: 20px;"); // Gold color for title
        layout->addWidget(titleLabel);

        // Start Button
        BouncingButton *startButton = new BouncingButton("Start", this);
        connect(startButton, &QPushButton::clicked, this, &MainMenu::onStartButtonClicked);
        layout->addWidget(startButton);

        // Exit Button
        BouncingButton *exitButton = new BouncingButton("Exit", this);
        connect(exitButton, &QPushButton::clicked, this, &MainMenu::onExitButtonClicked);
        layout->addWidget(exitButton);

        // Background styling with image
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

signals:
    void startGame();

private slots:
    void onStartButtonClicked() {
        GameMap *gamemap = new GameMap();



        gamemap->initialize();
        gamemap->show();
        gamemap->spawnEnemy();
        this->close();
        qDebug("Start Game!");
    }

    void onExitButtonClicked()
    {
        qDebug("Exit Game!");
        QApplication::quit();
    }
};

#endif
