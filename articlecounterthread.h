#ifndef ARTICLECOUNTERTHREAD_H
#define ARTICLECOUNTERTHREAD_H

#include <QString>
#include <QThread>



class ArticleCounterThread : public QThread
{
    Q_OBJECT

public:
    ArticleCounterThread(QString globalPath);
    ~ArticleCounterThread();
    void run();
signals:
    void sendCount(int);

private:
    QString globalPath;
    int articleCount;
};

#endif // ARTICLECOUNTERTHREAD_H
