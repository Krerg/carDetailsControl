#include "articlegalleryupdatethread.h"

#include <QDir>

ArticleGalleryUpdateThread::ArticleGalleryUpdateThread(QString galleryPath)
{
    this->galleryPath = galleryPath;
}

ArticleGalleryUpdateThread::~ArticleGalleryUpdateThread()
{

}

void ArticleGalleryUpdateThread::run()
{
    //this->ui->articleGallery->clear();
    //this->ui->articleGallery->setUpdatesEnabled(false);
    QDir dir(galleryPath);
    QStringList filters;
        filters << "*.jpg" << "*.jpeg" << "*.png" << "*.txt";
    dir.setNameFilters(filters);
    QStringList images = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    QList<QString>::iterator i;
    QIcon* j;
    for(i=images.begin();i!=images.end();i++){
        j = new QIcon(galleryPath+(*i));
        QListWidgetItem* g = new QListWidgetItem((j->pixmap(QSize(80,80))),(*i));
        //this->articleImages->append(g);
        g->setSizeHint(QSize(80,80));
        //this->ui->articleGallery->addItem(g);
        emit sendQListWidgetItem(g);
        delete j;
    }
    //this->ui->articleGallery->setUpdatesEnabled(true);
}

