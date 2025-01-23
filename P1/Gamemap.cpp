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
#include <QGraphicsView>
#include "Agent.h"
#include "Enemy.h"
#include "bullet.h"

GameMap::GameMap(QWidget *parent) : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
    this->setScene(scene);



    this->installEventFilter(this);


    scene->setBackgroundBrush(QPixmap(":/new/prefix2/bricks.png"));

    createMap();
    addAgents();

    resize(cols * cellSize + 600, rows * cellSize + 200);


    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            grid[i][j] = false;
        }
    }



    initialize();
    startElixirCounter();
    initialize();
}
void GameMap::startElixirCounter() {
     elixirTimer = new QTimer(this);
    connect(elixirTimer, &QTimer::timeout, this, [this]() {
        if (elixirCounter < 10) {
            elixirCounter++;
            elixirCounterText->setPlainText(QString::number(elixirCounter));

        } else {
            elixirTimer->stop();
            qDebug() << "Elixir has reached the maximum value of 10.";
        }
    });
    elixirTimer->start(2000);
}


void GameMap::createMap() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QGraphicsRectItem *cell = scene->addRect(j * cellSize, i * cellSize, cellSize, cellSize);
            cell->setPen(QPen(Qt::black));

            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                if (!addTexture(cell, ":/textures/path_texture.png")) {
                    cell->setBrush(QBrush(QColor(200, 200, 200, 180)));
                }
            }

            if (i >= 1 && i <= 4 && j >= 1 && j <= 4) {
                if (!addTexture(cell, ":/textures/agent_texture.png")) {
                    cell->setBrush(QBrush(QColor(200, 200, 200)));
                }
                boxCells.append(cell);
            }

            if (i == rows-1 && j == 0) {
                cell->setBrush(QBrush(QColor(200, 255, 200, 180)));
                cell->setPen(QPen(QColor(0, 200, 0), 2));
            } else if (i == rows - 1 && j == cols - 1) {
                cell->setBrush(QBrush(QColor(255, 200, 200, 180)));
                cell->setPen(QPen(QColor(200, 0, 0), 2));
            }
        }
    }


    for (int i = rows ; i < rows + 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            QGraphicsRectItem *cell = scene->addRect(j * cellSize, i * cellSize, cellSize, cellSize);
            cell->setPen(QPen(Qt::black));
            cell->setBrush(QBrush(QColor(150, 150, 150)));
            agentCells.append(cell);
        }
    }
    for (int i =  rows+1; i < rows +2; ++i) {
        for (int j = 0; j < cols ; ++j) {
            QGraphicsRectItem *cell = scene->addRect(j * cellSize, i * cellSize, cellSize, cellSize);
            cell->setPen(QPen(Qt::black));
            cell->setBrush(QBrush(QColor(150, 150, 150)));
            levelcells.append(cell);

        }
    }
    for (int i=rows+1; i< rows +2; ++i){
        for (int j=-2;j<-1;++j){
            QGraphicsRectItem *cell= scene->addRect(j*cellSize,i*cellSize,cellSize,cellSize);


            QPixmap elixirImage(":/new/prefix2/PICTURES/elixirBottle.jpg");
            QBitmap mask = elixirImage.createMaskFromColor(QColor(255, 255, 255));
            elixirImage.setMask(mask);

            QGraphicsPixmapItem *pixmapItem = scene->addPixmap(elixirImage.scaled(cellSize, cellSize));
            pixmapItem->setPos((j+0.1) * cellSize, i * cellSize);
            pixmapItem->setZValue(0);


            elixirCounterText = new QGraphicsTextItem(QString::number(elixirCounter));

                elixirCounterText->setDefaultTextColor(Qt::white);
            elixirCounterText->setFont(QFont("Bold", 18));
            elixirCounterText->setPos((j-0.1) * cellSize + cellSize / 4, (i+0.5) * cellSize - cellSize / 4);
            elixirCounterText->setZValue(1);
            scene->addItem(elixirCounterText);
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


bool isSpawning = false;

void GameMap::initialize() {
    QTimer *spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &GameMap::spawnEnemyWave);
    spawnTimer->start(20000);
}

void GameMap::spawnEnemyWave() {
    if (isSpawning) return;
    isSpawning = true;

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
                enemya->startMoving(30);

            } else {
                auto *enemyb = new EnemyB(spawnPosition, targetPosition);
                scene->addItem(enemyb);
                enemyb->startMoving(40);

            }
            (*enemyCount)++;
        } else {
            spawnTimer->stop();
            spawnTimer->deleteLater();
            delete enemyCount;
            isSpawning = false;
        }
    });

    spawnTimer->start(1000);
}

void GameMap::addAgents() {
    QColor colors[] = {Qt::blue, Qt::green, Qt::white, Qt::yellow};
    for (int j = 0; j < 4; ++j) {
        Agent *agent = nullptr;

        if (colors[j] == Qt::blue ) {
            agent = new EndStrikerAgent(colors[j]);
        } else if (colors[j] == Qt::green) {
            agent = new maxHealthstricker(colors[j]);
        } else if (colors[j] == Qt::yellow) {
            agent = new FirstStrickerAgent(colors[j]);
        } else if (colors[j] == Qt::white) {
            agent = new RandomStrickerAgent(colors[j]);
        }
        if (agent) {
            agent->setRect((j + 1) * cellSize, (rows ) * cellSize-1, cellSize, cellSize);
            scene->addItem(agent);
            agents.append(agent);
        }
    }
}
void GameMap::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem *clickedItem = scene->itemAt(scenePos, QTransform());

    if (Agent *clickedAgent = dynamic_cast<Agent *>(clickedItem)) {
        if (!selectedAgent) {
            selectedAgent = clickedAgent;
            qDebug() << "Agent selected with color:" << clickedAgent->getColor();
        }
        return;
    }

    if (QGraphicsRectItem *clickedBox = dynamic_cast<QGraphicsRectItem *>(clickedItem)) {
        if (boxCells.contains(clickedBox) && selectedAgent) {
            int x = clickedBox->rect().x() / cellSize;
            int y = clickedBox->rect().y() / cellSize;

            if (!isCellOccupied(x, y)) {
                QRectF previousRect = selectedAgent->rect();
                int previousX = previousRect.x() / cellSize;
                int previousY = previousRect.y() / cellSize;

                int requiredElixir = selectedAgent->getElixirN();

                if (elixirCounter >= requiredElixir) {
                    selectedAgent->setRect(clickedBox->rect());
                    occupyCell(x, y);
                    selectedAgent->startShooting();
                    releaseCell(previousX, previousY);

                    Agent *replacementAgent = nullptr;
                    int randomType = rand() % 4;
                    switch (randomType) {
                    case 0:
                        replacementAgent = new EndStrikerAgent(Qt::blue);
                        break;
                    case 1:
                        replacementAgent = new FirstStrickerAgent(Qt::yellow);
                        break;
                    case 2:
                        replacementAgent = new RandomStrickerAgent(Qt::white);
                        break;
                    case 3:
                        replacementAgent = new maxHealthstricker(Qt::green);
                        break;
                    }

                    if (replacementAgent) {
                        replacementAgent->setRect(previousRect);
                        scene->addItem(replacementAgent);
                        agents.append(replacementAgent);
                        occupyCell(previousX, previousY);
                    }

                    elixirCounter -= requiredElixir;
                    if (!elixirTimer->isActive()) {
                        startElixirCounter();
                    }

                    selectedAgent = nullptr;
                    qDebug() << "Agent placed and previous cell filled with a new agent.";
                } else {
                    qDebug() << "Not enough elixir to deploy this agent!";
                }
            } else {
                qDebug() << "Cell is already occupied!";
            }
        }
    }


}


bool GameMap::isCellOccupied(int x, int y) {
    return x < 0 || x >= 10 || y < 0 || y >= 10 || grid[x][y];
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
