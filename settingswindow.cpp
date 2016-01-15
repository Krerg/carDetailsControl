#include "settingswindow.h"
#include <QVBoxLayout>

SettingsWindow::SettingsWindow(QString globalPath,QString galleryPath, QString pathToFiles, int imageSize, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    this->globalPath = globalPath;
    this->galleryPath = galleryPath;
    this->setAttribute( Qt::WA_DeleteOnClose );
    globalPathEdit = new QLineEdit(this->globalPath);
    imageGalleryEdit = new QLineEdit(this->galleryPath);
    globalPathEditLabel = new QLabel("Путь к структуре:");
    imageGalleryEditLabel = new QLabel("Путь к фотографиям:");
    this->pathToFiles = new QLineEdit(pathToFiles);
    pathToFilesLabel = new QLabel("Путь к выгрузке");
    this->confirmButton = new QPushButton("Подтвердить");

    imageSizeLabel = new QLabel("Размер изображений", this);

    imageSizeSpinBox = new QSpinBox(this);
    imageSizeSpinBox->setValue(imageSize);
    imageSizeSpinBox->setMaximum(300);

    v->addWidget(globalPathEditLabel);
    v->addWidget(globalPathEdit);
    v->addWidget(imageGalleryEditLabel);
    v->addWidget(imageGalleryEdit);
    v->addWidget(pathToFilesLabel);
    v->addWidget(this->pathToFiles);
    v->addWidget(imageSizeLabel);
    v->addWidget(imageSizeSpinBox);
    v->addWidget(confirmButton);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(confirm()));
}

SettingsWindow::~SettingsWindow()
{
    QLayoutItem *child;
    while((child = this->layout()->takeAt(0))!=NULL) {
        child->widget()->deleteLater();
        delete child;
    }
}

void SettingsWindow::confirm()
{
    emit setSettings(globalPathEdit->text(),imageGalleryEdit->text(),pathToFiles->text(),imageSizeSpinBox->value());
    this->hide();
    delete this;
}
