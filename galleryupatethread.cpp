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
    for(i=images.begin();i!=images.end();i++)
    {
        if(!tmpDir.exists((*i))) {
            QPixmap* pixmap = new QPixmap(galleryPath+"/"+(*i));
            pixmap->scaledToWidth(300).save(tmpPath+(*i));
            delete pixmap;
        }
        j = new QIcon(tmpPath+(*i));
        QListWidgetItem* g = new QListWidgetItem((j->pixmap(QSize(300,300))),(*i));
        emit sendQListWidgetItem(g);
        delete j;

    }
}

int GalleryUpateThread::getNumberOfImages() const
{
    return images.size();
}

