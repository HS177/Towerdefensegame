#include "WaveSpawner.h"

#include <QGraphicsScene>
#include <QRandomGenerator>

#include <utility>

#include "Enemy.h"

WaveSpawner::WaveSpawner(QGraphicsScene *scene, QObject *parent)
    : QObject(parent), scene_(scene) {
    connect(&timer_, &QTimer::timeout, this, &WaveSpawner::onTimeout);
}

void WaveSpawner::setPath(QVector<QPointF> path) {
    path_ = std::move(path);
}

void WaveSpawner::setWaves(QVector<WaveDefinition> waves) {
    waves_ = std::move(waves);
    currentWaveIndex_ = 0;
    spawnedInWave_ = 0;
}

void WaveSpawner::setBetweenWavesMs(int ms) {
    betweenWavesMs_ = ms;
}

bool WaveSpawner::isRunning() const {
    return running_;
}

void WaveSpawner::start() {
    if (running_) {
        return;
    }
    if (!scene_ || path_.size() < 2 || waves_.isEmpty()) {
        return;
    }

    running_ = true;
    beginWave(0);
}

void WaveSpawner::stop() {
    running_ = false;
    waitingForNextWave_ = false;
    timer_.stop();
}

void WaveSpawner::startNextWaveNow() {
    if (!scene_ || path_.size() < 2 || waves_.isEmpty()) {
        return;
    }

    running_ = true;
    waitingForNextWave_ = false;
    timer_.stop();
    beginWave(currentWaveIndex_);
}

void WaveSpawner::beginWave(int waveIndex) {
    if (waves_.isEmpty() || !scene_ || path_.size() < 2) {
        return;
    }

    currentWaveIndex_ = waveIndex % waves_.size();
    spawnedInWave_ = 0;
    waitingForNextWave_ = false;

    const auto &wave = waves_[currentWaveIndex_];
    if (wave.count <= 0) {
        scheduleNextWave();
        return;
    }

    timer_.setSingleShot(false);
    timer_.setInterval(wave.cadenceMs);

    spawnOne();
    ++spawnedInWave_;

    if (spawnedInWave_ >= wave.count) {
        scheduleNextWave();
        return;
    }

    timer_.start();
}

void WaveSpawner::scheduleNextWave() {
    timer_.stop();

    if (!running_ || waves_.isEmpty()) {
        return;
    }

    waitingForNextWave_ = true;
    currentWaveIndex_ = (currentWaveIndex_ + 1) % waves_.size();

    timer_.setSingleShot(true);
    timer_.setInterval(betweenWavesMs_);
    timer_.start();
}

void WaveSpawner::spawnOne() {
    if (!scene_ || path_.isEmpty() || waves_.isEmpty()) {
        return;
    }

    const auto &wave = waves_[currentWaveIndex_];

    EnemyKind kind = EnemyKind::A;
    if (!wave.mix.isEmpty()) {
        kind = wave.mix.at(QRandomGenerator::global()->bounded(wave.mix.size()));
    }

    EnemyBase *enemy = nullptr;
    double speed = 80.0;

    switch (kind) {
    case EnemyKind::A:
        enemy = new EnemyA(path_);
        speed = 80.0;
        break;
    case EnemyKind::B:
        enemy = new EnemyB(path_);
        speed = 60.0;
        break;
    }

    if (!enemy) {
        return;
    }

    scene_->addItem(enemy);
    enemy->startMoving(speed);
}

void WaveSpawner::onTimeout() {
    if (!running_ || waves_.isEmpty()) {
        return;
    }

    if (waitingForNextWave_) {
        beginWave(currentWaveIndex_);
        return;
    }

    const auto &wave = waves_[currentWaveIndex_];
    if (spawnedInWave_ < wave.count) {
        spawnOne();
        ++spawnedInWave_;
    }

    if (spawnedInWave_ >= wave.count) {
        scheduleNextWave();
    }
}
