#include "createdetailwindow.h"
#include <QDir>

CreateDetailWindow::CreateDetailWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* v = new QVBoxLayout(this);
    this->detailName = new QLineEdit();
    this->pb = new QProgressBar();
    this->confirmButton = new QPushButton("Создать");
    pb->setMaximum(100);
    pb->setMinimum(0);
    pb->setValue(0);
    v->addWidget(detailName);
    v->addWidget(confirmButton);
    v->addWidget(pb);
    pb->setVisible(false);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(createDetail()));
}

void CreateDetailWindow::setParameters(QString path, QMap<QString, QStringList *> *map, QString category)
{
    this->path = path;
    this->map = map;
    this->selectedCategory = category;
}

void CreateDetailWindow::createDetail()
{
    pb->setVisible(true);
    QDir dir(path);
    QDir temp1;
    QDir temp2; //detail category
    QDir temp3; //detail
    int count = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size();
    int currentCount=0;
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        ++currentCount;
        pb->setValue((int)(((double)currentCount/count)*100));
        pb->update();
        temp1.setPath(path+"/"+dirName);
        foreach (QString dirName2, temp1.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            temp2.setPath(temp1.path()+"/"+dirName2);
            temp3.setPath(temp2.path()+"/"+selectedCategory+"/"+detailName->text());
            if(!temp3.exists()) {
                temp3.mkpath(".");
                this->map->value(selectedCategory)->append(detailName->text());
            }
    }
}
    this->hide();
    delete this;
}
