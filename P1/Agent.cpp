
#include "Agent.h"
#include <QRandomGenerator>
Agent::Agent(const QColor & color,QObject * parent)
    :QObject(parent), QGraphicsEllipseItem(), color(color), level(1){
    setRect(0, 0, 20, 20);
    setBrush(QBrush(color));
    setPen(QPen(Qt::black));

    levelDisplay = new QGraphicsTextItem(QString("Lv %1").arg(level), this);
    levelDisplay->setDefaultTextColor(Qt::black);
    QFont font;
    font.setPointSize(8);
    font.setBold(true);
    levelDisplay->setFont(font);


    levelDisplay->setPos(pos().x() + rect().width() / 2 - levelDisplay->boundingRect().width() / 2,
                         pos().y() - levelDisplay->boundingRect().height() - 5);
    levelDisplay->setVisible(true);

}
void Agent::setLevel(int newLevel) {
    level = newLevel;
    if (levelDisplay) {
        levelDisplay->setPlainText(QString("Lv %1").arg(level));
    }
}

int Agent::getLevel() const {
    return level;
}
Agent::~Agent() {
}
QColor Agent::getColor() const {
    return color;
}

void Agent::setColor(const QColor &color) {
    this->color = color;
    setBrush(QBrush(color));
}
void Agent::freezeAgent() {
    isFrozen = true;
    stopShooting();
}

void Agent::unfreezeAgent() {
    isFrozen = false;
    startShooting();
}


striker::striker(const QColor &color, QObject *parent)
    : Agent(color,parent), level(1) {
    setRect(0, 0, 20, 20);
    setBrush(QBrush(color));
    setPen(QPen(Qt::black));

    shootTimer = new QTimer(this);
    connect(shootTimer, &QTimer::timeout, this, &striker::shoot);





}


striker::~striker() {
    if (shootTimer) {
        shootTimer->stop();
        delete shootTimer;
    }
}

QColor striker::getColor() const {
    return color;
}

void striker::setColor(const QColor &color) {
    this->color = color;
    setBrush(QBrush(color));
}

void striker::startShooting() {
    if (!shootTimer) {
        shootTimer = new QTimer(this);
        connect(shootTimer, &QTimer::timeout, this, &striker::shoot);
    }
    shootTimer->start(1000 / shootingSpeed);
    shooting = true;
}

void striker::stopShooting() {
    shooting=false;
    shootTimer->stop();
}

void striker::shoot() {

    qDebug() << "Base Agent class does not implement shooting.";
}


 EndStrikerAgent:: EndStrikerAgent(const QColor &color, QObject *parent)
    : striker(color, parent) {
     setLevel(1);
 }



 void EndStrikerAgent::shoot() {
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


     QPointF predictedPos = enemyPos;

     while (closestEnemy->pathIndex < closestEnemy->pathPoints.size() - 1) {
         QPointF currentPoint = closestEnemy->pathPoints[closestEnemy->pathIndex];
         QPointF nextPoint = closestEnemy->pathPoints[closestEnemy->pathIndex + 1];
         QLineF segment(currentPoint, nextPoint);

         double segmentDistance = segment.length();
         double timeToNextPoint = segmentDistance / closestEnemy->targetSpeed;

         if (travelTime < timeToNextPoint) {

             QPointF direction = (nextPoint - currentPoint) / segmentDistance;
             predictedPos = currentPoint + direction * (closestEnemy->targetSpeed * travelTime);
             break;
         } else {
             travelTime -= timeToNextPoint;
             closestEnemy->pathIndex++;
         }
     }

     if (closestEnemy->pathIndex >= closestEnemy->pathPoints.size() - 1) {
         predictedPos = closestEnemy->pathPoints.last();
     }


     Projectile* bullet = new Projectile(startPos, predictedPos);
     if (scene()) {
         scene()->addItem(bullet);
     }

     qDebug() << "Fired at predicted enemy position:" << predictedPos;
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
    : striker(color, parent) {
 }

FirstStrickerAgent::~FirstStrickerAgent() {
    setLevel(1);
}

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
    : striker(color, parent) {
    setLevel(1);
}

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

maxHealthstricker::maxHealthstricker(const QColor &color, QObject *parent)
    : striker(color, parent) {
    setLevel(1);
}

maxHealthstricker::~maxHealthstricker() {

}

void maxHealthstricker::shoot() {
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

Enemy* maxHealthstricker::findClosestEnemy() {
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


    Enemy* mostHealthEnemy = nullptr;
    int maxHealth = -1;

    for (Enemy* enemy : enemies) {
        if (enemy->health > maxHealth) {
            maxHealth = enemy->health;
            mostHealthEnemy = enemy;
        }
    }


    currentTarget = mostHealthEnemy;
    return currentTarget;
}
