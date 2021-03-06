#include "renamecarmakewindow.h"
#include "QVBoxLayout"
#include <QDir>
#include <QTextStream>
#include <QDebug>

RenameCarMakeWindow::RenameCarMakeWindow(QString path, QString oldName, QWidget *parent) :
    AbstractRenameWindow(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    carMakeNameLabel = new QLabel("Новое название");
    newName->setText(oldName);
    proceedButton->setText("Переименовать");
    mainLayout->addWidget(carMakeNameLabel);
    mainLayout->addWidget(newName);
    mainLayout->addWidget(proceedButton);
    this->path = path;
    this->oldName = oldName;
    QObject::connect(proceedButton,SIGNAL(clicked()),this, SLOT(renameCarMakeSlot()));
}

void RenameCarMakeWindow::renameCarMakeSlot()
{
    QDir dir;
    QString newNameText = newName->text();
    bool sucessful = dir.rename(path+"/"+oldName,path+"/"+newNameText);

    qDebug()<<"Renaiming "<<path+"/"+oldName<<" into "<<path+"/"+newNameText<<" is "<<sucessful;
    if(sucessful) {
        emit updateArticlesInfos();
    }
    delete this;
}
