#include "Agent.h"
#include <QPen>

Agent::Agent(const QColor &color, QGraphicsItem *parent)
    : QGraphicsRectItem(parent), color(color) {
    setBrush(QBrush(color));
    setPen(QPen(Qt::black));
}

QColor Agent::getColor() const {
    return color;
}

void Agent::setColor(const QColor &color) {
    this->color = color;
    setBrush(QBrush(color));
}
