
#include "Enemy.h"
#include <QGraphicsScene>
#include <QBrush>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QEasingCurve>
#include <QtMath>
#include <QLineF>



EnemyA::EnemyA(QPointF start, QPointF end, QObject *parent)
    : QObject(parent), startPoint(start), endPoint(end), pathIndex(0) {


    setRect(0, 0, 30, 30);
    setBrush(QBrush(QColor(200, 0, 0)));

    setPos(startPoint);


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EnemyA::move);


    setupPath();
}


void EnemyA::setupPath() {

     pathPoints = {
                       //QPointF(0, 200),
                       QPointF(0,150),
                       QPointF(0, 100),
                       QPointF(0, 50),
                       QPointF(0, 0),
                       QPointF(50, 0),
                       QPointF(100,0 ),
                       QPointF(150, 0),
                       QPointF(200, 0),
                       QPointF(250, 0),
                       QPointF(250,50 ),
                       QPointF(250, 100),
                       QPointF(250, 150),
                       QPointF(250, 200),


  };
}
void EnemyA::startMoving(double speed) {
    if (pathPoints.size() < 2) return;

    timer->start(20);
    targetSpeed = speed;
}





void EnemyA::move() {
    if (pathIndex < pathPoints.size() - 1) {
        QPointF currentPoint = pos();
        QPointF targetPoint = pathPoints[pathIndex + 1];


        QPointF direction = targetPoint - currentPoint;
        QLineF line(currentPoint, targetPoint);
        double distance = line.length();

        if (distance > 0) {
            direction /= distance;


            QPointF newPos = currentPoint + direction * targetSpeed * 0.02;
            setPos(newPos);


            if (QLineF(newPos, targetPoint).length() < targetSpeed * 0.02) {
                pathIndex++;
                qDebug() << "Moved to point:" << pathPoints[pathIndex];
            }
        }
    } else {

        timer->stop();
        qDebug() << "Movement finished!";
        scene()->removeItem(this);
        delete this;
    }
}

/*void EnemyA::startMoving() {
    int totalFrames = 1;
    int durationPerFrame = 7;


    QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup(this);


    if (scene() && !scene()->items().contains(this)) {
        scene()->addItem(this);
    }


    for (int i = 0; i < pathPoints.size() - 1; ++i) {
        QPointF startPoint = pathPoints[i];
        QPointF endPoint = pathPoints[i + 1];


        for (int j = 0; j < totalFrames; ++j) {

            qreal t = static_cast<qreal>(j) / totalFrames;
            QPointF intermediatePoint = startPoint + (endPoint - startPoint) * t;


            QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
            animation->setStartValue(startPoint);
            animation->setEndValue(intermediatePoint);
            animation->setDuration(durationPerFrame);
            animation->setEasingCurve(QEasingCurve::Linear);

            animationGroup->addAnimation(animation);
            startPoint = intermediatePoint;
        }
    }


    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

    timer->start(600);
}


void EnemyA::move() {

    if (pathIndex < pathPoints.size()) {
        setPos(pathPoints[pathIndex]);
        pathIndex++;
    } else {

        timer->stop();
        scene()->removeItem(this);
        delete this;
}
}
*/



EnemyB::EnemyB(QPointF start, QPointF end, QObject *parent)
    : QObject(parent), startPoint(start), endPoint(end), pathIndex(0) {


    setRect(2, 2, 20, 30);
    setBrush(QBrush(QColor(0, 200, 0)));

    setPos(startPoint);


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EnemyB::move);


    setupPath();
}


void EnemyB::setupPath() {

    pathPoints = {
        //QPointF(0, 200),
        QPointF(0,150),
        QPointF(0, 100),
        QPointF(0, 50),
        QPointF(0, 0),
        QPointF(50, 0),
        QPointF(100,0 ),
        QPointF(150, 0),
        QPointF(200, 0),
        QPointF(250, 0),
        QPointF(250,50 ),
        QPointF(250, 100),
        QPointF(250, 150),
        QPointF(250, 200),


    };
}



void EnemyB::startMoving(double speed) {
    if (pathPoints.size() < 2) return;

    timer->start(20); // This value controls the update rate, feel free to adjust
    targetSpeed = speed;
}





void EnemyB::move() {
    if (pathIndex < pathPoints.size() - 1) {
        QPointF currentPoint = pos();
        QPointF targetPoint = pathPoints[pathIndex + 1];

        // Calculate the direction vector and normalize it
        QPointF direction = targetPoint - currentPoint;
        QLineF line(currentPoint, targetPoint);
        double distance = line.length();

        if (distance > 0) {
            direction /= distance; // Normalize


            QPointF newPos = currentPoint + direction * targetSpeed * 0.02;
            setPos(newPos);


            if (QLineF(newPos, targetPoint).length() < targetSpeed * 0.02) {
                pathIndex++;
                qDebug() << "Moved to point:" << pathPoints[pathIndex];
            }
        }
    } else {

        timer->stop();
        qDebug() << "Movement finished!";
        scene()->removeItem(this);
        delete this;
    }
}




//تركن

/*void EnemyB::startMoving() {
    int totalFrames = 10000;
    int durationPerFrame = 1;


    QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup(this);


    if (scene() && !scene()->items().contains(this)) {
        scene()->addItem(this);
    }


    for (int i = 0; i < pathPoints.size() - 1; ++i) {
        QPointF startPoint = pathPoints[i];
        QPointF endPoint = pathPoints[i + 1];


        for (int j = 0; j < totalFrames; ++j) {

            qreal t = static_cast<qreal>(j) / totalFrames;
            QPointF intermediatePoint = startPoint + (endPoint - startPoint) * t;


            QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
            animation->setStartValue(startPoint);
            animation->setEndValue(intermediatePoint);
            animation->setDuration(durationPerFrame);
            animation->setEasingCurve(QEasingCurve::Linear);

            animationGroup->addAnimation(animation);
            startPoint = intermediatePoint;
        }
    }


    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

    timer->start(600);
}*/
/*void EnemyB::move() {

    if (pathIndex < pathPoints.size()) {
        setPos(pathPoints[pathIndex]);
        pathIndex++;
    } else {

        timer->stop();
        scene()->removeItem(this);
        delete this;
    }
}
*/
