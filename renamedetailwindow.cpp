#include "renamedetailwindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>

RenameDetailWindow::RenameDetailWindow(QString path, QString selectedCategory, QString oldName, QMap<QString, QStringList*>* map, QWidget *parent) :
    AbstractRenameWindow(parent), selectedCategory(selectedCategory)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    detailInfoLabel = new QLabel("Новое название");
    newName->setText(oldName);
    proceedButton->setText("Переименовать");

    mainLayout->addWidget(detailInfoLabel);
    mainLayout->addWidget(newName);
    mainLayout->addWidget(proceedButton);

    this->path = path;
    this->oldName = oldName;
    this->selectedCategory = selectedCategory;
    this->map = map;

    QObject::connect(proceedButton,SIGNAL(clicked()),this,SLOT(renameDetailSlot()));
}

void RenameDetailWindow::renameDetailSlot()
{
    QDir dir(path);
    QDir carModelDir;
    QDir detailCategoryDir; //detail category
    QDir detailDir; //detail
    QDir tmpDir;
    QString oldPath;
    QString newPath;
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        carModelDir.setPath(path+"/"+dirName);
        foreach (QString dirName2, carModelDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            detailCategoryDir.setPath(carModelDir.path()+"/"+dirName2);
            oldPath = detailCategoryDir.path()+"/"+selectedCategory+"/"+oldName;
            newPath = detailCategoryDir.path()+"/"+selectedCategory+"/"+newName->text();
            bool sucessful = tmpDir.rename(oldPath,newPath);
            qDebug()<<"Renaming "+oldPath+" into "+newPath+" is "+sucessful;
        }
    }
    QStringList* detail = map->value(selectedCategory);
    QList<QString>::Iterator i;
    int count = 0;
    for(i=detail->begin();i!=detail->end();i++) {
        if((*i)==oldName) {
            detail->replace(count,newName->text());
        }
        count++;
    }
    this->hide();
    delete detailInfoLabel;
    delete newName;
    delete proceedButton;
    emit updateArticlesInfos();
    delete this;
}
