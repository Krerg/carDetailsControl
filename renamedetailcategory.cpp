#include "renamedetailcategory.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>

RenameDetailCategory::RenameDetailCategory(QString path, QString oldName,QMap<QString, QStringList*>* map, QWidget *parent) :
    AbstractRenameWindow(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    detailCategoryLabel = new QLabel("Новое название");
    proceedButton->setText("Переименовать");
    newName->setText(oldName);
    mainLayout->addWidget(detailCategoryLabel);
    mainLayout->addWidget(newName);
    mainLayout->addWidget(proceedButton);
    this->path = path;
    this->oldName = oldName;
    this->map = map;
    QObject::connect(proceedButton,SIGNAL(clicked()),this, SLOT(renameDetailCategorySlot()));
}

void RenameDetailCategory::renameDetailCategorySlot()
{
    QDir dir(path);
    QDir carModelDir;
    QDir detailCategoryDir; //detail category
    QDir tmpDir;
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        carModelDir.setPath(path+"/"+dirName);
        foreach (QString dirName2, carModelDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            bool sucessful = tmpDir.rename(carModelDir.path()+"/"+dirName2+"/"+oldName,carModelDir.path()+"/"+dirName2+"/"+newName->text());
            qDebug()<<"Renaming "+ carModelDir.path()+"/"+dirName2+"/"+oldName+" into "+carModelDir.path()+"/"+dirName2+"/"+newName->text()+" is "+sucessful;
        }
    }
    QStringList* details = this->map->value(oldName);
    this->map->remove(oldName);
    this->map->insert(newName->text(),details);
    this->close();
    emit updateArticlesInfos();
    delete this;
}
