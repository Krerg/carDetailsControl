#include "settingswindow.h"
#include <QVBoxLayout>

SettingsWindow::SettingsWindow(QString globalPath,QString galleryPath, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    this->globalPath = globalPath;
    this->galleryPath = galleryPath;
    globalPathEdit = new QLineEdit(this->globalPath);
    imageGalleryEdit = new QLineEdit(this->galleryPath);
    globalPathEditLabel = new QLabel("Путь к структуре:");
    imageGalleryEditLabel = new QLabel("Путь к фотографиям:");
    this->confirmButton = new QPushButton("Подтвердить");
    v->addWidget(globalPathEditLabel);
    v->addWidget(globalPathEdit);
    v->addWidget(imageGalleryEditLabel);
    v->addWidget(imageGalleryEdit);
    v->addWidget(confirmButton);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(confirm()));
}

void SettingsWindow::confirm()
{
    emit setSettings(globalPathEdit->text(),imageGalleryEdit->text());
    this->hide();
    delete this;
}
