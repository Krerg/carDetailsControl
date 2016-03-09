#include "deletedetailwindow.h"
#include <QVBoxLayout>
#include <QDir>

DeleteDetailWindow::DeleteDetailWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* v = new QVBoxLayout(this);
    msg = new QLabel("Вы уверены, что хотите сделать это?");
    confirmButton = new QPushButton("Удалить");
    v->addWidget(msg);
    v->addWidget(confirmButton);
    pb = new QProgressBar();
    v->addWidget(pb);
    pb->setVisible(false);
    pb->setMaximum(100);
    pb->setValue(0);
    pb->setMinimum(0);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(deleteDetail()));
}

void DeleteDetailWindow::deleteDetail()
{
    pb->setVisible(true);
    QDir dir(path);
    QDir temp1;
    QDir temp2; //detail category
    QDir temp3; //detail
    int count = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size();
    int currentCount=0;
    emit deleted();
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        ++currentCount;
        pb->setValue((int)(((double)currentCount/count)*100));
        pb->update();
        temp1.setPath(path+dirName);
        foreach (QString dirName2, temp1.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            temp2.setPath(temp1.path()+"/"+dirName2);
            temp3.setPath(temp2.path()+"/"+detailCategory+"/"+detailName);
            temp3.removeRecursively();
        }
    }
    this->map->value(detailCategory)->removeAll(detailName);
    this->close();
    delete this;
}

void DeleteDetailWindow::setParameters(QString detailName, QString path, QString detailCategory, QMap<QString, QStringList *> *map)
{
    this->path = path;
    this->detailName = detailName;
    this->detailCategory = detailCategory;
    this->map = map;
}
