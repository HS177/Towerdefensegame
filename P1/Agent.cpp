
#include "Agent.h"
#include <QRandomGenerator>

Agent::Agent(const QColor &color, QObject *parent)
    : QObject(parent), QGraphicsEllipseItem(), color(color) {
    setRect(0, 0, 20, 20);
    setBrush(QBrush(color));
    setPen(QPen(Qt::black));

    shootTimer = new QTimer(this);
    connect(shootTimer, &QTimer::timeout, this, &Agent::shoot);
}

Agent::~Agent() {
    if (shootTimer) {
        shootTimer->stop();
        delete shootTimer;
    }
}

QColor Agent::getColor() const {
    return color;
}

void Agent::setColor(const QColor &color) {
    this->color = color;
    setBrush(QBrush(color));
}

void Agent::startShooting() {
    shootTimer->start(1000);
}

void Agent::shoot() {

    qDebug() << "Base Agent class does not implement shooting.";
}


 EndStrikerAgent:: EndStrikerAgent(const QColor &color, QObject *parent)
    : Agent(color, parent) {}



 void EndStrikerAgent::shoot() {
     Enemy* closestEnemy = findClosestEnemy();
     if (!closestEnemy) {
         qDebug() << "No enemies to shoot!";
         return;
     }


     QPointF localCenter = boundingRect().center();
     QPointF startPos = mapToScene(localCenter);


     QPointF enemyPos = closestEnemy->pos();
     double enemySpeed = closestEnemy->targetSpeed;


     const double projectileSpeed = 300.0;
     QLineF lineToEnemy(startPos, enemyPos);
     double distanceToEnemy = lineToEnemy.length();


     double travelTime = distanceToEnemy / projectileSpeed;


     QPointF predictedPosition = predictEnemyPosition(closestEnemy, travelTime);


     Projectile* bullet = new Projectile(startPos, predictedPosition);


     if (scene()) {
         scene()->addItem(bullet);
     }

     qDebug() << "Fired at predicted enemy position:" << predictedPosition;
 }

 QPointF EndStrikerAgent::predictEnemyPosition(Enemy* enemy, double travelTime) {
     QPointF predictedPosition = enemy->pos();
     double remainingTime = travelTime;


     while (remainingTime > 0 && enemy->pathIndex < enemy->pathPoints.size() - 1) {

         QPointF currentPoint = enemy->pathPoints[enemy->pathIndex];
         QPointF nextPoint = enemy->pathPoints[enemy->pathIndex + 1];


         QLineF segment(currentPoint, nextPoint);
         double segmentDistance = segment.length();
         double timeToNextPoint = segmentDistance / enemy->targetSpeed;

         if (remainingTime < timeToNextPoint) {

             QPointF direction = (nextPoint - currentPoint) / segmentDistance;
             predictedPosition = currentPoint + direction * (enemy->targetSpeed * remainingTime);
             break;
         } else {

             remainingTime -= timeToNextPoint;
             enemy->pathIndex++;
         }
     }


     if (enemy->pathIndex >= enemy->pathPoints.size() - 1) {
         predictedPosition = enemy->pathPoints.last();
     }

     return predictedPosition;
 }


Enemy*  EndStrikerAgent::findClosestEnemy() {
    if (!scene()) return nullptr;

    QList<QGraphicsItem*> items = scene()->items();
    Enemy* lastEnemy = nullptr;

    for (QGraphicsItem* item : items) {

        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {

            if (!lastEnemy) {
                lastEnemy = enemy;
            } else {

                if (enemy->pathIndex > lastEnemy->pathIndex) {
                    lastEnemy = enemy;
                }
            }
        }
    }

    return lastEnemy;
}
EndStrikerAgent::~EndStrikerAgent(){
}



 FirstStrickerAgent::FirstStrickerAgent(const QColor &color, QObject *parent)
    : Agent(color, parent) {}

FirstStrickerAgent::~FirstStrickerAgent() {}

void FirstStrickerAgent::shoot() {
    Enemy* closestEnemy = findClosestEnemy();
    if (!closestEnemy) {
        qDebug() << "No enemies to shoot!";
        return;
    }


    QPointF localCenter = boundingRect().center();
    QPointF startPos = mapToScene(localCenter);

    QPointF enemyPos = closestEnemy->pos();


    double projectileSpeed = 300.0;


    QLineF lineToEnemy(startPos, enemyPos);
    double distance = lineToEnemy.length();


    double travelTime = distance / projectileSpeed;


    while (closestEnemy->pathIndex < closestEnemy->pathPoints.size() - 1) {
        QPointF currentPoint = closestEnemy->pathPoints[closestEnemy->pathIndex];
        QPointF nextPoint = closestEnemy->pathPoints[closestEnemy->pathIndex + 1];
        QLineF segment(currentPoint, nextPoint);

        double segmentDistance = segment.length();
        double timeToNextPoint = segmentDistance / closestEnemy->targetSpeed;

        if (travelTime < timeToNextPoint) {

            QPointF direction = (nextPoint - currentPoint) / segmentDistance;
            enemyPos = currentPoint + direction * (closestEnemy->targetSpeed * travelTime);
            break;
        } else {

            travelTime -= timeToNextPoint;
            closestEnemy->pathIndex++;
        }
    }


    if (closestEnemy->pathIndex >= closestEnemy->pathPoints.size() - 1) {
        enemyPos = closestEnemy->pathPoints.last();
    }


    Projectile* bullet = new Projectile(startPos, enemyPos);
    if (scene()) {
        scene()->addItem(bullet);
    }

    qDebug() << "Fired at predicted enemy position:" << enemyPos;
}

Enemy* FirstStrickerAgent::findClosestEnemy() {
    if (!scene()) return nullptr;

    QList<QGraphicsItem*> items = scene()->items();
    Enemy* firstEnemy = nullptr;

    for (QGraphicsItem* item : items) {

        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {

            if (!firstEnemy) {
                firstEnemy = enemy;
            } else {

                if (enemy->pathIndex < firstEnemy->pathIndex) {
                    firstEnemy = enemy;
                }
            }
        }
    }

    return firstEnemy;
}




RandomStrickerAgent::RandomStrickerAgent(const QColor &color, QObject *parent)
    : Agent(color, parent) {}

RandomStrickerAgent::~RandomStrickerAgent() {}

void RandomStrickerAgent::shoot() {
    Enemy* closestEnemy = findClosestEnemy();
    if (!closestEnemy) {
        qDebug() << "No enemies to shoot!";
        return;
    }


    QPointF localCenter = boundingRect().center();
    QPointF startPos = mapToScene(localCenter);

    QPointF enemyPos = closestEnemy->pos();


    double projectileSpeed = 300.0;


    QLineF lineToEnemy(startPos, enemyPos);
    double distance = lineToEnemy.length();


    double travelTime = distance / projectileSpeed;


    while (closestEnemy->pathIndex < closestEnemy->pathPoints.size() - 1) {
        QPointF currentPoint = closestEnemy->pathPoints[closestEnemy->pathIndex];
        QPointF nextPoint = closestEnemy->pathPoints[closestEnemy->pathIndex + 1];
        QLineF segment(currentPoint, nextPoint);

        double segmentDistance = segment.length();
        double timeToNextPoint = segmentDistance / closestEnemy->targetSpeed;

        if (travelTime < timeToNextPoint) {

            QPointF direction = (nextPoint - currentPoint) / segmentDistance;
            enemyPos = currentPoint + direction * (closestEnemy->targetSpeed * travelTime);
            break;
        } else {

            travelTime -= timeToNextPoint;
            closestEnemy->pathIndex++;
        }
    }


    if (closestEnemy->pathIndex >= closestEnemy->pathPoints.size() - 1) {
        enemyPos = closestEnemy->pathPoints.last();
    }


    Projectile* bullet = new Projectile(startPos, enemyPos);
    if (scene()) {
        scene()->addItem(bullet);
    }

    qDebug() << "Fired at predicted enemy position:" << enemyPos;
}
Enemy* RandomStrickerAgent::findClosestEnemy() {
    if (!scene()) return nullptr;

    if (currentTarget) {
        QList<QGraphicsItem*> items = scene()->items();
        if (std::find_if(items.begin(), items.end(), [this](QGraphicsItem* item) {
                return dynamic_cast<Enemy*>(item) == currentTarget;
            }) != items.end()) {
            return currentTarget;
        }
    }


    QList<QGraphicsItem*> items = scene()->items();


    QList<Enemy*> enemies;


    for (QGraphicsItem* item : items) {
        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {
            enemies.append(enemy);
        }
    }


    if (enemies.isEmpty()) {
        currentTarget = nullptr;
        return nullptr;
    }


    int randomIndex = QRandomGenerator::global()->bounded(enemies.size());
    currentTarget = enemies[randomIndex];
    return currentTarget;
}

