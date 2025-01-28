
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsEllipseItem>
#include <QColor>
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QLineF>
#include <QDebug>
#include "Enemy.h"

class Projectile : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    explicit Projectile(const QPointF &startPos, const QPointF &targetPos, QObject *parent = nullptr)
        : QObject(parent), QGraphicsEllipseItem(), targetPos(targetPos) {
        setRect(0, 0, 20, 20);
        setBrush(Qt::red);
        setPen(Qt::NoPen);

        setPos(startPos);

        QTimer *moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, this, &Projectile::move);
        moveTimer->start(7.8);
    }


private slots:
    void move() {


        QList<QGraphicsItem *> collidingItemsList = collidingItems();

        for (auto item : collidingItemsList) {

            Enemy *enemy = dynamic_cast<Enemy *>(item);
            if (enemy) {

                enemy->decreaseHealth(20);


                if (scene()) {
                    scene()->removeItem(this);
                }


                delete this;
                return;
            }
        }



        QPointF currentPos = pos();
        QLineF line(currentPos, targetPos);
        qreal distance = line.length();

        if (distance < 5.0) {
            if (scene()) {
                scene()->removeItem(this);
            }
            delete this;
            return;
        }

        QPointF direction = line.p2() - line.p1();
        direction /= distance;
        QPointF newPos = currentPos + direction * 5;
        setPos(newPos);
    }
private:
    QPointF targetPos;
};
#endif
