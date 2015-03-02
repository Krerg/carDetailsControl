#include "deletedetailcategorywindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QMap>

DeleteDetailCategoryWindow::DeleteDetailCategoryWindow(QWidget *parent) :
    QWidget(parent)
{
    pb = new QProgressBar();
    QVBoxLayout *v = new QVBoxLayout(this);
    QHBoxLayout *h = new QHBoxLayout();
    msg = new QLabel("Вы точно хотите это сделать?", this);
    confirmButton = new QPushButton("OK",this);
    cancelButton = new QPushButton("Отмена",this);
    v->addWidget(msg);
    v->addStrut(10);
    v->addItem(h);
    v->addWidget(pb);
    pb->setVisible(false);
    pb->setMaximum(100);
    pb->setValue(0);
    pb->setMinimum(0);
    h->addWidget(confirmButton);
    h->addWidget(cancelButton);
    this->setLayout(v);
    QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelAction()));
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(deleteCategory()));
}

void DeleteDetailCategoryWindow::deleteCategory()
{
    //поехали
    pb->setVisible(true);
    QDir dir(path);
    QDir temp1;
    QDir temp2;
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
            temp2.setPath(temp1.path()+"/"+dirName2+"/"+categoryName);
            if (temp2.exists()) {
                temp2.removeRecursively();
            }
        }
    }
    this->map->remove(categoryName);
    this->close();
    delete this;
}

void DeleteDetailCategoryWindow::cancelAction()
{
    this->close();
    delete this;
}

void DeleteDetailCategoryWindow::setParameters(QString path, QMap<QString, QStringList*>* map,QString categoryName)
{
    this->path = path;
    this->map = map;
    this->categoryName = categoryName;
}
