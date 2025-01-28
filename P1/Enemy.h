

#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

class Agent;



class Enemy : public QObject {
    Q_OBJECT

public:
    Enemy(QPointF start, QPointF end,int H, QObject *parent = nullptr);

    virtual void setupAppearance() = 0;
    virtual void setPos(const QPointF &position) = 0;
    virtual QPointF pos() const = 0;
    virtual void removeFromScene() = 0;

    void startMoving(double speed);
    void decreaseHealth(int damage);
    int getHealth() const;
    QVector<QPointF> pathPoints;
    int pathIndex;
    double targetSpeed;
    int health;


signals:
    void destroyed();
    void reachedTarget();
protected:
    QTimer *timer;
    QPointF startPoint;
    QPointF endPoint;

    int maxHealth;

    QGraphicsRectItem *healthBarBackground;
    QGraphicsRectItem *healthBar;

    void setupPath();
    void updateHealthBar();

protected slots:
    void move();
};

class BOSSE : public Enemy, public QGraphicsPixmapItem {
    Q_OBJECT

public:
   BOSSE(QPointF start, QPointF end,int H, QObject *parent = nullptr);

    void setupAppearance() =0;
    void setPos(const QPointF &position) override { QGraphicsPixmapItem::setPos(position); }
    QPointF pos() const override { return QGraphicsPixmapItem::pos(); }
    void removeFromScene() override { scene()->removeItem(this); delete this; }
};


class FreezerBoss : public BOSSE {
    Q_OBJECT

public:
    FreezerBoss(QPointF start, QPointF end, QVector<Agent*>& agentsRef, int H, QObject *parent = nullptr);
    ~FreezerBoss();

    void setupAppearance() override;
    void startFreezing();
    void stopFreezing();
    void removeFromScene();

private:
    QTimer *freezeTimer;
    QVector<Agent *> frozenAgents;
    QVector<Agent*>& agents;

    const int freezeDuration = 5000;

private slots:
    void freezeRandomAgent();
};




class EraserBoss : public BOSSE {
    Q_OBJECT

public:
    EraserBoss(QPointF start, QPointF end, QVector<Agent*>& agentsRef,int H, QObject *parent = nullptr);

    ~EraserBoss();
    void setupAppearance() override;

private:
    QTimer *killTimer;

    QVector<Agent*>& agents;

    const int KillDuration = 7000;

private slots:
    void KillRandomAgent();

};






class Soldier : public Enemy, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Soldier(QPointF start, QPointF end,int H, QObject *parent = nullptr);

    void setupAppearance() =0;
    void setPos(const QPointF &position) override { QGraphicsEllipseItem::setPos(position); }
    QPointF pos() const override { return QGraphicsEllipseItem::pos(); }
    void removeFromScene() override { scene()->removeItem(this); delete this; }
};

class Runner : public Soldier {
    Q_OBJECT

public:
    Runner(QPointF start, QPointF end,int H, QObject *parent = nullptr);
    void setupAppearance() override;


};
class shielder : public Soldier {
    Q_OBJECT

public:
    shielder(QPointF start, QPointF end,int H, QObject *parent = nullptr);
    void setupAppearance() override;


};


#endif

