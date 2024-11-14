// GameMap.cpp

#include "Gamemap.h"
#include <QRandomGenerator>
//#include "./ui_Gamemap.h"
#include <QTimer>
#include <QPixmap>
#include <QDebug>
#include <QtGui>
#include"Enemy.h"
GameMap::GameMap(QWidget *parent) : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
    setScene(scene);

    /*scene->setSceneRect(-30,-60,cols * cellSize+50 , rows * cellSize+100 );
    QPen mypen =QPen(Qt::red);
    QLineF Topline(scene->sceneRect().topLeft(),scene->sceneRect().topRight());
     QLineF Leftline(scene->sceneRect().topLeft(),scene->sceneRect().bottomLeft());
     QLineF Rightline(scene->sceneRect().topRight(),scene->sceneRect().bottomRight());
    QLineF Bottomline(scene->sceneRect().bottomLeft(),scene->sceneRect().bottomRight());

     scene->addLine(Topline,mypen);
      scene->addLine(Leftline,mypen);
     scene->addLine(Rightline,mypen);
      scene->addLine(Bottomline,mypen);*/

    createMap();
    setFixedSize(cols * cellSize + 100, rows * cellSize +200);
    /*scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(-200,-200,300,300);*/
}

void GameMap::createMap() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QGraphicsRectItem *cell = scene->addRect(j * cellSize, i * cellSize, cellSize, cellSize);
            cell->setPen(QPen(Qt::black));


            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                if (!addTexture(cell, ":/textures/path_texture.png")) {
                    cell->setBrush(QBrush(QColor(200, 50, 50)));
                }
            }


            if (i >= 1 && i <= 4 && j >= 1 && j <= 4) {
                if (!addTexture(cell, ":/textures/agent_texture.png")) {
                    cell->setBrush(QBrush(QColor(50, 50, 200)));
                }
            }


            if (i == rows - 1 && j == 0) {
                cell->setBrush(QBrush(QColor(0, 255, 0, 180)));
                cell->setPen(QPen(QColor(0, 200, 0), 2));
            }

            else if (i == rows - 1 && j == cols - 1) {
                cell->setBrush(QBrush(QColor(255, 0, 0, 180)));
                cell->setPen(QPen(QColor(200, 0, 0), 2));
            }
        }
    }
    for (int i = rows+1; i < rows+2; ++i) {
        for (int j = 1; j < cols-1; ++j) {
            QGraphicsRectItem *cell = scene->addRect(j * cellSize, i * cellSize, cellSize, cellSize);
            cell->setPen(QPen(Qt::black));


            cell->setBrush(QBrush(QColor(150, 150, 150)));


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
    spawnTimer->start(1000);
}

void GameMap::spawnEnemy() {
    if (rand() %2){
    EnemyA *enemya = new EnemyA(QPointF(0, 200) ,QPointF(500, 200));
         scene->addItem(enemya);
    enemya->startMoving(150);
    }else{
        EnemyB *enemyb = new EnemyB(QPointF(0, 200) ,QPointF(500, 200));
        scene->addItem(enemyb);
         enemyb->startMoving(100);
    }



}

