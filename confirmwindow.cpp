#include "confirmwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>

ConfirmWindow::ConfirmWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    QHBoxLayout *h = new QHBoxLayout();
    confirmText = new QLabel("Вы точно хотите это сделать?", this);
    okButton = new QPushButton("OK",this);
    cancelButton = new QPushButton("Отмена",this);
    v->addWidget(confirmText);
    v->addStrut(10);
    v->addItem(h);
    h->addWidget(okButton);
    h->addWidget(cancelButton);
    this->setLayout(v);
    QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelAction()));
    QObject::connect(okButton,SIGNAL(clicked()),this,SLOT(confirmAction()));
}

void ConfirmWindow::cancelAction()
{
    this->close();
    delete this;
}

void ConfirmWindow::confirmAction()
{
    this->close();
    emit confirm();
    QDir dir(this->path);
    dir.removeRecursively();
    delete this;
}

void ConfirmWindow::setPath(QString path)
{
    this->path = path;
}
