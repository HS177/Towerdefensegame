#ifndef AGENT_H
#define AGENT_H

#include <QGraphicsRectItem>
#include <QColor>

class Agent : public QGraphicsRectItem {
public:
    explicit Agent(const QColor &color, QGraphicsItem *parent = nullptr);

    QColor getColor() const;
    void setColor(const QColor &color);

private:
    QColor color;
};

#endif
