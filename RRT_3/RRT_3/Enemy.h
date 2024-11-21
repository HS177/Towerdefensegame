

#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>




class EnemyA : public QObject, public QGraphicsEllipseItem {
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

/*class Agent {
public:
    // Constructor to initialize the agent with a color
    Agent(QColor color) : color(color) {}

    // Getter for the color of the agent
    QColor getColor() const { return color; }

private:
    QColor color;  // Color of the agent
};
*/
#endif
