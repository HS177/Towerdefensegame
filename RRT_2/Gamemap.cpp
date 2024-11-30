#include "Gamemap.h"
#include <QRandomGenerator>
#include <QTimer>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsScene>
#include <QtGui>
#include <algorithm>
#include <vector>
#include <random>
#include "Agent.h"
#include "Enemy.h"

GameMap::GameMap(QWidget *parent) : QGraphicsView(parent), scene(new QGraphicsScene(this)) {

    this->setScene(scene);


    scene->setBackgroundBrush(QBrush(QPixmap(":/new/prefix2/bricks.png")));


    createMap();

    addAgents();
    resize(cols * cellSize + 600, rows * cellSize + 200);

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            grid[i][j] = false;
        }
    }
}

void GameMap::createMap() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QGraphicsRectItem *cell = scene->addRect(j * cellSize, i * cellSize, cellSize, cellSize);
            cell->setPen(QPen(Qt::black));

            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                if (!addTexture(cell, ":/textures/path_texture.png")) {
                    cell->setBrush(QBrush(QColor(200, 200, 200,180)));
                }
            }

            if (i >= 1 && i <= 4 && j >= 1 && j <= 4) {
                if (!addTexture(cell, ":/textures/agent_texture.png")) {
                    cell->setBrush(QBrush(QColor(200, 200, 200)));
                }
                boxCells.append(cell);
            }

            if (i == rows - 1 && j == 0) {
                cell->setBrush(QBrush(QColor(200, 255, 200, 180)));
                cell->setPen(QPen(QColor(0, 200, 0), 2));
            } else if (i == rows - 1 && j == cols - 1) {
                cell->setBrush(QBrush(QColor(255, 200, 200, 180)));
                cell->setPen(QPen(QColor(200, 0, 0), 2));
            }
        }
    }
    for (int i = rows + 1; i < rows + 2; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            QGraphicsRectItem *cell = scene->addRect(j * cellSize, i * cellSize, cellSize, cellSize);
            cell->setPen(QPen(Qt::black));
            cell->setBrush(QBrush(QColor(150, 150, 150)));
            agentCells.append(cell);
        }
    }
}

bool GameMap::addTexture(QGraphicsRectItem *item, const QString &texturePath) {
    QPixmap texture(texturePath);
    if (texture.isNull()) {
        qDebug() << "Texture not found or failed to load:" << texturePath;
        return false;
    }
    texture = texture.scaled(cellSize, cellSize);
    item->setBrush(QBrush(texture));
    return true;
}

void GameMap::initialize() {
    QTimer *spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &GameMap::spawnEnemy);
    spawnTimer->start(20000);
}

void GameMap::spawnEnemy() {
    int totalEnemies = 4;
    QPointF spawnPosition(20, 350);
    QPointF targetPosition(500, 200);
    QTimer *spawnTimer = new QTimer(this);
    int *enemyCount = new int(0);
    connect(spawnTimer, &QTimer::timeout, this, [this, spawnTimer, enemyCount, totalEnemies, spawnPosition, targetPosition]() {
        if (*enemyCount < totalEnemies) {
            if (rand() % 2) {
                auto *enemya = new EnemyA(spawnPosition, targetPosition);
                scene->addItem(enemya);
                enemya->startMoving(80);
            } else {
                auto *enemyb = new EnemyB(spawnPosition, targetPosition);
                scene->addItem(enemyb);
                enemyb->startMoving(60);
            }
            (*enemyCount)++;
        } else {
            spawnTimer->stop();
            spawnTimer->deleteLater();
            delete enemyCount;
        }
    });
    spawnTimer->start(1000);
}

void GameMap::addAgents() {
    QColor colors[] = {Qt::blue, Qt::green, Qt::white, Qt::yellow};
    for (int j = 0; j < 4; ++j) {

        QColor agentColor = colors[j ];
        Agent *agent = new Agent(agentColor);
        agent->setRect((j+1) * cellSize, (rows+1) * cellSize, cellSize, cellSize);
        scene->addItem(agent);
        agents.append(agent);
    }
}

void GameMap::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem *clickedItem = scene->itemAt(scenePos, QTransform());

    if (Agent *clickedAgent = dynamic_cast<Agent*>(clickedItem)) {
        selectedAgent = clickedAgent;
        qDebug() << "Agent selected with color:" << clickedAgent->getColor();
        return;
    }

    if (QGraphicsRectItem *clickedBox = dynamic_cast<QGraphicsRectItem*>(clickedItem)) {
        if (boxCells.contains(clickedBox) && selectedAgent) {
            int x = clickedBox->rect().x() / cellSize;
            int y = clickedBox->rect().y() / cellSize;

            if (!isCellOccupied(x, y)) {
                clickedBox->setBrush(QBrush(selectedAgent->getColor()));
                occupyCell(x, y);
                qDebug() << "Changed cell color to:" << selectedAgent->getColor();
                selectedAgent = nullptr;
            } else {
                qDebug() << "Cell is already occupied!";
            }
        }
    }
}

bool GameMap::isCellOccupied(int x, int y) {
    if (x < 0 || x >= 10 || y < 0 || y >= 10) {
        return true;
    }
    return grid[x][y];
}

void GameMap::occupyCell(int x, int y) {
    if (x >= 0 && x < 10 && y >= 0 && y < 10) {
        grid[x][y] = true;
    }
}

void GameMap::releaseCell(int x, int y) {
    if (x >= 0 && x < 10 && y >= 0 && y < 10) {
        grid[x][y] = false;
    }
}

QGraphicsRectItem *GameMap::getCellAt(const QPointF &position) {
    QList<QGraphicsItem *> itemsAtPos = scene->items(position);
    for (QGraphicsItem *item : itemsAtPos) {
        QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(item);
        if (rectItem) {
            return rectItem;
        }
    }
    return nullptr;
}
