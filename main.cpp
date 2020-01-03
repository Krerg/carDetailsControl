#include "mainwindow.h"
#include <QApplication>
#include "updatingwindow.h"
#include <QDateTime>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow();
    QDateTime current = QDateTime::currentDateTime();
    QString curBuild = "Car Shop Build: ";
    QString curString = current.toString("yy.M.dd");
    qDebug()<<curString;
    curBuild.append(curString);
    w->setWindowTitle(curBuild);
    w->show();
    return a.exec();
}
