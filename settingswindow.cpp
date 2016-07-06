#include "settingswindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDebug>
SettingsWindow::SettingsWindow(QString globalPath,QString galleryPath, QString pathToFiles,QString pathToSiteFiles, int imageSize, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    QHBoxLayout *h1 = new QHBoxLayout();
    QHBoxLayout *h2 = new QHBoxLayout();
    QHBoxLayout *h3 = new QHBoxLayout();
    QHBoxLayout *h4 = new QHBoxLayout();
    this->globalPath = globalPath;
    this->galleryPath = galleryPath;
    this->setAttribute( Qt::WA_DeleteOnClose );
    globalPathEdit = new QLineEdit(this->globalPath);
    imageGalleryEdit = new QLineEdit(this->galleryPath);
    globalPathEditLabel = new QLabel("Путь к структуре:");
    imageGalleryEditLabel = new QLabel("Путь к фотографиям:");
    this->pathToFiles = new QLineEdit(pathToFiles);
    pathToFilesLabel = new QLabel("Путь к выгрузке");
    pathToSiteFilesLabel = new QLabel("Путь на сайте до импортируемых фотографий");
    pathToSiteFilesEdit = new QLineEdit(pathToSiteFiles);
    this->confirmButton = new QPushButton("Подтвердить");

    selectDirectoryBtn1 = new QPushButton("...");
    selectDirectoryBtn1->setMaximumWidth(30);
    selectDirectoryBtn2 = new QPushButton("...");
    selectDirectoryBtn2->setMaximumWidth(30);
    selectDirectoryBtn3 = new QPushButton("...");
    selectDirectoryBtn3->setMaximumWidth(30);
    selectDirectoryBtn4 = new QPushButton("...");
    selectDirectoryBtn4->setMaximumWidth(30);

    error1 = new QLabel("Директории с таким именем не существует");
    error1->setStyleSheet("QLabel {color : red; }");
    error1->setHidden(true);
    error2 = new QLabel("Директории с таким именем не существует");
    error2->setStyleSheet("QLabel {color : red; }");
    error2->setHidden(true);
    error3 = new QLabel("Директории с таким именем не существует");
    error3->setStyleSheet("QLabel {color : red; }");
    error3->setHidden(true);

    imageSizeLabel = new QLabel("Размер изображений", this);

    imageSizeSpinBox = new QSpinBox(this);
    imageSizeSpinBox->setMaximum(300);
    imageSizeSpinBox->setValue(imageSize);

    resetCache = new QPushButton("Cбросить кэш");

    v->addWidget(globalPathEditLabel);
    v->addWidget(error1);
    h1->addWidget(globalPathEdit);
    h1->addWidget(selectDirectoryBtn1);
    h1->setStretch(0,10);
    v->addLayout(h1);
    v->addWidget(imageGalleryEditLabel);
    v->addWidget(error2);
    h2->addWidget(imageGalleryEdit);
    h2->addWidget(selectDirectoryBtn2);
    h2->setStretch(0,10);
    v->addLayout(h2);
    v->addWidget(pathToFilesLabel);
    v->addWidget(error3);
    h3->addWidget(this->pathToFiles);
    h3->addWidget(selectDirectoryBtn3);
    h3->setStretch(0,10);
    v->addLayout(h3);
    v->addWidget(pathToSiteFilesLabel);
    h4->addWidget(pathToSiteFilesEdit);
    h4->addWidget(selectDirectoryBtn4);
    h4->setStretch(0,10);
    v->addLayout(h4);
    v->addWidget(imageSizeLabel);
    v->addWidget(imageSizeSpinBox);
    v->addWidget(resetCache);
    v->addWidget(confirmButton);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(confirm()));
    QObject::connect(selectDirectoryBtn1,SIGNAL(clicked()),
                     this,SLOT(selectDirectoryBtnClickedSlot()));
    QObject::connect(selectDirectoryBtn2,SIGNAL(clicked()),
                     this,SLOT(selectDirectoryBtnClickedSlot()));
    QObject::connect(selectDirectoryBtn3,SIGNAL(clicked()),
                     this,SLOT(selectDirectoryBtnClickedSlot()));
    QObject::connect(selectDirectoryBtn4,SIGNAL(clicked()),
                     this,SLOT(selectDirectoryBtnClickedSlot()));
    QObject::connect(resetCache,SIGNAL(clicked()),
                     this,SLOT(resetCacheSlot()));

}

SettingsWindow::~SettingsWindow()
{

}

QString SettingsWindow::setCanonicalPath(QString path)
{
     path[0] = path[0].toUpper();
     return path;
}

void SettingsWindow::confirm()
{
    QDir globalDir(setCanonicalPath(globalPathEdit->text()));
    QDir imageGallerDir(setCanonicalPath(imageGalleryEdit->text()));
    QDir pathToFilesDir(setCanonicalPath(pathToFiles->text()));
    QString pathToFilesStr = (pathToFiles->text().isEmpty()) ? "" : pathToFilesDir.absolutePath();
    QString pathToSiteFilesStr = pathToSiteFilesEdit->text();
    if(!pathToSiteFilesStr.isEmpty()) {
        QChar lastChar = pathToSiteFilesStr.at(pathToSiteFilesStr.length()-1);
        if(lastChar == '/' || lastChar == '\\')
            pathToSiteFilesStr.remove(pathToSiteFilesStr.length()-1,1);
    }
    if(globalDir.exists() && imageGallerDir.exists() &&
            (pathToFiles->text().isEmpty() || pathToFilesDir.exists())) {

            emit setSettings(globalDir.absolutePath(),imageGallerDir.absolutePath(),
                            pathToFilesStr,pathToSiteFilesStr,
                             imageSizeSpinBox->value());
            this->hide();
            delete this;

    } else {
        if(!globalDir.exists())
            error1->setHidden(false);
        else
            error1->setHidden(true);
        if(!imageGallerDir.exists())
            error2->setHidden(false);
        else
            error2->setHidden(true);
        if(!pathToFilesDir.exists() && !pathToFilesStr.isEmpty())
            error3->setHidden(false);
        else
            error3->setHidden(true);
    }

}

void SettingsWindow::selectDirectoryBtnClickedSlot()
{
    QString dir = QFileDialog::
            getExistingDirectory(this, tr("Open Directory"),
                                 "/home",
                                  QFileDialog::ShowDirsOnly |
                                  QFileDialog::DontResolveSymlinks);
    QObject* obj = sender();
    if( obj == selectDirectoryBtn1 ) {
        globalPathEdit->setText(dir);
    }
    if( obj == selectDirectoryBtn2 ) {
        imageGalleryEdit->setText(dir);
    }
    if( obj == selectDirectoryBtn3 ) {
        pathToFiles->setText(dir);
    }
    if( obj == selectDirectoryBtn4 ) {
        pathToSiteFilesEdit->setText(dir);
    }

}

void SettingsWindow::resetCacheSlot()
{
    QDir dir("tmp");
    dir.removeRecursively();

    QDir baseDir;
    baseDir.current().mkdir("tmp");
}
