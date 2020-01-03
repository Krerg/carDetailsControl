#include "articlecounterthread.h"

#include <QDirIterator>

ArticleCounterThread::ArticleCounterThread(QString globalPath)
{
    this->globalPath = globalPath;
    this->articleCount = 0;
}

ArticleCounterThread::~ArticleCounterThread()
{

}

void ArticleCounterThread::run()
{
    QDirIterator i(globalPath, QStringList() << "*.txt",
                    QDir::Files, QDirIterator::Subdirectories);

     while (i.hasNext()) {
        articleCount++;
        i.next();
     }
     emit sendCount(articleCount);
}

