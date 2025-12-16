#include "Enemy.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QLineF>

EnemyBase::EnemyBase(const QVector<QPointF> &pathPoints, QObject *parent)
    : QObject(parent), pathPoints_(pathPoints) {
    connect(&timer_, &QTimer::timeout, this, &EnemyBase::move);

    if (!pathPoints_.isEmpty()) {
        setPos(pathPoints_.first());
    }
}

void EnemyBase::startMoving(double speed) {
    if (pathPoints_.size() < 2) {
        return;
    }

    targetSpeed_ = speed;
    timer_.start(20);
}

void EnemyBase::move() {
    if (pathIndex_ < pathPoints_.size() - 1) {
        const QPointF currentPoint = pos();
        const QPointF targetPoint = pathPoints_[pathIndex_ + 1];

        QPointF direction = targetPoint - currentPoint;
        const QLineF line(currentPoint, targetPoint);
        const double distance = line.length();

        if (distance > 0.0) {
            direction /= distance;

            const QPointF newPos = currentPoint + direction * targetSpeed_ * 0.02;
            setPos(newPos);

            if (QLineF(newPos, targetPoint).length() < targetSpeed_ * 0.02) {
                ++pathIndex_;
            }
        }

        return;
    }

    timer_.stop();

    if (scene()) {
        scene()->removeItem(this);
    }

    delete this;
}

EnemyA::EnemyA(const QVector<QPointF> &pathPoints, QObject *parent)
    : EnemyBase(pathPoints, parent) {
    setRect(0, 0, 40, 40);
    setBrush(QBrush(QColor(100, 0, 0)));
}

EnemyB::EnemyB(const QVector<QPointF> &pathPoints, QObject *parent)
    : EnemyBase(pathPoints, parent) {
    setRect(3, 3, 30, 40);
    setBrush(QBrush(QColor(0, 200, 0)));
}
