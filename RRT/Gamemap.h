

#ifndef GAMEMAP_H
#define GAMEMAP_H


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QtGui>
#include <QtCore>

class GameMap : public QGraphicsView {
    Q_OBJECT

public:
    GameMap(QWidget *parent = nullptr);


    void spawnEnemy();
    void initialize();
    void createMap();

    bool addTexture(QGraphicsRectItem *item, const QString &texturePath);

private:
    QGraphicsScene *scene;
    QTimer *spawnTimer;




    const int cellSize = 50;
    const int rows = 5;
    const int cols = 6;


};

#endif
