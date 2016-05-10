#include "mainwindow.h"
#include <QApplication>
#include "updatingwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow();
    w->setWindowTitle("Car Shop v 1.0");
    w->show();
    return a.exec();
}
