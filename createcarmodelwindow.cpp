#include "createcarmodelwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QDir>

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
    QDir dir(this->path+carModelName->text());
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QStringList::iterator i;
    for(i=categories->begin();i!=categories->end();i++)
    {
        dir.setPath(this->path+carModelName->text()+"/"+(*i));
        if (!dir.exists()) {
            dir.mkpath(".");
        }
    }

    this->close();
    delete this;
}

void CreateCarModelWindow::setCategoriesList(QStringList *j)
{
    this->categories = j;
}
