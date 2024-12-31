#include "Agent.h"
#include "Enemy.h"
#include "bullet.h"

Agent::Agent(const QColor &color, QObject *parent)
    : QObject(parent), QGraphicsEllipseItem() {
    this->color = color;


    setRect(0, 0, 20, 20);
    setBrush(QBrush(color));
    setPen(QPen(Qt::black));


    shootTimer = new QTimer(this);
    connect(shootTimer, &QTimer::timeout, this, &Agent::shoot);

}

Agent::~Agent() {

    if (shootTimer) {
        shootTimer->stop();
        delete shootTimer;
    }
}

QColor Agent::getColor() const {
    return color;
}

void Agent::setColor(const QColor &color) {
    this->color = color;
    setBrush(QBrush(color));
}

void Agent::shoot() {

    QPointF localCenter = boundingRect().center();
    QPointF startPos = mapToScene(localCenter);

    QPointF direction(0,-2);
    bullet = new Projectile(startPos, direction);
    shootTimer->start(500);

    if (scene()) {
        scene()->addItem(bullet);
    }
}
