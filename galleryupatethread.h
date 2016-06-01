#ifndef GALLARYUPATETHREAD_H
#define GALLARYUPATETHREAD_H
#include <QListWidgetItem>
#include <QThread>

class GalleryUpateThread : public QThread
{
    Q_OBJECT
public:
    GalleryUpateThread(QList<QListWidgetItem*>* galleryItems, QString galleryPath,
                       QString tmpPath);
    ~GalleryUpateThread();
    void run();
    int getNumberOfImages() const;
signals:
    void sendQListWidgetItem(QListWidgetItem*);
private:
     QList<QListWidgetItem*>* galleryItems;
     QString galleryPath;
     QString tmpPath;
     QStringList images;
     QStringList subDirs;
};

#endif // GALLARYUPATETHREAD_H
