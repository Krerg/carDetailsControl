#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileSystemModel>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include "createcarmakewindow.h"
#include "createcarmodelwindow.h"
#include "createdetailcategorywindow.h"
#include "createarticlewindow.h"
#include "deletedetailcategorywindow.h"
#include "deletearticlewindow.h"
#include "deletedetailwindow.h"
#include "confirmwindow.h"
#include "settingswindow.h"
#include "createdetailwindow.h"
#include "3rdparty/QtXlsxWriter-master/src/xlsx/xlsxdocument.h"
#include <QListWidget>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    this->selectedDetailCategory = "";

    this->editDetail = false;

    this->detailCategories = new QStringList();

    this->detailsMap = new QMap<QString, QStringList*>();

    this->details = new QTreeView();

    this->images = new QList<QListWidgetItem*>();

    ui->setupUi(this);
    menuBar = new QMenuBar(this);
    this->service = new QMenu("Сервис");
    this->settings = new QAction("Настройки",service);
    this->importToExcel = new QAction("Импорт в Excel",service);
    this->menuBar->addMenu(service);
    this->service->addAction(settings);
    QObject::connect(settings,SIGNAL(triggered()),this,SLOT(openSettingsWindow()));
    this->service->addAction(importToExcel);
    this->menuBar->setNativeMenuBar(false);
    this->menuBar->setMaximumHeight(23);
    this->menuBar->setMaximumWidth(58);

    menuBar->setContextMenuPolicy(Qt::CustomContextMenu);

    fileModelCarMake = new QFileSystemModel(this);
    fileModelCarModel = new QFileSystemModel(this);
    fileModelDetailCategory = new QFileSystemModel(this);
    fileDetail = new QFileSystemModel(this);
    fileDetailArticle = new QFileSystemModel(this);
    this->updateGallery();

    this->requestCarMakeMenu = new QMenu(this);
    this->createCarMake = new QAction("Добавить марку", requestCarMakeMenu);
    QObject::connect(createCarMake,SIGNAL(triggered()),this,SLOT(createCarMakeSlot()));
    this->deleteCarMake = new QAction("Удалить марку машины",requestCarMakeMenu);
    QObject::connect(deleteCarMake,SIGNAL(triggered()),this,SLOT(deleteCarMakeSlot()));
    this->requestCarMakeMenu->addAction(createCarMake);
    this->requestCarMakeMenu->addAction(deleteCarMake);

    this->requestCarModelMenu = new QMenu(this);
    this->createCarModel = new QAction("Добавить модель", requestCarModelMenu);
    QObject::connect(createCarModel,SIGNAL(triggered()),this,SLOT(createCarModelSlot()));
    this->deleteCarModel = new QAction("Удалить модель",requestCarModelMenu);
    QObject::connect(deleteCarModel,SIGNAL(triggered()),this,SLOT(deleteCarModelSlot()));
    this->requestCarModelMenu->addAction(createCarModel);
    this->requestCarModelMenu->addAction(deleteCarModel);

    this->requestDetailCategoryMenu = new QMenu(this);
    this->createDetailCategory = new QAction("Добавить категорию", requestDetailCategoryMenu);
    QObject::connect(createDetailCategory,SIGNAL(triggered()),this,SLOT(createDetailCategorySlot()));
    this->deleteDetailCategory = new QAction("Удалить категорию",requestDetailCategoryMenu);
    QObject::connect(deleteDetailCategory,SIGNAL(triggered()),this,SLOT(deleteDetailCategorySlot()));
    this->requestDetailCategoryMenu->addAction(createDetailCategory);
    this->requestDetailCategoryMenu->addAction(deleteDetailCategory);

    this->requestDetailMenu = new QMenu(this);
    this->createDetail = new QAction("Добавить делать",requestDetailMenu);
    QObject::connect(createDetail,SIGNAL(triggered()),this,SLOT(createDetailSlot()));
    this->deleteDetail = new QAction("Удалить деталь",requestDetailMenu);
    QObject::connect(deleteDetail,SIGNAL(triggered()),this,SLOT(deleteDetailSlot()));
    this->requestDetailMenu->addAction(createDetail);
    this->requestDetailMenu->addAction(deleteDetail);

    this->requestDetailArticleMenu = new QMenu(this);
    //this->createArticle = new QAction("Добавить артикул", requestDetailArticleMenu);
    //QObject::connect(createArticle,SIGNAL(triggered()),this,SLOT(createArticleSlot()));
    this->deleteArticle = new QAction("Удалить артикул", requestDetailArticleMenu);
    //this->requestDetailArticleMenu->addAction(createArticle);
    this->requestDetailArticleMenu->addAction(deleteArticle);
    QObject::connect(deleteArticle,SIGNAL(triggered()),this,SLOT(deleteArticleSlot()));

    ui->carMake->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->carMake,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestCarMake(QPoint)));

    ui->carModel->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->carModel,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestCarModel(QPoint)));

    ui->detailCategory->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->detailCategory,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequsetDetailCategory(QPoint)));

    ui->detail->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->detail,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestDetail(QPoint)));

    ui->detailArticle->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->detailArticle,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestDetailArticle(QPoint)));
    //кстановка фильтров на папки
    fileModelCarMake->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileModelCarModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileModelDetailCategory->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileDetail->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    //открытие фотки по двойному щелчку
    QObject::connect(ui->gallery,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(openImage(QModelIndex)));

    //установка корневой папки
    fileModelCarMake->setRootPath(globalPath);
    ui->carMake->setModel(fileModelCarMake);
    ui->carMake->setRootIndex(fileModelCarMake->index(globalPath));

    QObject::connect(this->ui->carMake,SIGNAL(clicked(QModelIndex)),this,SLOT(carMakeChanged(QModelIndex)));
    QObject::connect(this->ui->carModel,SIGNAL(clicked(QModelIndex)),this,SLOT(carModelChanged(QModelIndex)));
    QObject::connect(this->ui->detailCategory,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailCategoryChanged(QModelIndex)));
    QObject::connect(this->ui->detail,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailChanged(QModelIndex)));
    QObject::connect(this->ui->detailArticle,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailArticleChanged(QModelIndex)));

    //apply settings
    this->settingsFile = new QFile("settings.txt");
    if(settingsFile->exists()) {
        settingsFile->open(QIODevice::ReadWrite);
        QTextStream in(settingsFile);
        QString globalP = in.readLine();
        QString galleryP = in.readLine();

        this->setSettings(globalP,galleryP);
    } else {
        this->globalPath = "c:/carShop/";
        this->galleryPath = "c:/gallery/";
        settingsFile->open(QIODevice::ReadWrite | QFile::Append | QFile::Text);
        QTextStream out(settingsFile);
        out << globalPath << endl;
        out << galleryPath << endl;
    }

    this->getDetailCategoriesList();
}

void MainWindow::carMakeChanged(QModelIndex t)
{
    this->sPath = fileModelCarMake->fileInfo(t).absoluteFilePath();
    ui->carModel->setModel(fileModelCarModel);
    this->ui->carModel->setRootIndex(fileModelCarModel->setRootPath(sPath));
}

void MainWindow::carModelChanged(QModelIndex t)
{
    this->modelPath = fileModelCarModel->fileInfo(t).absoluteFilePath();
    ui->detailCategory->setModel(fileModelDetailCategory);
    this->ui->detailCategory->setRootIndex(fileModelDetailCategory->setRootPath(modelPath));
}

void MainWindow::carDetailCategoryChanged(QModelIndex t)
{
    detailCategoryPath = fileModelDetailCategory->fileInfo(t).absoluteFilePath();
    this->selectedDetailCategory = fileModelDetailCategory->fileInfo(t).fileName();
    ui->detail->setModel(fileDetail);
    ui->detail->setRootIndex(fileDetail->setRootPath(detailCategoryPath));
}

void MainWindow::carDetailChanged(QModelIndex t)
{
    detailPath = fileDetail->fileInfo(t).absoluteFilePath();
    ui->detailArticle->setModel(fileDetailArticle);
    ui->detailArticle->setRootIndex(fileDetailArticle->setRootPath(detailPath));
}

void MainWindow::openImage(QModelIndex t)
{
    QString path = t.data().toString();
    QProcess process;
    QString fp = galleryPath+"/"+path;
    process.start("C:/windows/system32/cmd.exe", QStringList() << "/C" << galleryPath+"/"+path);
    process.waitForFinished(-1);
}

void MainWindow::carDetailArticleChanged(QModelIndex t)
{
    this->clearOutput();

    detailArticlePath = fileDetailArticle->fileInfo(t).absoluteFilePath();
    QFile describe(detailArticlePath+"/Описание.txt");
    if (describe.open(QIODevice::ReadOnly))
    {
        QTextStream in(&describe);
        QString temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->makeOutput->setText(temp);

        temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->modelOutput->setText(temp);

        temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->categoryOutput->setText(temp);

        temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->detailOutput->setText(temp);

        temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->articleOutput->setText(temp);

        temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->costOutput->setText(temp);

        temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->originaArtcileOutput->setText(temp);

        temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->placeOutput->setText(temp);

        temp = in.readLine();
        if(temp=="")
        {
            return;
        }
        this->ui->noteOutput->setText(temp);
    }
    qDebug()<<detailArticlePath;
}

void MainWindow::menuRequestCarMake(QPoint p)
{
    requestCarMakeMenu->removeAction(deleteCarMake);
    if(ui->carMake->indexAt(p).row()>=0)
    requestCarMakeMenu->addAction(deleteCarMake);
    requestCarMakeMenu->popup(ui->carMake->viewport()->mapToGlobal(p));
}

void MainWindow::menuRequestCarModel(QPoint pos)
{
    requestCarModelMenu->removeAction(deleteCarModel);
    if(ui->carModel->indexAt(pos).row()>=0)
    requestCarModelMenu->addAction(deleteCarModel);
    requestCarModelMenu->popup(ui->carModel->viewport()->mapToGlobal(pos));
}

void MainWindow::menuRequsetDetailCategory(QPoint pos)
{
    requestDetailCategoryMenu->removeAction(deleteDetailCategory);
    if(ui->detailCategory->indexAt(pos).row()>=0) {
    requestDetailCategoryMenu->addAction(deleteDetailCategory);
    }
    requestDetailCategoryMenu->popup(ui->detailCategory->viewport()->mapToGlobal(pos));
}

void MainWindow::menuRequestDetail(QPoint pos)
{
    requestDetailMenu->removeAction(deleteDetail);
    QString fp;
    if(ui->detail->indexAt(pos).row()>=0)
    {
    requestDetailMenu->addAction(deleteDetail);
    QModelIndex qmodel = ui->detail->selectionModel()->selectedIndexes().at(0);
    fp = fileDetail->filePath(qmodel);
    QDir dir(fp);

    }
    requestDetailMenu->popup(ui->detail->viewport()->mapToGlobal(pos));
}

void MainWindow::menuRequestDetailArticle(QPoint pos)
{
    requestDetailArticleMenu->removeAction(deleteArticle);
    if(ui->detailArticle->indexAt(pos).row()>=0)
    {
        requestDetailArticleMenu->addAction(deleteArticle);
    }
    requestDetailArticleMenu->popup(ui->detailArticle->viewport()->mapToGlobal(pos));
}

void MainWindow::createCarMakeSlot()
{
    CreateCarMakeWindow* m = new CreateCarMakeWindow();
    m->setPath(globalPath);
    m->show();
}

void MainWindow::createCarModelSlot()
{
    CreateCarModelWindow* m = new CreateCarModelWindow();
    m->setPath(sPath);
    m->setCategoriesList(this->detailsMap);
    m->show();
}

void MainWindow::createDetailCategorySlot()
{
    CreateDetailCategoryWindow* c = new CreateDetailCategoryWindow();
    c->setParameters(globalPath,detailsMap);
    c->show();
}

void MainWindow::createDetailSlot()
{
    CreateDetailWindow* w = new CreateDetailWindow();
    w->setParameters(globalPath,detailsMap,selectedDetailCategory);
    w->show();
}

void MainWindow::createArticleSlot()
{
    CreateArticleWindow* w = new CreateArticleWindow();
    w->setPath(detailPath);
    w->show();
}

void MainWindow::deleteArticleSlot()
{
    DeleteArticleWindow *d = new DeleteArticleWindow();
    d->setPath(this->fileDetailArticle->filePath(this->ui->detailArticle->currentIndex()));
    d->show();
}

void MainWindow::deleteCarModelSlot()
{
    ConfirmWindow* c = new ConfirmWindow();
    c->setPath(modelPath);
    c->show();
}

void MainWindow::deleteDetailCategorySlot()
{
    QString catName = this->fileModelDetailCategory->fileInfo(this->ui->detailCategory->currentIndex()).fileName();
    DeleteDetailCategoryWindow *w = new DeleteDetailCategoryWindow();
    w->setParameters(globalPath,detailsMap,catName);
    w->show();
}

void MainWindow::deleteCarMakeSlot()
{
    ConfirmWindow* c = new ConfirmWindow();
    c->setPath(sPath);
    c->show();
}

void MainWindow::deleteDetailSlot()
{
    QString catName = this->fileModelDetailCategory->fileInfo(this->ui->detailCategory->currentIndex()).fileName();
    QString detName = this->fileDetail->fileName(this->ui->detail->currentIndex());
    DeleteDetailWindow* w = new DeleteDetailWindow();
    w->setParameters(detName,globalPath,catName,detailsMap);
    w->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDetailCategoriesList()
{
    detailsMap->clear();
    QDir *dir = new QDir(globalPath);
    if(dir->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).size() == 0)
    {
        return;
    }
    QString tempPath = dir->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).first().fileName();
    QString fullPath = globalPath+"/"+tempPath;
    QDir *dir2 = new QDir(fullPath);
    if(dir2->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).size() == 0)
    {
        return;
    }
    tempPath = dir2->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).first().fileName();
    fullPath += "/";
    fullPath += tempPath;

    QDir *dir3 = new QDir(fullPath);
    if(dir3->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).size() == 0)
    {
        return;
    }
    QList<QFileInfo> gh = dir3->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    tempPath = dir3->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).first().fileName();

    QList<QFileInfo>::Iterator i;
    QList<QFileInfo>::Iterator j;

    for(i=gh.begin();i!=gh.end();i++)
    {
        QDir *dir4 = new QDir(fullPath+"/"+(*i).fileName());
        QList<QFileInfo> infoList = dir4->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
        this->detailCategories->append((*i).fileName());
        this->detailsMap->insert((*i).fileName(), new QStringList());

        for(j=infoList.begin();j!=infoList.end();j++)
        {
            this->detailsMap->value((*i).fileName())->append((*j).fileName());
        }
        delete dir4;
    }

    delete dir;
    delete dir2;
    delete dir3;
}

void MainWindow::openSettingsWindow()
{
    SettingsWindow* w = new SettingsWindow(globalPath,galleryPath);
    QObject::connect(w,SIGNAL(setSettings(QString,QString)),this,SLOT(setSettings(QString,QString)));
    w->show();
}

void MainWindow::setSettings(QString path, QString galleryPath)
{
    this->globalPath = path;
    this->galleryPath = galleryPath;
    this->settingsFile->remove();
    settingsFile->open(QIODevice::ReadWrite | QFile::Append | QFile::Text);
    QTextStream out(settingsFile);
    out << globalPath << endl;
    out << galleryPath << endl;
    this->updateAll();
}

void MainWindow::updateGallery()
{
    this->ui->gallery->clear();
    QDir dir(galleryPath);
    QStringList images = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    QList<QString>::iterator i;
    for(i=images.begin();i!=images.end();i++)
    {
        QIcon j(galleryPath+"/"+(*i));

        QListWidgetItem* g = new QListWidgetItem(j,(*i));
        this->images->append(g);

       g->setSizeHint(QSize(80,80));
        this->ui->gallery->addItem(g);
    }
}

void MainWindow::updateAll()
{
    fileModelCarMake->setRootPath(globalPath);
    ui->carMake->setModel(fileModelCarMake);
    ui->carMake->setRootIndex(fileModelCarMake->index(globalPath));

    this->updateGallery();

    delete fileModelCarModel;
    delete fileModelDetailCategory;
    delete fileDetail;
    delete fileDetailArticle;

    fileModelCarModel = new QFileSystemModel(this);
    fileModelDetailCategory = new QFileSystemModel(this);
    fileDetail = new QFileSystemModel(this);
    fileDetailArticle = new QFileSystemModel(this);
    this->getDetailCategoriesList();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    if(event->key()==16777220)
    {
        if(this->ui->detailCategory->currentIndex().isValid()) {
            if(this->editDetail == false) {
                this->ui->articleOutput->setFocus();
                this->ui->articleOutput->setSelection(0,50);
                this->editDetail = true;
                QList<QModelIndex> list = this->ui->carMake->selectionModel()->selectedIndexes();
                this->ui->makeOutput->setText(this->ui->carMake->selectionModel()->selectedIndexes().at(0).data().toString());
                this->ui->modelOutput->setText(this->ui->carModel->selectionModel()->selectedIndexes().at(0).data().toString());
                this->ui->categoryOutput->setText(this->ui->detailCategory->selectionModel()->selectedIndexes().at(0).data().toString());
                this->ui->detailOutput->setText(this->ui->detail->selectionModel()->selectedIndexes().at(0).data().toString());
            } else {
                //создаем артикул
                if(this->ui->articleOutput->text()!="") {
                //qDebug()<<detailPath+"/"+this->ui->articleOutput->text()+"/Описание.txt";
                QDir dir(detailPath+"/"+this->ui->articleOutput->text());
                if(!dir.exists())
                {
                     dir.mkpath(".");
                }
                QFile file(detailPath+"/"+this->ui->articleOutput->text()+"/Описание.txt");
                if(file.open(QIODevice::ReadWrite | QFile::Append | QFile::Text))
                {
                     QTextStream stream(&file);
                     stream<<this->ui->makeOutput->text() <<endl;
                     stream<<this->ui->modelOutput->text() <<endl;
                     stream<<this->ui->categoryOutput->text() <<endl;
                     stream<<this->ui->detailOutput->text() <<endl;
                     stream<<this->ui->articleOutput->text() <<endl;
                     stream<<this->ui->costOutput->text() <<endl;
                     stream<<this->ui->originaArtcileOutput->text() <<endl;
                     stream<<this->ui->placeOutput->text() <<endl;
                     stream<<this->ui->noteOutput->text() <<endl;
                }
                QList<QListWidgetItem*> files = this->ui->gallery->selectedItems();
                QList<QListWidgetItem*>::Iterator i;
                for(i=files.begin();i!=files.end();i++)
                {
                    QFile f(galleryPath+"/"+(*i)->text());
                    QFileInfo *d = new QFileInfo(f);
                    QString h = detailPath+"/"+this->ui->articleOutput->text()+"/"+d->fileName();
                    bool b = QFile::copy(galleryPath+"/"+(*i)->text(),h);
                    QFile::remove(galleryPath+"/"+(*i)->text());
                    delete d;
                }
                }
                this->editDetail = false;
                this->updateGallery();
            }
        }
    }
}

void MainWindow::clearOutput()
{
    this->ui->makeOutput->setText("");
    this->ui->modelOutput->setText("");
    this->ui->categoryOutput->setText("");
    this->ui->detailOutput->setText("");
    this->ui->articleOutput->setText("");
    this->ui->costOutput->setText("");
    this->ui->originaArtcileOutput->setText("");
    this->ui->placeOutput->setText("");
    this->ui->noteOutput->setText("");
}
