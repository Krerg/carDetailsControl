#include "createdetailcategorywindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QKeyEvent>

CreateDetailCategoryWindow::CreateDetailCategoryWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* v = new QVBoxLayout(this);
    this->categoryName = new QLineEdit();
    this->createButton = new QPushButton("Добавить");
    this->pb = new QProgressBar();
    pb->setMaximum(100);
    pb->setMinimum(0);
    pb->setValue(0);
    v->addWidget(categoryName);
    v->addWidget(createButton);
    v->addWidget(pb);
    pb->setVisible(false);
    QObject::connect(createButton,SIGNAL(clicked()),this,SLOT(createDetailCategory()));
}

void CreateDetailCategoryWindow::setParameters(QString path,QMap<QString, QStringList*>* map)
{
    this->path = path;
    this->map = map;
}

void CreateDetailCategoryWindow::createDetailCategory()
{
    pb->setVisible(true);
    QDir dir(path);
    QDir temp1;
    QDir temp2; //detail category
    QDir temp3; //detail
    int count = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size();
    int currentCount=0;
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        ++currentCount;
        pb->setValue((int)(((double)currentCount/count)*100));
        pb->update();
        temp1.setPath(path+"/"+dirName);
        foreach (QString dirName2, temp1.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            temp2.setPath(temp1.path()+"/"+dirName2+"/"+categoryName->text());
            if (!temp2.exists()) {
                temp2.mkpath(".");
            }
        }
    }
    this->map->insert(categoryName->text(),new QStringList());
    this->close();
    delete this;
}

void CreateDetailCategoryWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==16777220) {
        if(createButton->hasFocus()) {
            createButton->click();
        } else {
            createButton->setFocus();
        }
    }
}
