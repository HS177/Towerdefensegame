#ifndef AGENT_H
#define AGENT_H

#include <QGraphicsRectItem>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QTimer>
#include "bullet.h"
#include "Enemy.h"

class Agent : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Agent(const QColor &color, QObject *parent = nullptr);
    virtual ~Agent();

    QColor getColor() const;
    void setColor(const QColor &color);

    virtual void startShooting();
    virtual void shoot();

    virtual int getElixirN(){
        return elixerN;
    }
    int elixerN;


    int getLevel() const;
    void setLevel(int level);



protected:
    int level;
    QGraphicsTextItem* levelDisplay;

    QTimer *shootTimer;
    QColor color;
};


class EndStrikerAgent : public Agent {
    Q_OBJECT

public:
    EndStrikerAgent(const QColor &color, QObject *parent = nullptr);
    ~EndStrikerAgent();
    QPointF predictEnemyPosition(Enemy* enemy, double travelTime);

    void shoot() override;

    int getElixirN() override{
        return elixerN;
    }

private:
    int elixerN=2;
    Enemy* findClosestEnemy();
};


class FirstStrickerAgent : public Agent {
    Q_OBJECT

public:
    FirstStrickerAgent(const QColor &color, QObject *parent = nullptr);
    ~FirstStrickerAgent();

    void shoot() override;

    int getElixirN() override{
        return elixerN;
    }

private:
     int elixerN=2;
    Enemy* findClosestEnemy();
};



class RandomStrickerAgent : public Agent {
    Q_OBJECT

public:
    RandomStrickerAgent(const QColor &color, QObject *parent = nullptr);
    ~RandomStrickerAgent();

    void shoot() override;
    Enemy* currentTarget=nullptr;

    int getElixirN() override{
        return elixerN;
    }

private:
    int elixerN=4;
    Enemy* findClosestEnemy();
};

class maxHealthstricker : public Agent {
    Q_OBJECT

public:
    maxHealthstricker(const QColor &color, QObject *parent = nullptr);
    ~maxHealthstricker();

    void shoot() override;
    Enemy* currentTarget=nullptr;


    int getElixirN() override{
        return elixerN;
    }
private:
    int elixerN=3;
    Enemy* findClosestEnemy();
};
#endif
