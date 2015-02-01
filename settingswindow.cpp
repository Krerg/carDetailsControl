#include "settingswindow.h"
#include <QVBoxLayout>

SettingsWindow::SettingsWindow(QString globalPath,QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    this->globalPath = globalPath;
    globalPathEdit = new QLineEdit(this->globalPath);
    this->confirmButton = new QPushButton("Подтвердить");
    v->addWidget(globalPathEdit);
    v->addWidget(confirmButton);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(confirm()));
}

void SettingsWindow::confirm()
{
    this->hide();
    delete this;
}
