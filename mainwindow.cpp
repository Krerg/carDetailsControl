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
#include "confirmwindow.h"
#include "settingswindow.h"
#include "3rdparty/QtXlsxWriter-master/src/xlsx/xlsxdocument.h"
#include <QListWidget>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{

    this->detailCategories = new QStringList();

    this->details = new QTreeView();

    this->images = new QList<QListWidgetItem*>();
   // QListWidget *g = new QListWidget(this);
   // g->setViewMode(QListWidget::IconMode);
    //this->ui->gallery->setViewMode(QListWidget::IconMode);
    //ui->gallery->setIconSize(QSize(50,50));
    //ui->gallery->setResizeMode(QListWidget::Adjust);
  //  QListWidgetItem *item = new QListWidgetItem(QIcon("C:/Users/sasha_000/Pictures/subway.jpg"),"Earth");
 //   item->setData( Qt::CheckStateRole, Qt::Checked );
  //  ui->gallery->addItem(new QListWidgetItem(QIcon("C:/Users/sasha_000/Pictures/subway.jpg"),"Earth"));
  //  ui->gallery->addItem(new QListWidgetItem(QIcon("C:/Users/sasha_000/Pictures/subway.jpg"),"Earth"));
  //  ui->gallery->addItem(new QListWidgetItem(QIcon("C:/Users/sasha_000/Pictures/subway.jpg"),"Earth"));


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
    menuBar->setContextMenuPolicy(Qt::CustomContextMenu);

    fileModelCarMake = new QFileSystemModel(this);
    fileModelCarModel = new QFileSystemModel(this);
    fileModelDetailCategory = new QFileSystemModel(this);
    fileDetail = new QFileSystemModel(this);
    fileDetailArticle = new QFileSystemModel(this);
    this->globalPath = "c:/carShop";
    this->galleryPath = "C:/Users/sasha_000/Pictures";
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
    this->requestDetailMenu->addAction(createDetail);
    this->requestDetailMenu->addAction(deleteDetail);

    this->requestDetailArticleMenu = new QMenu(this);
    this->createArticle = new QAction("Добавить артикул", requestDetailArticleMenu);
    QObject::connect(createArticle,SIGNAL(triggered()),this,SLOT(createArticleSlot()));
    this->deleteArticle = new QAction("Удалить артикул", requestDetailArticleMenu);
    this->requestDetailArticleMenu->addAction(createArticle);
    this->requestDetailArticleMenu->addAction(deleteArticle);

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

    QListWidget* j = ui->gallery;

    QObject::connect(this->ui->carMake,SIGNAL(clicked(QModelIndex)),this,SLOT(carMakeChanged(QModelIndex)));
    QObject::connect(this->ui->carModel,SIGNAL(clicked(QModelIndex)),this,SLOT(carModelChanged(QModelIndex)));
    QObject::connect(this->ui->detailCategory,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailCategoryChanged(QModelIndex)));
    QObject::connect(this->ui->detail,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailChanged(QModelIndex)));
    QObject::connect(this->ui->detailArticle,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailArticleChanged(QModelIndex)));

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
    detailArticlePath = fileDetailArticle->fileInfo(t).absoluteFilePath();
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
    m->setCategoriesList(this->detailCategories);
    m->show();
}

void MainWindow::createDetailCategorySlot()
{
    CreateDetailCategoryWindow* c = new CreateDetailCategoryWindow();
    c->setPath(globalPath);
    c->show();
}

void MainWindow::createDetailSlot()
{

}

void MainWindow::createArticleSlot()
{
    CreateArticleWindow* w = new CreateArticleWindow();
    w->setPath(detailPath);
    w->show();
}

void MainWindow::deleteArticleSlot()
{

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
    w->setCategoryName(catName);
    w->setPath(globalPath);
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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDetailCategoriesList()
{
    QDir *dir = new QDir(globalPath);
    QString tempPath = dir->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).first().fileName();
    QString fullPath = globalPath+"/"+tempPath;

    QDir *dir2 = new QDir(fullPath);
    tempPath = dir2->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs).first().fileName();
    fullPath += "/";
    fullPath += tempPath;

    QDir *dir3 = new QDir(fullPath);
    QList<QFileInfo> gh = dir3->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);

    QList<QFileInfo>::Iterator i;
    for(i=gh.begin();i!=gh.end();i++)
    {
        this->detailCategories->append((*i).fileName());
    }
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

    int y=0;
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


}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    event->text();
}
