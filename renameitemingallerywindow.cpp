#include "renameitemingallerywindow.h"
#include "QVBoxLayout"
#include <QDir>
#include <QTextStream>
#include <QDebug>

RenameItemInGalleryWindow::RenameItemInGalleryWindow(QString path, QString oldName, QWidget *parent) :
    AbstractRenameWindow(parent)
{

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    newNameLabel = new QLabel("Новое название");
    newName->setText(oldName);
    proceedButton->setText("Переименовать");
    mainLayout->addWidget(newNameLabel);
    mainLayout->addWidget(newName);
    mainLayout->addWidget(proceedButton);
    this->path = path;
    this->oldName = oldName;
    QObject::connect(proceedButton,SIGNAL(clicked()),this, SLOT(renameGalleryItemSlot()));
}

void RenameItemInGalleryWindow::renameGalleryItemSlot()
{
    QDir dir;
    QString newNameText = newName->text();\
    bool sucessful = dir.rename(path+"/"+oldName,path+"/"+newNameText);

    qDebug()<<"Renaiming "<<path+"/"+oldName<<" into "<<path+"/"+newNameText<<" is "<<sucessful;
    if(sucessful) {
        emit updateGallerySignal();
    }
    delete this;
}
