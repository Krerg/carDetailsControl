#include "galleryupatethread.h"
#include <updatingwindow.h>
#include <QDir>
#include <QListWidgetItem>

GalleryUpateThread::GalleryUpateThread(QList<QListWidgetItem *> *galleryItems, QString galleryPath, QString tmpPath)
{
    this->galleryItems = galleryItems;
    this->galleryPath = galleryPath;
    this->tmpPath = tmpPath;
    QDir dir(galleryPath);
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png";
    subDirs = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    dir.setNameFilters(filters);
    images = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);

}

GalleryUpateThread::~GalleryUpateThread()
{

}

void GalleryUpateThread::run()
{
    QList<QString>::iterator i;
    QIcon* j;
    QDir tmpDir(tmpPath);
    for (i=subDirs.begin();i!=subDirs.end();i++) {
        QListWidgetItem* g = new QListWidgetItem(QIcon(":resource/images/folder.png"),(*i),0,1000);
        //g->
        emit sendQListWidgetItem(g);
    }

    for(i=images.begin();i!=images.end();i++)
    {
        if(!tmpDir.exists((*i))) {
            QPixmap* pixmap = new QPixmap(galleryPath+"/"+(*i));
            pixmap->scaledToWidth(300).save(tmpPath+"/"+(*i));
            delete pixmap;
        }
        j = new QIcon(tmpPath+"/"+(*i));
        QListWidgetItem* g = new QListWidgetItem((j->pixmap(QSize(300,300))),(*i));
        emit sendQListWidgetItem(g);
        delete j;
    }
}

int GalleryUpateThread::getNumberOfImages() const
{
    return images.size()+subDirs.size();
}

