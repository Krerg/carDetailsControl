#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileSystemModel>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include "createcarmakewindow.h"
#include "createcarmodelwindow.h"
#include "createdetailcategorywindow.h"
#include "createdetailwindow.h"
#include "deletedetailcategorywindow.h"
#include "confirmwindow.h"
#include "settingswindow.h"
#include "3rdparty/QtXlsxWriter-master/src/xlsx/xlsxdocument.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    QXlsx::Document xlsx;
    xlsx.write("A1", "Hello Qt!");
    xlsx.saveAs("Test.xlsx");

    this->detailCategories = new QStringList();

    this->details = new QTreeView();


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
    this->globalPath = "c:/carShop";

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

    ui->carMake->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->carMake,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestCarMake(QPoint)));

    ui->carModel->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->carModel,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestCarModel(QPoint)));

    ui->detailCategory->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->detailCategory,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequsetDetailCategory(QPoint)));

    ui->detail->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->detail,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestDetail(QPoint)));

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
    QObject::connect(this->ui->detailCategory,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailCategoryChanged(QModelIndex)));

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
    detailPath = fileModelDetailCategory->fileInfo(t).absoluteFilePath();
    ui->detail->setModel(fileDetail);
    ui->detail->hideColumn(1);
    ui->detail->hideColumn(2);
    ui->detail->hideColumn(3);
    ui->detail->hideColumn(4);
    ui->detail->setRootIndex(fileDetail->setRootPath(detailPath));
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
    }
    requestDetailMenu->popup(ui->detail->viewport()->mapToGlobal(pos));
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
    CreateDetailWindow* w = new CreateDetailWindow();
    w->show();
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
    SettingsWindow* w = new SettingsWindow(globalPath);
    w->show();
}

void MainWindow::setGlobalPath(QString path)
{
    this->globalPath = path;
}
