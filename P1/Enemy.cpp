#include "Enemy.h"
#include <QGraphicsScene>
#include <QBitmap>
#include <QBrush>
#include <QLineF>

Enemy::Enemy(QPointF start, QPointF end, QObject *parent)
    : QObject(parent), startPoint(start), endPoint(end), pathIndex(0) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemy::move);
    setupPath();
}

void Enemy::setupPath() {
    pathPoints = {
        QPointF(20, 300),
        QPointF(20, 220),
        QPointF(20, 140),
        QPointF(20, 20),
        QPointF(90, 20),
        QPointF(170, 20),
        QPointF(250, 20),
        QPointF(330, 20),
        QPointF(400, 20),
        QPointF(420, 20),
        QPointF(420, 140),
        QPointF(420, 220),
        QPointF(420, 300),
        QPointF(420, 350)
    };
}

void Enemy::startMoving(double speed) {
    timer->start(20);
    targetSpeed = speed;
}

void Enemy::move() {
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
        removeFromScene();
    }
}

EnemyA::EnemyA(QPointF start, QPointF end, QObject *parent)
    : Enemy(start, end, parent) {
    setupAppearance();
    setPos(start);
}

void EnemyA::setupAppearance() {
    QPixmap enemyPixmap(":/new/prefix2/enemy1.png");
    setScale(0.2);
    QBitmap mask = enemyPixmap.createMaskFromColor(QColor(255, 255, 255));
    enemyPixmap.setMask(mask);
    setPixmap(enemyPixmap);
}

EnemyB::EnemyB(QPointF start, QPointF end, QObject *parent)
    : Enemy(start, end, parent) {
    setupAppearance();
    setPos(start);
}

void EnemyB::setupAppearance() {
    setRect(3, 3, 30, 40);
    setBrush(QBrush(QColor(0, 200, 0)));
}
