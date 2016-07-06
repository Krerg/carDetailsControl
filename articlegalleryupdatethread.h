#ifndef ARTICLEGALLERYUPDATETHREAD_H
#define ARTICLEGALLERYUPDATETHREAD_H

#include <QListWidgetItem>
#include <QMutex>
#include <QThread>



class ArticleGalleryUpdateThread : public QThread
{
    Q_OBJECT
public:
    ArticleGalleryUpdateThread(QString galleryPath);
    ~ArticleGalleryUpdateThread();

public:
    void run();
signals:
    void sendQListWidgetItem(QListWidgetItem*);
private:
     QString galleryPath;
     //QString tmpPath;
     //QStringList images;
};

#endif // ARTICLEGALLERYUPDATETHREAD_H
