#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QVector>

#include "Agent.h"
#include "Enemy.h"
#include "WaveSpawner.h"

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
    enum class TileRole {
        Empty,
        Path,
        Spawn,
        Exit,
        Buildable,
    };

    QGraphicsScene *scene;

    QVector<QGraphicsRectItem *> boxCells;
    QVector<Agent *> agents;
    QVector<QGraphicsRectItem *> agentCells;

    QGraphicsRectItem *selectedBox = nullptr;
    Agent *selectedAgent = nullptr;

    bool grid[10][10];

    QVector<QVector<TileRole>> tileRoles;
    QVector<QPointF> pathPoints_;

    WaveSpawner *waveSpawner = nullptr;
    QVector<WaveSpawner::WaveDefinition> waves_;

    bool addTexture(QGraphicsRectItem *item, const QString &texturePath);
    QGraphicsRectItem *getCellAt(const QPointF &position);
    void addAgents();

    bool isCellOccupied(int x, int y);
    void occupyCell(int x, int y);
    void releaseCell(int x, int y);

    QVector<QPointF> buildPathFromGrid() const;
    bool isPathRole(TileRole role) const;
    QPointF cellToPathPoint(int row, int col) const;

    const int cellSize = 80;
    const int rows = 5;
    const int cols = 6;
};

#endif // GAMEMAP_H
