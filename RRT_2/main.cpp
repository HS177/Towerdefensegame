#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QVector>
#include <QMessageBox>
#include"mainmenu.h"
#include "mainwindow.h"
#include"Gamemap.h"
#include"Enemy.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    MainMenu mainMenu;
    mainMenu.setWindowTitle("Rush Royale");

    mainMenu.resize(1000, 600);
    mainMenu.show();

    return app.exec();
}
#include "main.moc"
