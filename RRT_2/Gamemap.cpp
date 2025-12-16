#include "Gamemap.h"

#include <QDebug>
#include <QPixmap>
#include <QSet>
#include <QTransform>

#include <limits>

GameMap::GameMap(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
    setScene(scene);

    tileRoles = QVector<QVector<TileRole>>(rows, QVector<TileRole>(cols, TileRole::Empty));

    createMap();
    addAgents();

    pathPoints_ = buildPathFromGrid();

    waveSpawner = new WaveSpawner(scene, this);
    waveSpawner->setPath(pathPoints_);

    waves_ = {
        WaveSpawner::WaveDefinition{4, 1000, {WaveSpawner::EnemyKind::A, WaveSpawner::EnemyKind::B}},
        WaveSpawner::WaveDefinition{6, 800, {WaveSpawner::EnemyKind::A, WaveSpawner::EnemyKind::B}},
    };
    waveSpawner->setWaves(waves_);
    waveSpawner->setBetweenWavesMs(20000);

    resize(cols * cellSize + 600, rows * cellSize + 200);
}

void GameMap::createMap() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QGraphicsRectItem *cell = scene->addRect(j * cellSize, i * cellSize, cellSize, cellSize);
            cell->setPen(QPen(Qt::black));

            TileRole role = TileRole::Empty;
            if (i == rows - 1 && j == 0) {
                role = TileRole::Spawn;
            } else if (i == rows - 1 && j == cols - 1) {
                role = TileRole::Exit;
            } else if (i == 0 || j == 0 || j == cols - 1) {
                role = TileRole::Path;
            }

            if (i >= 1 && i <= rows - 1 && j >= 1 && j <= cols - 2) {
                role = TileRole::Buildable;
            }

            tileRoles[i][j] = role;

            if (role == TileRole::Buildable) {
                if (!addTexture(cell, ":/textures/agent_texture.png")) {
                    cell->setBrush(QBrush(QColor(200, 200, 200)));
                }
                boxCells.append(cell);
            } else if (isPathRole(role)) {
                if (!addTexture(cell, ":/textures/path_texture.png")) {
                    cell->setBrush(QBrush(QColor(200, 10, 50)));
                }
            }

            if (role == TileRole::Spawn) {
                cell->setBrush(QBrush(QColor(0, 255, 0, 180)));
                cell->setPen(QPen(QColor(0, 200, 0), 2));
            } else if (role == TileRole::Exit) {
                cell->setBrush(QBrush(QColor(255, 0, 0, 180)));
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
    if (waveSpawner) {
        waveSpawner->start();
    }
}

void GameMap::spawnEnemy() {
    if (waveSpawner) {
        waveSpawner->startNextWaveNow();
    }
}

void GameMap::addAgents() {
    QColor colors[] = {Qt::red, Qt::green, Qt::white, Qt::yellow};

    for (int j = 1; j < cols - 1; ++j) {
        QColor agentColor = colors[j % 4];
        if (j % 4 == 0) {
            QColor temp = colors[2];
            colors[2] = colors[1];
            colors[1] = temp;
        }
        Agent *agent = new Agent(agentColor);
        agent->setRect(j * cellSize, (rows + 1) * cellSize, cellSize, cellSize);
        scene->addItem(agent);
        agents.append(agent);
    }
}

void GameMap::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem *clickedItem = scene->itemAt(scenePos, QTransform());

    if (Agent *clickedAgent = dynamic_cast<Agent *>(clickedItem)) {
        selectedAgent = clickedAgent;
        qDebug() << "Agent selected with color:" << clickedAgent->getColor();
        return;
    }

    if (QGraphicsRectItem *clickedBox = dynamic_cast<QGraphicsRectItem *>(clickedItem)) {
        if (boxCells.contains(clickedBox) && selectedAgent) {
            clickedBox->setBrush(QBrush(selectedAgent->getColor()));
            qDebug() << "Changed cell color to:" << selectedAgent->getColor();
            selectedAgent = nullptr;
        }
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

bool GameMap::isPathRole(TileRole role) const {
    return role == TileRole::Path || role == TileRole::Spawn || role == TileRole::Exit;
}

QPointF GameMap::cellToPathPoint(int row, int col) const {
    constexpr qreal enemySize = 40.0;
    const qreal offset = (cellSize - enemySize) / 2.0;
    return {col * cellSize + offset, row * cellSize + offset};
}

QVector<QPointF> GameMap::buildPathFromGrid() const {
    QPoint spawn(-1, -1);
    QPoint exit(-1, -1);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (tileRoles[row][col] == TileRole::Spawn) {
                spawn = QPoint(col, row);
            } else if (tileRoles[row][col] == TileRole::Exit) {
                exit = QPoint(col, row);
            }
        }
    }

    if (spawn.x() < 0 || exit.x() < 0) {
        return {};
    }

    QVector<QPointF> path;
    QSet<QPoint> visited;

    QPoint current = spawn;
    QPoint previous(-999, -999);

    auto isValidCell = [&](const QPoint &cell) {
        return cell.x() >= 0 && cell.x() < cols && cell.y() >= 0 && cell.y() < rows;
    };

    auto manhattan = [](const QPoint &a, const QPoint &b) {
        return qAbs(a.x() - b.x()) + qAbs(a.y() - b.y());
    };

    while (true) {
        path.append(cellToPathPoint(current.y(), current.x()));
        if (current == exit) {
            break;
        }

        visited.insert(current);

        QVector<QPoint> neighbors;
        const QVector<QPoint> directions = {QPoint(1, 0), QPoint(-1, 0), QPoint(0, 1), QPoint(0, -1)};
        for (const QPoint &dir : directions) {
            const QPoint next = current + dir;
            if (!isValidCell(next)) {
                continue;
            }
            if (!isPathRole(tileRoles[next.y()][next.x()])) {
                continue;
            }
            if (next == previous) {
                continue;
            }
            neighbors.append(next);
        }

        QPoint bestNext(-1, -1);
        int bestDistance = std::numeric_limits<int>::max();
        for (const QPoint &candidate : neighbors) {
            if (visited.contains(candidate)) {
                continue;
            }

            const int dist = manhattan(candidate, exit);
            if (dist < bestDistance) {
                bestDistance = dist;
                bestNext = candidate;
            }
        }

        if (bestNext.x() < 0) {
            break;
        }

        previous = current;
        current = bestNext;
    }

    return path;
}
