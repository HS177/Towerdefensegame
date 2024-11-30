

#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsPixmapItem>




class EnemyA : public QObject,public QGraphicsPixmapItem {
    Q_OBJECT

public:
    EnemyA (QPointF start, QPointF end, QObject *parent = nullptr);


    void startMoving(double);

private slots:
    void move();

private:
    QTimer *timer;
    QPointF startPoint;
    QPointF endPoint;
    int pathIndex;
    QVector<QPointF> pathPoints;
    double targetSpeed;

    void setupPath();
};


class EnemyB : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    EnemyB (QPointF start, QPointF end, QObject *parent = nullptr);


    void startMoving(double);

private slots:
    void move();

private:
    QTimer *timer;
    QPointF startPoint;
    QPointF endPoint;
    int pathIndex;
    QVector<QPointF> pathPoints;
    double targetSpeed;

    void setupPath();
};


#endif
