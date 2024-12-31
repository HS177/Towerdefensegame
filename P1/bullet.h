#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsRectItem>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QTimer>

class Projectile : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    explicit Projectile(const QPointF &startPos, const QPointF &direction, QObject *parent = nullptr)
        : QObject(parent), QGraphicsEllipseItem() {
        setRect(0, 0, 10, 10);
        setBrush(Qt::red);
        setPen(Qt::NoPen);

        setPos(startPos);
        velocity = direction * 3;


        QTimer *moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, this, &Projectile::move);
        moveTimer->start(16);
    }

private slots:
    void move() {
        moveBy(velocity.x(), velocity.y());


        if (!scene()->sceneRect().contains(sceneBoundingRect())) {
            scene()->removeItem(this);
            delete this;
        }

    }

private:
    QPointF velocity;
};
#endif
