#include "mainmenu.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create a GameMap object
    GameMap *gamemap = new GameMap(this); // Set MainWindow as parent
    gamemap->initialize();

    // Create a QLabel for the picture
    QLabel *pictureLabel = new QLabel(this);
    pictureLabel->setPixmap(QPixmap(":/new/prefix2/images.jpg"));
    pictureLabel->setAlignment(Qt::AlignCenter); // Optional: Center the image

    // Ensure centralwidget has a layout, or create one
    if (!ui->centralwidget->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(layout);
    }

    // Add QLabel (picture) and GameMap to the layout
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->centralwidget->layout());
    layout->addWidget(pictureLabel); // Add the picture first (or adjust order as needed)
    layout->addWidget(gamemap);     // Add GameMap after the picture

    // Perform other operations
    gamemap->spawnEnemy();

    qDebug("Start Game!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

