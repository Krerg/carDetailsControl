#include "deleteimagewindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QListWidgetItem>

DeleteImageWindow::DeleteImageWindow(QWidget *parent) :
    QWidget(parent)
{
    this->pb = new QProgressBar();
    QVBoxLayout* v = new QVBoxLayout(this);
    this->msg = new QLabel("Вы точно хотите это сделать?");
    this->confirmButton = new QPushButton("Удалить");
    v->addWidget(msg);
    v->addWidget(confirmButton);
    v->addWidget(pb);

    pb->setVisible(false);
    pb->setMaximum(100);
    pb->setValue(0);
    pb->setMinimum(0);

    QObject::connect(this->confirmButton,SIGNAL(clicked()),this,SLOT(deleteImage()));
}

void DeleteImageWindow::deleteImage()
{
    emit updateGallery();
    delete this;
}

void DeleteImageWindow::setPath(QString path)
{
    this->path = path;
}
