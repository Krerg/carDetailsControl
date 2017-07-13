#include "createcarmodelwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QDir>
#include <QKeyEvent>
#include <qDebug>

CreateCarModelWindow::CreateCarModelWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* v = new QVBoxLayout(this);
    this->carModelName = new QLineEdit();
    this->createButton = new QPushButton("Создать");
    QObject::connect(createButton,SIGNAL(clicked()),this,SLOT(createCarModel()));
    v->addWidget(carModelName);
    v->addWidget(createButton);
}

void CreateCarModelWindow::setPath(QString path)
{
    this->path = path+"/";
}

void CreateCarModelWindow::createCarModel()
{
    QDir dir(this->path+carModelName->text().simplified().replace("\\", ".").replace("/", "-"));
    QDir temp;
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QStringList* tempDetailList;
    QStringList::Iterator listIterator;
    QMap<QString, QStringList*>::const_iterator i = detailsMap->begin();
    while(i != detailsMap->constEnd()) {

        dir.setPath(this->path+carModelName->text().simplified().replace("\\", ".").replace("/", "-")+"/"+i.key());
//        if(dir.path().contains("chevrolete")){
//            qDebug()<<dir.path();
//            qDebug()<<dir.path().replace("\\", "-");
//            qDebug()<<dir.path().replace("/", "-");
//        }
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        tempDetailList = i.value();
        for(listIterator = tempDetailList->begin();listIterator!=tempDetailList->end();listIterator++) {
            temp.setPath(dir.path()+"/"+(*listIterator));
            if(!temp.exists()) {
                temp.mkpath(".");
            }
        }
        ++i;
    }
    this->close();
    delete this;
}

void CreateCarModelWindow::setCategoriesList(QMap<QString, QStringList*>* map)
{
    this->detailsMap = map;
}

void CreateCarModelWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==16777220) {
        if(createButton->hasFocus()) {
            createButton->click();
        } else {
            createButton->setFocus();
        }
    }
}
