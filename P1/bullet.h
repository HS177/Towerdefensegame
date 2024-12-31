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
        setRect(0, 0, 10, 10); // Bullet size
        setBrush(Qt::red);   // Bullet color
        setPen(Qt::NoPen);

        setPos(startPos);    // Starting position
        velocity = direction * 3; // Adjust speed as needed

        // Automatically move the bullet every frame
        QTimer *moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, this, &Projectile::move);
        moveTimer->start(16); // ~60 FPS
    }

private slots:
    void move() {
        moveBy(velocity.x(), velocity.y());

        // Remove projectile if it's out of bounds
        if (!scene()->sceneRect().contains(sceneBoundingRect())) {
            scene()->removeItem(this);
            delete this;
        }

    }

private:
    QPointF velocity; // Direction and speed
};
#endif
