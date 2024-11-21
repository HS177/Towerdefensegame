
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

class GameMap : public QGraphicsView {
    Q_OBJECT

public:
    GameMap(QWidget *parent = nullptr);

    void spawnEnemy();
    void initialize();
    void createMap();


protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QGraphicsScene *scene;
    QTimer *spawnTimer;
    QVector<QGraphicsRectItem*> boxCells;
    QVector<Agent*> agents;

    QVector<QGraphicsRectItem*> agentCells;
    QGraphicsRectItem *selectedBox = nullptr;
    Agent *selectedAgent = nullptr;

    bool addTexture(QGraphicsRectItem *item, const QString &texturePath);

    const int cellSize = 80;
    const int rows = 5;
    const int cols = 6;

    void addAgents();
    QGraphicsRectItem *getCellAt(const QPointF &position);
};

#endif
