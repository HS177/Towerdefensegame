#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QBrush>
#include <QPen>
#include <QVector>
#include "Agent.h"
#include "Enemy.h"

class GameMap :public QGraphicsView {
    Q_OBJECT

public:
    GameMap(QWidget *parent = nullptr);

    void spawnEnemyWave();
    void initialize();
    void createMap();
    QList<QGraphicsTextItem *> levelTexts;

protected:
    void mousePressEvent(QMouseEvent *event) ;
     void mousePressEventForLevel(QMouseEvent *event) ;
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    const QString colorNames[6] = {"EndStriker", "MaxHealthStriker", "FirstStriker", "RandomStriker", "Bomb", "Trap"};

private:
    QGraphicsScene *scene;
    QTimer *spawnTimer;
    QVector<QGraphicsRectItem*> boxCells;
    QVector<Agent*> agents;
    QVector<Agent*> agentsA;

    QVector<QGraphicsRectItem*> agentCells;
    QVector<QGraphicsRectItem*> levelcells;
    QVector<QGraphicsRectItem*> BlockerCells;

    QVector<Enemy *> enemies;

    QGraphicsRectItem *selectedBox = nullptr;
    Agent *selectedstriker = nullptr;
    Agent *selectedblocker = nullptr;

    bool grid[10][10];

    bool addTexture(QGraphicsRectItem *item, const QString &texturePath);
    QGraphicsRectItem *getCellAt(const QPointF &position);
    void addAgents();
    void addlevels();

    bool isCellOccupied(int x, int y);
    void occupyCell(int x, int y);
    void releaseCell(int x, int y);

    QGraphicsTextItem * elixirCounterText;
    int elixirCounter=0;
    QTimer *elixirTimer;

    void startElixirCounter();


    BOSSE *enemya;
    Soldier *enemyb;

    const int cellSize = 80;
    const int rows = 5;
    const int cols = 6;

    int waveNumber=0;

};

#endif
