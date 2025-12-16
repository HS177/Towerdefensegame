#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QTimer>
#include <QVector>

class EnemyBase : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    explicit EnemyBase(const QVector<QPointF> &pathPoints, QObject *parent = nullptr);

    void startMoving(double speed);

private slots:
    void move();

private:
    QTimer timer_;
    QVector<QPointF> pathPoints_;
    int pathIndex_ = 0;
    double targetSpeed_ = 0.0;
};

class EnemyA : public EnemyBase {
public:
    explicit EnemyA(const QVector<QPointF> &pathPoints, QObject *parent = nullptr);
};

class EnemyB : public EnemyBase {
public:
    explicit EnemyB(const QVector<QPointF> &pathPoints, QObject *parent = nullptr);
};

#endif // ENEMY_H
