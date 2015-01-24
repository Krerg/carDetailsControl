#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileSystemModel>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include "createcarmakewindow.h"
#include "createcarmodelwindow.h"
#include "createdetailcategorywindow.h"
#include "deletedetailcategorywindow.h"
#include "confirmwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{

    this->detailCategories = new QStringList();

    menuBar = new QMenuBar(this);
    ui->setupUi(this);

    this->service = new QMenu("Сервис", this);
    this->settings = new QAction("Настройки",this);
    this->importToExcel = new QAction("Импорт в Excel",this);
    this->service->addAction(settings);
    this->service->addAction(importToExcel);
    this->menuBar->addMenu(service);

    fileModelCarMake = new QFileSystemModel(this);
    fileModelCarModel = new QFileSystemModel(this);
    fileModelDetailCategory = new QFileSystemModel(this);
    this->globalPath = "c:/carShop";

    this->requestCarMakeMenu = new QMenu(this);
    this->createCarMake = new QAction("Добавить марку", requestCarMakeMenu);
    QObject::connect(createCarMake,SIGNAL(triggered()),this,SLOT(createCarMakeSlot()));
    this->createCarMakeWindow = new QWidget(this);
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

    ui->carMake->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->carMake,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestCarMake(QPoint)));

    ui->carModel->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->carModel,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestCarModel(QPoint)));

    ui->detailCategory->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->detailCategory,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequsetDetailCategory(QPoint)));

    //кстановка фильтров на папки
    fileModelCarMake->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileModelCarModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileModelDetailCategory->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);



    //установка корневой папки
    fileModelCarMake->setRootPath(globalPath);
    ui->carMake->setModel(fileModelCarMake);
    ui->carMake->setRootIndex(fileModelCarMake->index(globalPath));

    QObject::connect(this->ui->carMake,SIGNAL(clicked(QModelIndex)),this,SLOT(carMakeChanged(QModelIndex)));
    QObject::connect(this->ui->carModel,SIGNAL(clicked(QModelIndex)),this,SLOT(carModelChanged(QModelIndex)));

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

void MainWindow::menuRequestCarMake(QPoint p)
{
    requestCarMakeMenu->removeAction(deleteCarMake);
    if(ui->carMake->indexAt(p).row()>0)
    requestCarMakeMenu->addAction(deleteCarMake);
    requestCarMakeMenu->popup(ui->carMake->viewport()->mapToGlobal(p));
}

void MainWindow::menuRequestCarModel(QPoint pos)
{
    requestCarModelMenu->removeAction(deleteCarModel);
    if(ui->carModel->indexAt(pos).row()>0)
    requestCarModelMenu->addAction(deleteCarModel);
    requestCarModelMenu->popup(ui->carModel->viewport()->mapToGlobal(pos));
}

void MainWindow::menuRequsetDetailCategory(QPoint pos)
{
    requestDetailCategoryMenu->removeAction(deleteDetailCategory);
    if(ui->detailCategory->indexAt(pos).row()>0)
    requestDetailCategoryMenu->addAction(deleteDetailCategory);
    requestDetailCategoryMenu->popup(ui->detailCategory->viewport()->mapToGlobal(pos));
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
