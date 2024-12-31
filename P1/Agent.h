#ifndef AGENT_H
#define AGENT_H

#include <QGraphicsRectItem>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QTimer>
#include "bullet.h"




class Agent : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Agent(const QColor &color, QObject *parent = nullptr);
    ~Agent();


    QColor getColor() const;
    void setColor(const QColor &color);


public slots:
    void shoot( );

public:


    QColor color;
    Projectile *bullet;
    QTimer *shootTimer;
};


#endif
