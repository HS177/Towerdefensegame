
#include "Enemy.h"
#include "Agent.h"
#include <QBitmap>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QBrush>
#include <QDebug>
#include <QLineF>
#include <qrandom.h>
#include <random>


Enemy::Enemy(QPointF start, QPointF end,int H, QObject *parent)
    : QObject(parent), startPoint(start), endPoint(end), pathIndex(0), health(H), maxHealth(H) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemy::move);
    setupPath();
}





void Enemy::updateHealthBar() {
    if (health < 0) health = 0;


    double healthPercentage = static_cast<double>(health)/maxHealth ;
    healthBar->setRect(-15, -10, 30 * healthPercentage, 5);
}


void Enemy::decreaseHealth(int damage) {
    health -= damage;
    if (health <= 0) {
        emit destroyed();
        qDebug() << "Enemy destroyed!";
        removeFromScene();
    } else {
        updateHealthBar();
    }
}


void Enemy::startMoving(double speed) {
    timer->start(20);
    targetSpeed = speed;
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


void Enemy::move() {
    if (pathIndex < pathPoints.size() - 1) {
        QPointF currentPoint = pos();
        QPointF targetPoint = pathPoints[pathIndex + 1];


        QPointF direction = targetPoint - currentPoint;
        QLineF line(currentPoint, targetPoint);
        double distance = line.length();

        if (distance > 0) {
            direction /= distance;


            double adjustedSpeed = qMin(100.0, targetSpeed * 0.02);
            QPointF newPos = currentPoint + direction * adjustedSpeed;

            setPos(newPos);


            if (QLineF(newPos, targetPoint).length() < adjustedSpeed) {
                qDebug() << "Reached Target Point. Moving to Next Index:" << pathIndex + 1;
                pathIndex++;
            }
        }
    } else {

        emit reachedTarget();
        timer->stop();
        qDebug() << "Enemy reached the destination!";
        removeFromScene();
    }
}




void Enemy::removeFromScene() {
    auto graphicsItem = dynamic_cast<QGraphicsItem *>(this);
    if (graphicsItem && graphicsItem->scene()) {
        graphicsItem->scene()->removeItem(graphicsItem);
        delete this;
    }
}





BOSSE::BOSSE(QPointF start, QPointF end, int H, QObject *parent)
    : Enemy(start, end, H, parent) {

}


FreezerBoss::FreezerBoss(QPointF start, QPointF end, QVector<Agent*>& agentsRef, int H, QObject *parent)
    : BOSSE(start, end, H, parent),agents(agentsRef) {
    freezeTimer = new QTimer(this);
    setupAppearance();
    setPos(start);
    startFreezing();
}


void FreezerBoss::setupAppearance() {
    QPixmap enemyPixmap(":/new/prefix2/enemy1.png");
    setScale(0.2);
    QBitmap mask = enemyPixmap.createMaskFromColor(QColor(255, 255, 255));
    enemyPixmap.setMask(mask);
    setPixmap(enemyPixmap);



    healthBar = new QGraphicsRectItem(-15,-10,30,5,this);

    healthBar->setScale(5);
    healthBar->setBrush(Qt::green);
}
FreezerBoss::~FreezerBoss() {
    stopFreezing();
    delete freezeTimer;
}



void FreezerBoss::startFreezing() {
    connect(freezeTimer, &QTimer::timeout, this, &FreezerBoss::freezeRandomAgent);
    freezeTimer->start(5000);
}

void FreezerBoss::freezeRandomAgent() {
    if (agents.isEmpty()) {
        qDebug() << "No agents to freeze!";
        return;
    }


    int randomIndex = rand() % agents.size();
    Agent *targetAgent = agents.at(randomIndex);


    if (frozenAgents.contains(targetAgent)) {
        return;
    }

    qDebug() << "Freezing agent with color:" << targetAgent->getColor();


    targetAgent->freezeAgent();
    frozenAgents.append(targetAgent);



}


void FreezerBoss::stopFreezing() {
    freezeTimer->stop();


    for (Agent *agent : frozenAgents) {
        agent->unfreezeAgent();
    }
    frozenAgents.clear();
}


void FreezerBoss::removeFromScene() {
    stopFreezing();
    if (scene()) {
        scene()->removeItem(this);
    }
    delete this;
}



EraserBoss::EraserBoss(QPointF start, QPointF end, QVector<Agent*>& agentsRef,int H,  QObject *parent)
    : BOSSE(start, end, H, parent), agents(agentsRef) {
    killTimer = new QTimer(this);
    setupAppearance();
    setPos(start);


    connect(killTimer, &QTimer::timeout, this, &EraserBoss::KillRandomAgent);
    killTimer->start(7000);
}



void EraserBoss::KillRandomAgent() {
    if (agents.isEmpty()) {
        qDebug() << "No agents to delete!";
        return;
    }


    int randomIndex = QRandomGenerator::global()->bounded(agents.size());
    Agent* targetAgent = agents.at(randomIndex);


    if (targetAgent->scene()) {
        targetAgent->scene()->removeItem(targetAgent);
    }
    agents.remove(randomIndex);



    qDebug() << "Agent deleted by EraserBoss!";
}

EraserBoss::~EraserBoss() {
    killTimer->stop();
    killTimer->disconnect();
    delete killTimer;
}

void EraserBoss::setupAppearance() {
    QPixmap enemyPixmap(":/new/prefix2/PICTURES/images.jpeg");
    setScale(0.24);
    QBitmap mask = enemyPixmap.createMaskFromColor(QColor(255, 255, 255));
    enemyPixmap.setMask(mask);
    setPixmap(enemyPixmap);



    healthBar = new QGraphicsRectItem(-15,-10,30,5,this);

    healthBar->setScale(5);
    healthBar->setBrush(Qt::green);
}



Soldier::Soldier(QPointF start, QPointF end,int H, QObject *parent)
    : Enemy(start, end,H, parent) {
     setPos(start);

}
Runner::Runner(QPointF start, QPointF end,int H, QObject *parent)
    : Soldier(start, end,H, parent) {
    setupAppearance();
}

void Runner::setupAppearance() {
    setRect(3, 3, 30, 40);
    setBrush(QBrush(QColor(0, 200, 0)));


    healthBar = new QGraphicsRectItem(-15, -10, 30, 5, this);
    healthBar->setBrush(Qt::green);
}


shielder::shielder(QPointF start, QPointF end,int H, QObject *parent)
    : Soldier(start, end,H, parent) {
    setupAppearance();
}

void shielder::setupAppearance() {
    setRect(3, 3, 30, 40);
    setBrush(QBrush(QColor(0, 200, 200)));


    healthBar = new QGraphicsRectItem(-15, -10, 30, 5, this);
    healthBar->setBrush(Qt::green);
}
