#ifndef WAVESPAWNER_H
#define WAVESPAWNER_H

#include <QObject>
#include <QPointF>
#include <QTimer>
#include <QVector>

class QGraphicsScene;

class WaveSpawner : public QObject {
    Q_OBJECT

public:
    enum class EnemyKind {
        A,
        B,
    };

    struct WaveDefinition {
        int count = 0;
        int cadenceMs = 1000;
        QVector<EnemyKind> mix;
    };

    explicit WaveSpawner(QGraphicsScene *scene, QObject *parent = nullptr);

    void setPath(QVector<QPointF> path);
    void setWaves(QVector<WaveDefinition> waves);
    void setBetweenWavesMs(int ms);

    void start();
    void stop();
    bool isRunning() const;

    void startNextWaveNow();

private slots:
    void onTimeout();

private:
    void beginWave(int waveIndex);
    void scheduleNextWave();
    void spawnOne();

    QGraphicsScene *scene_ = nullptr;
    QTimer timer_;

    QVector<QPointF> path_;
    QVector<WaveDefinition> waves_;

    int betweenWavesMs_ = 20000;
    int currentWaveIndex_ = 0;
    int spawnedInWave_ = 0;

    bool running_ = false;
    bool waitingForNextWave_ = false;
};

#endif // WAVESPAWNER_H
