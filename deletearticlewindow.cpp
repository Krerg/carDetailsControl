#include "deletearticlewindow.h"
#include <QVBoxLayout>
#include <QDir>

DeleteArticleWindow::DeleteArticleWindow(QWidget *parent) :
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

    QObject::connect(this->confirmButton,SIGNAL(clicked()),this,SLOT(deleteArticle()));
}

void DeleteArticleWindow::deleteArticle()
{
    QDir dir(path);
    dir.removeRecursively();
    this->close();
    delete this;
}

void DeleteArticleWindow::setPath(QString path)
{
    this->path = path;
}
