#include "createcarmakewindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QKeyEvent>
#include <qDebug>
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
    QDir dir(this->path+carMakeName->text().simplified().replace("\\", ".").replace("/", "."));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    this->close();
    emit selectRowSignal(dir.path());
    delete this;
}

CreateCarMakeWindow::~CreateCarMakeWindow()
{

}

void CreateCarMakeWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==16777220) {
        if(createButton->hasFocus()) {
            createButton->click();
        } else {
            createButton->setFocus();
        }
    }
}

