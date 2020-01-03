#include "articlegalleryupdatethread.h"

#include <QDir>
#include <QPainter>

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
        if(!(*i).contains("[H]")){
            j = new QIcon(galleryPath+(*i));
            QListWidgetItem* g = new QListWidgetItem((j->pixmap(QSize(80,80))),(*i));
            g->setSizeHint(QSize(80,80));
            emit sendQListWidgetItem(g);
            delete j;
        }
    }
    for(i=images.begin();i!=images.end();i++){
        if((*i).contains("[H]")){
            j = new QIcon(galleryPath+(*i));
            QPixmap transparent(QSize(80,60));
            transparent.fill(Qt::transparent);
            QPainter p;
            p.begin(&transparent);
            p.setCompositionMode(QPainter::CompositionMode_Source);
            p.drawPixmap(0, 0, j->pixmap(QSize(80,80)));
            p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
            p.fillRect(transparent.rect(), QColor(0, 0, 0, 80));
            p.end();
            QListWidgetItem* g = new QListWidgetItem(transparent,(*i));
            g->setSizeHint(QSize(80,80));
            emit sendQListWidgetItem(g);
            delete j;
        }
    }
}

