#ifndef AGENT_H
#define AGENT_H

#include <QGraphicsRectItem>
#include <QColor>

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QTimer>
#include "bullet.h"

class Enemy;
class Agent :public QObject ,public QGraphicsEllipseItem{
    Q_OBJECT
public:
    Agent(const QColor &color, QObject *parent = nullptr);
    virtual ~Agent();

    QColor getColor() const;
    void setColor(const QColor &color);
    int getLevel() const;
    void setLevel(int level);
    virtual int getElixirN(){
        return elixerN;
    }


    int elixerN;

    virtual void startShooting()=0;
    virtual void startWatchingForEnemies()=0;

    virtual void stopShooting() = 0;
     bool isShooting(){
        return shooting ;
    }



    void freezeAgent();
    void unfreezeAgent();

    bool isFrozen;
    bool shooting;
    int ElexerNL=level*2;
    const int Maxlevel=5;


protected:

    int level=1;


    QGraphicsTextItem* levelDisplay;
    QColor color;

};

class striker : public Agent {
    Q_OBJECT

public:
   striker(const QColor &color, QObject *parent = nullptr);
     ~striker();

    QColor getColor() const;
    void setColor(const QColor &color);

    void explode(){

    }

     void startShooting();
    void stopShooting();
    virtual void shoot();

     void startWatchingForEnemies(){}
    double getShootingSpeed(){
        return shootingSpeed;
    }
    void setShootingSpeed(double S){
        shootingSpeed=S;
    }





int ElexerNL=level*2;


protected:
     double shootingSpeed=1;
     bool shooting;
    int level=1;


    QGraphicsTextItem* levelDisplay;
    QTimer *shootTimer;
};


class EndStrikerAgent : public striker {
    Q_OBJECT

public:
    EndStrikerAgent(const QColor &color, QObject *parent = nullptr);
    ~EndStrikerAgent();
    QPointF predictEnemyPosition(Enemy* enemy, double travelTime);

    void shoot() override;

    int getElixirN() override{
        return elixerN;
    }

     void startWatchingForEnemies(){}
     int ElexerNL=level*2;
private:
    double shootingSpeed=1;
    int elixerN=2;
     int level=1;

    Enemy* findClosestEnemy();
};


class FirstStrickerAgent : public striker {
    Q_OBJECT

public:
    FirstStrickerAgent(const QColor &color, QObject *parent = nullptr);
    ~FirstStrickerAgent();

    void shoot() override;

    int getElixirN() override{
        return elixerN;
    }
     void startWatchingForEnemies(){}
      int ElexerNL=level*2;

private:
      double shootingSpeed=1;
     int elixerN=2;
       int level=1;
    Enemy* findClosestEnemy();
};



class RandomStrickerAgent : public striker {
    Q_OBJECT

public:
    RandomStrickerAgent(const QColor &color, QObject *parent = nullptr);
    ~RandomStrickerAgent();

    void shoot() override;
    Enemy* currentTarget=nullptr;

    int getElixirN() override{
        return elixerN;
    }
 void startWatchingForEnemies(){}
     int ElexerNL=level*2;
private:
    double shootingSpeed=1;
    int elixerN=4;
     int level=1;
    Enemy* findClosestEnemy();
};

class maxHealthstricker : public striker {
    Q_OBJECT

public:
    maxHealthstricker(const QColor &color, QObject *parent = nullptr);
    ~maxHealthstricker();

    void shoot() override;
    Enemy* currentTarget=nullptr;


    int getElixirN() override{
        return elixerN;
    }
     void startWatchingForEnemies(){}
     int ElexerNL=level*2;
private:
      double shootingSpeed=1;
    int elixerN=3;
       int level=1;
    Enemy* findClosestEnemy();
};



class Blocker : public Agent {
    Q_OBJECT

public:
    Blocker(const QColor &color, QObject *parent = nullptr) : Agent(color, parent) {
        setRect(0, 0, 10, 0);
        setBrush(QBrush(color));
        setPen(QPen(Qt::black));
    }

     ~Blocker() {}





     void startWatchingForEnemies(){}
      int ElexerNL=level*2;




protected:

    int level=1;
    int enemyN;
    std::vector<Enemy *> enemiesInRange;

    Enemy *findClosestEnemy() {
        if (enemiesInRange.empty()) return nullptr;
        return *std::min_element(enemiesInRange.begin(), enemiesInRange.end(), [](Enemy *a, Enemy *b) {
            return std::hypot(a->pos().x(), a->pos().y()) < std::hypot(b->pos().x(), b->pos().y());
        });
    }
};

class Bomb : public Blocker {
public:
    Bomb(const QColor &color, QObject *parent = nullptr) : Blocker(color, parent), level(1) {}

     int ElexerNL=level*2;
    bool isSameCell(Enemy *enemy) {
        int enemyX = enemy->pos().x() / cellSize;
        int enemyY = enemy->pos().y() / cellSize;
        int bombX = this->pos().x() / cellSize;
        int bombY = this->pos().y() / cellSize;
        return enemyX == bombX && enemyY == bombY;
    }

    void explode(Enemy *enemy) {

        QList<Enemy *> enemiesToDamage = getEnemiesInRadius(1);
        for (Enemy *nearbyEnemy : enemiesToDamage) {
            nearbyEnemy->decreaseHealth(100);
            qDebug() << "Bomb exploded! Damaged enemy at cell ("
                     << nearbyEnemy->pos().x() / cellSize << ","
                     << nearbyEnemy->pos().y() / cellSize << ")";
        }
    }

    void deleteBomb() {
        scene()->removeItem(this);
        delete this;
    }
    void upgrade() { level++; }
    int getElixirN() override{
        return elixerN;
    }

    void startShooting() override {

        qDebug() << "Trap doesn't shoot but startShooting() must be implemented.";
    }
    void stopShooting() override{
    }
    void startWatchingForEnemies() {
        QTimer *checkTimer = new QTimer(this);
        connect(checkTimer, &QTimer::timeout, this, [this]() {

            for (Enemy *enemy : enemiesInRange) {
                if (isSameCell(enemy)) {
                    explode(enemy);
                    deleteBomb();
                    break;
                }
            }
        });
        checkTimer->start(100);
    }
    QList<Enemy *> getEnemiesInRadius(int radius) {
        QList<Enemy *> nearbyEnemies;
        int bombX = rect().x() / cellSize;
        int bombY = rect().y() / cellSize;

        for (Enemy *enemy : enemiesInRange) {
            int enemyX = enemy->pos().x() / cellSize;
            int enemyY = enemy->pos().y() / cellSize;
            if (abs(enemyX - bombX) <= radius && abs(enemyY - bombY) <= radius) {
                nearbyEnemies.append(enemy);
            }
        }
        return nearbyEnemies;
    }
    ~Bomb(){}



private:
    const int cellSize = 80;
    int elixerN=2;
    int level=1;
};

class Trap : public Blocker {
public:
    Trap(const QColor &color, QObject *parent = nullptr) : Blocker(color, parent), level(1) {}

     int ElexerNL=level*2;

    int getElixirN() override{
        return elixerN;
    }
    void upgrade() { level++; }
    void startShooting() override {

        qDebug() << "Trap doesn't shoot but startShooting() must be implemented.";
    }
    void stopShooting() override{
    }



        void startWatchingForEnemies() {
        QTimer *checkTimer = new QTimer(this);
        connect(checkTimer, &QTimer::timeout, this, [this]() {

            for (Enemy *enemy : enemiesInRange) {
                if (isSameCell(enemy)) {
                    slowEnemy(enemy);
                    deleteTrap();
                    break;
                }
            }
        });
        checkTimer->start(100);
    }

        bool isSameCell(Enemy *enemy) {
            int enemyX = enemy->pos().x() / cellSize;
            int enemyY = enemy->pos().y() / cellSize;
            int trapX = this->rect().x() / cellSize;
            int trapY = this->rect().y() / cellSize;
            return enemyX == trapX && enemyY == trapY;
        }

        void slowEnemy(Enemy *enemy) {
            enemy->startMoving(enemy->targetSpeed / 2);
            qDebug() << "Trap activated! Slowed enemy at cell (" << pos().x() / cellSize
                     << "," << pos().y() / cellSize << ")";
        }

        void deleteTrap() {
            scene()->removeItem(this);
            delete this;
        }
        ~Trap(){}
private:
        const int cellSize = 80;
    int elixerN=2;
    int level=1;
};






#endif
