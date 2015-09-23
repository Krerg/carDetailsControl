#include "renamecarmakewindow.h"
#include "QVBoxLayout"
#include <QDir>
#include <QTextStream>
#include <QDebug>

RenameCarMakeWindow::RenameCarMakeWindow(QString path, QString oldName, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    carMakeNameLabel = new QLabel("Название марки");
    carMakeNameInput = new QLineEdit();
    proceedButton = new QPushButton("Переименовать");
    mainLayout->addWidget(carMakeNameLabel);
    mainLayout->addWidget(carMakeNameInput);
    mainLayout->addWidget(proceedButton);
    this->path = path;
    this->oldName = oldName;
    QObject::connect(proceedButton,SIGNAL(clicked()),this, SLOT(renameCarMakeSlot()));
}

void RenameCarMakeWindow::renameCarMakeSlot()
{
    QDir* dir = new QDir();
    QString newName = carMakeNameInput->text();
    bool sucessful = dir->rename(path+"/"+oldName,path+"/"+newName);
    qDebug()<<"Renaiming "<<path+"/"+oldName<<" into "<<path+"/"+newName<<" is "<<sucessful;
    if(sucessful) {
        emit updateArticlesInfos();
    }
    delete this;
}
