#include "createcarmakewindow.h"
#include <QVBoxLayout>
#include <QDir>

CreateCarMakeWindow::CreateCarMakeWindow(QWidget *parent) :
    QWidget(parent)
{
    this->carMakeName = new QLineEdit(this);
    this->createButton = new QPushButton("Создать",this);
    QVBoxLayout *v = new QVBoxLayout(this);
    v->addWidget(carMakeName);
    v->addSpacing(10);
    v->addWidget(createButton);

    QObject::connect(createButton,SIGNAL(clicked()),this,SLOT(createCarMake()));
}

void CreateCarMakeWindow::setPath(QString path)
{
    this->path = path+"/";
}

void CreateCarMakeWindow::createCarMake()
{
    QDir dir(this->path+carMakeName->text());
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    this->close();
    delete this;
}

CreateCarMakeWindow::~CreateCarMakeWindow()
{

}

