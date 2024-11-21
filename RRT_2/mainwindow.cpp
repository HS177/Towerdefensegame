#include "mainmenu.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GameMap *gamemap = new GameMap();

    gamemap->initialize();
    gamemap->show();
    gamemap->spawnEnemy();

    this->close();
    qDebug("Start Game!");
}

MainWindow::~MainWindow()
{
    delete ui;
}
