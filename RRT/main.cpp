#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QVector>
#include <QMessageBox>
#include"mainmenu.h"
//#include"Gamemap.h"
#include"Enemy.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainMenu mainMenu;
    mainMenu.setWindowTitle("Rush Royale");

    mainMenu.resize(400, 300);
    mainMenu.show();

    return app.exec();
}

#include "main.moc"
