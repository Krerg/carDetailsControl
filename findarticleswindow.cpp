﻿#include "findarticleswindow.h"

#include <QDirIterator>
#include <QVBoxLayout>
#include <QDebug>
#include <QKeyEvent>

FindArticlesWindow::FindArticlesWindow(QString path, QWidget *parent)
    : QWidget(parent)
{
    this->findLineEdit = new QLineEdit();
    this->findPushButton = new QPushButton("Поиск");
    this->findPushButton->setMaximumWidth(50);
    this->path = path;
    this->filesWidget = new QListWidget();
    this->filesWidget->setVisible(false);
    QVBoxLayout *v = new QVBoxLayout(this);
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->addWidget(findLineEdit);
    h1->addWidget(findPushButton);
    v->addLayout(h1);
    v->addWidget(filesWidget);

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Найти Артикулы"));
    this->setMinimumWidth(300);

    QObject::connect(findPushButton,SIGNAL(clicked()),this,SLOT(findArticles()));
    QObject::connect(filesWidget,SIGNAL(itemActivated(QListWidgetItem*)),
                     this,SLOT(fileItemActivated(QListWidgetItem*)));
}

FindArticlesWindow::~FindArticlesWindow()
{

}

void FindArticlesWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==16777220) {
        if(findPushButton->hasFocus()) {
            findPushButton->click();
        } else {
            findPushButton->setFocus();
        }
    }
}

void FindArticlesWindow::showFiles()
{
    filesWidget->clear();
    if(foundFiles.size() > 0) {
        filesWidget->insertItems(0, foundFiles);
        this->filesWidget->setVisible(true);
    }
}

void FindArticlesWindow::findArticles()
{

    if(findLineEdit->text() != "") {
        containingWords = findLineEdit->text().split(" ");
        QStringListIterator i(containingWords);
        QString fileName;
        QDirIterator it(path, QStringList() << "*.txt",
                        QDir::Files, QDirIterator::Subdirectories);

        //QDir currentDir(path);
        QString fileText;
        foundFiles.clear();
        bool findFlag = true;
        while (it.hasNext()) {
            findFlag = true;
            fileName = it.next();
            QFile file(fileName);
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                break;
            QTextStream stream(&file);
            fileText = stream.readAll();
            i.toFront();

            while (i.hasNext()) {
                if(!fileText.contains(i.next(), Qt::CaseInsensitive))
                    findFlag = false;
            }
            file.close();
            if(findFlag) {
                fileName.remove(path);
                fileName = "..."+fileName;
                foundFiles << fileName;
            }
            showFiles();
        }
    }
}

void FindArticlesWindow::fileItemActivated(QListWidgetItem *item)
{
    QString str = item->text();
    emit articleActivated(&str);
}

