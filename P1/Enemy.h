#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>

// Base class for all enemies
class Enemy : public QObject {
    Q_OBJECT

public:
    Enemy(QPointF start, QPointF end, QObject *parent = nullptr);

    virtual void setupAppearance() = 0;
    virtual void setPos(const QPointF &position) = 0;
    virtual QPointF pos() const = 0;
    virtual void removeFromScene() = 0;
    void startMoving(double speed);
    int pathIndex;
    QVector<QPointF> pathPoints;

    double targetSpeed;

protected slots:
    void move();

protected:
    QTimer *timer;
    QPointF startPoint;
    QPointF endPoint;


    void setupPath();
};


class EnemyA : public Enemy, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    EnemyA(QPointF start, QPointF end, QObject *parent = nullptr);

    void setupAppearance() override;
    void setPos(const QPointF &position) override { QGraphicsPixmapItem::setPos(position); }
    QPointF pos() const override { return QGraphicsPixmapItem::pos(); }
    void removeFromScene() override { scene()->removeItem(this); delete this; }
};


class EnemyB : public Enemy, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    EnemyB(QPointF start, QPointF end, QObject *parent = nullptr);

    void setupAppearance() override;
    void setPos(const QPointF &position) override { QGraphicsEllipseItem::setPos(position); }
    QPointF pos() const override { return QGraphicsEllipseItem::pos(); }
    void removeFromScene() override { scene()->removeItem(this); delete this; }
};

#endif
