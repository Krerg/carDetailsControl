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
#include "renamecarmakewindow.h"
#include "deletearticlewindow.h"
#include "deletedetailwindow.h"
#include "confirmwindow.h"
#include "renamedetailcategory.h"
#include "renamedetailwindow.h"
#include "changearticlewindow.h"
#include "settingswindow.h"
#include "createdetailwindow.h"
#include "3rdparty/QtXlsxWriter-master/src/xlsx/xlsxdocument.h"
#include "updatingwindow.h"
#include <QListWidget>
#include <QProcess>
#include <QMessageBox>
#include "articlecounterthread.h"
#include "articlegalleryupdatethread.h"
#include "excelhandler.h"
#include "findarticleswindow.h"
#include "galleryupatethread.h"
#include "imageviewer.h"
#include "importfromexcelwindow.h"
#include "movingariclesdialog.h"

const QString MainWindow::NEW_STATE = "NEW";

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->UpdateGalleryProgressBar->setVisible(false);
    this->setAttribute( Qt::WA_DeleteOnClose );
    //this->setAcceptDrops(true);
    //this->ui->gallery->setAcceptDrops(true);
    //this->ui->articleGallery->setAcceptDrops(true);
    this->ui->gallery->setIconSize(QSize(5,5));
    this->selectedDetailCategory = "";
    this->editDetail = false;
    this->detailCategories = new QStringList();
    this->detailsMap = new QMap<QString, QStringList*>();
    this->details = new QTreeView();
    this->images = new QList<QListWidgetItem*>();
    this->articleImages = new QList<QListWidgetItem*>();

    initWindowSize();

    isUpdateDetailGalleryFinished = true;
    isUpdateGalleryFinished = true;

    menuBar = new QMenuBar(this);
    this->service = new QMenu("Сервис");
    this->settings = new QAction("Настройки",service);
    this->importFromExcel = new QAction("Импорт из Excel",service);
    QObject::connect(importFromExcel,SIGNAL(triggered()),this,SLOT(importFromExcelSlot()));
    this->exportToExcel = new QAction("Экспорт в Excel", service);
    QObject::connect(this->exportToExcel,SIGNAL(triggered()),this,SLOT(exportToExcelSlot()));
    this->menuBar->addMenu(service);
    this->service->addAction(settings);
    QObject::connect(settings,SIGNAL(triggered()),this,SLOT(openSettingsWindow()));
    this->service->addAction(importFromExcel);
    this->service->addAction(exportToExcel);
    this->menuBar->setNativeMenuBar(false);
    this->menuBar->setMaximumHeight(23);
    this->menuBar->setMaximumWidth(58);
    menuBar->setContextMenuPolicy(Qt::CustomContextMenu);
    fileModelCarMake = new QFileSystemModel(this);
    fileModelCarModel = new QFileSystemModel(this);
    fileModelDetailCategory = new QFileSystemModel(this);
    fileDetail = new QFileSystemModel(this);
    fileDetailArticle = new QFileSystemModel(this);

    this->requestCarMakeMenu = new QMenu(this);
    this->createCarMake = new QAction("Добавить марку", requestCarMakeMenu);
    QObject::connect(createCarMake,SIGNAL(triggered()),this,SLOT(createCarMakeSlot()));
    this->renameCarMake = new QAction("Переименовать марку", requestCarMakeMenu);
    QObject::connect(renameCarMake, SIGNAL(triggered()),this,SLOT(renameCarMakeSlot()));
    this->deleteCarMake = new QAction("Удалить марку машины",requestCarMakeMenu);
    QObject::connect(deleteCarMake,SIGNAL(triggered()),this,SLOT(deleteCarMakeSlot()));
    this->requestCarMakeMenu->addAction(createCarMake);
    this->requestCarMakeMenu->addAction(renameCarMake);
    this->requestCarMakeMenu->addAction(deleteCarMake);

    this->requestCarModelMenu = new QMenu(this);
    this->createCarModel = new QAction("Добавить модель", requestCarModelMenu);
    QObject::connect(createCarModel,SIGNAL(triggered()),this,SLOT(createCarModelSlot()));
    this->renameCarModel = new QAction("Переименовать модель", requestCarModelMenu);
    QObject::connect(renameCarModel,SIGNAL(triggered()),this,SLOT(renameCarModelSlot()));
    this->deleteCarModel = new QAction("Удалить модель",requestCarModelMenu);
    QObject::connect(deleteCarModel,SIGNAL(triggered()),this,SLOT(deleteCarModelSlot()));
    this->requestCarModelMenu->addAction(createCarModel);
    this->requestCarModelMenu->addAction(renameCarModel);
    this->requestCarModelMenu->addAction(deleteCarModel);

    this->requestDetailCategoryMenu = new QMenu(this);
    this->createDetailCategory = new QAction("Добавить категорию", requestDetailCategoryMenu);
    QObject::connect(createDetailCategory,SIGNAL(triggered()),this,SLOT(createDetailCategorySlot()));
    this->renameDetailCategory = new QAction("Переименовать",requestDetailCategoryMenu);
    QObject::connect(renameDetailCategory,SIGNAL(triggered()),this,SLOT(renameDetailCategorySlot()));
    this->deleteDetailCategory = new QAction("Удалить категорию",requestDetailCategoryMenu);
    QObject::connect(deleteDetailCategory,SIGNAL(triggered()),this,SLOT(deleteDetailCategorySlot()));
    this->requestDetailCategoryMenu->addAction(createDetailCategory);
    this->requestDetailCategoryMenu->addAction(renameDetailCategory);
    this->requestDetailCategoryMenu->addAction(deleteDetailCategory);

    this->requestDetailMenu = new QMenu(this);
    this->createDetail = new QAction("Добавить делать",requestDetailMenu);
    QObject::connect(createDetail,SIGNAL(triggered()),this,SLOT(createDetailSlot()));
    this->renameDetail = new QAction("Переименовать деталь",requestDetailMenu);
    QObject::connect(renameDetail,SIGNAL(triggered()),this,SLOT(renameDetailSlot()));
    this->deleteDetail = new QAction("Удалить деталь",requestDetailMenu);
    QObject::connect(deleteDetail,SIGNAL(triggered()),this,SLOT(deleteDetailSlot()));
    this->requestDetailMenu->addAction(createDetail);
    this->requestDetailMenu->addAction(renameDetail);
    this->requestDetailMenu->addAction(deleteDetail);

    this->requestDetailArticleMenu = new QMenu(this);
    this->deleteArticle = new QAction("Удалить артикул", requestDetailArticleMenu);
    this->changeArticle = new QAction("Изменить артикул",requestDetailArticleMenu);

    this->requestGalleryMenu = new QMenu(this);
    this->deleteImage = new QAction("Удалить изображене", requestGalleryMenu);
    QObject::connect(deleteImage,SIGNAL(triggered()),this,SLOT(deleteImageSlot()));
    this->add2ExistArticle = new QAction("Добавить к артиклю",requestGalleryMenu);
    QObject::connect(add2ExistArticle,SIGNAL(triggered()),this,SLOT(add2ExistArticleSlot()));

    this->requestDetailGalleryMenu = new QMenu(this);
    this->returnImage = new QAction("Вернуть изображение",requestDetailGalleryMenu);
    this->setMainImage = new QAction("Установить первым",requestDetailGalleryMenu);
    this->hideImage = new QAction("Пометить как скрытые",requestDetailGalleryMenu);
    this->showImage = new QAction("Убрать из скрытых",requestDetailGalleryMenu);
    QObject::connect(returnImage,SIGNAL(triggered()),this,SLOT(returnImageSlot()));
    QObject::connect(setMainImage,SIGNAL(triggered()),this,SLOT(setMainImageSlot()));
    QObject::connect(hideImage,SIGNAL(triggered()),this,SLOT(hideImageSlot()));
    QObject::connect(showImage,SIGNAL(triggered()),this,SLOT(showImageSlot()));

    this->requestDetailArticleMenu->addAction(deleteArticle);
    this->requestDetailArticleMenu->addAction(changeArticle);
    QObject::connect(deleteArticle,SIGNAL(triggered()),this,SLOT(deleteArticleSlot()));
    QObject::connect(changeArticle,SIGNAL(triggered()),this,SLOT(changeArticleSlot()));

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
    fileDetailArticle->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    ui->gallery->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->gallery,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequestGallery(QPoint)));

    ui->articleGallery->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->articleGallery,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menuRequsetDetailGallery(QPoint)));

    //открытие фотки по двойному щелчку
    QObject::connect(ui->gallery,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(openImage(QModelIndex)));

    QObject::connect(ui->articleGallery,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(openArticleImage(QModelIndex)));

    //установка корневой папки
    fileModelCarMake->setRootPath(globalPath);
    ui->carMake->setModel(fileModelCarMake);
    ui->carMake->setRootIndex(fileModelCarMake->index(globalPath));




    QObject::connect(this->ui->carMake,SIGNAL(clicked(QModelIndex)),this,SLOT(carMakeChanged(QModelIndex)));
    QObject::connect(this->ui->carModel,SIGNAL(clicked(QModelIndex)),this,SLOT(carModelChanged(QModelIndex)));
    QObject::connect(this->ui->detailCategory,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailCategoryChanged(QModelIndex)));
    QObject::connect(this->ui->detail,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailChanged(QModelIndex)));
    QObject::connect(this->ui->detailArticle,SIGNAL(clicked(QModelIndex)),this,SLOT(carDetailArticleChanged(QModelIndex)));

    movingArticlesFlag = false;
    //apply settings
    QDir baseDir;
    baseDir.current().mkdir("tmp");
    this->tmpPath = baseDir.current().path() + "/tmp";
    this->currentTmpPath = this->tmpPath;
    this->settingsFile = new QFile("settings.txt");
    if(settingsFile->exists()) {
        settingsFile->open(QIODevice::ReadWrite);
        QTextStream in(settingsFile);
        this->globalPath = in.readLine();
        this->galleryPath = in.readLine();
        this->pathTofiles = in.readLine();
        this->pathToSiteFiles = in.readLine();
        this->imageSize = in.readLine().toInt();
    } else {
        this->globalPath = "C:/carShop";
        this->galleryPath = "C:/gallery";
        this->imageSize = 120;
        QDir globalPathDir(globalPath);
        if (!globalPathDir.exists()) {
            globalPathDir.mkdir(".");
        }
        QDir galleryPathDir(galleryPath);
        if (!galleryPathDir.exists()) {
            galleryPathDir.mkdir(".");
        }
        settingsFile->open(QIODevice::ReadWrite | QFile::Append | QFile::Text);
        QTextStream out(settingsFile);
        out << globalPath << endl;
        out << galleryPath << endl;
    }
    //galleryPath.replace("\\","/");
    currentGalleryPath = galleryPath;
    this->setSettings(globalPath,galleryPath,pathTofiles,pathToSiteFiles,imageSize);
    //Установка сохраненных значений
    this->savedIndexes = new QFile("savedValues.txt");
    if(savedIndexes->exists()) {
        applySavedValues();
    } else {
        this->savedIndexes->open(QIODevice::ReadWrite | QFile::Append | QFile::Text);
        fileModelCarMake->setRootPath(globalPath);
        ui->carMake->setModel(fileModelCarMake);
        ui->carMake->setRootIndex(fileModelCarMake->index(globalPath));
    }
    this->articleCount = 0;
    ArticleCounterThread* thred = new ArticleCounterThread(globalPath);
    connect(thred,SIGNAL(sendCount(int)),this,SLOT(setArticleCountSlot(int)));
    thred->start();

}

void MainWindow::carMakeChanged(QModelIndex t)
{
    this->sPath = fileModelCarMake->fileInfo(t).absoluteFilePath();
    ui->carModel->setModel(fileModelCarModel);
    this->ui->carModel->setRootIndex(fileModelCarModel->setRootPath(sPath));
//    QDir tmpDir(sPath);
//    QFileInfoList models = tmpDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
//    if(models.size()>0) {
//        QString firstFile = models.first().fileName();
//        QModelIndex k = fileModelCarModel->index(sPath+"/"+firstFile,0);
//        this->ui->carModel->setCurrentIndex(k);
//        carModelChanged(k);
//    }
    clearArticleGallery();
    QModelIndex* tmpIndex = returnFirstChildIndex(sPath, fileModelCarModel);
    if(tmpIndex) {
        ui->carModel->setCurrentIndex(*tmpIndex);
        carModelChanged(*tmpIndex);
    } else {
        clearOutput();
//        delete fileModelDetailCategory;
//        fileModelDetailCategory = new QFileSystemModel(this);
//        delete fileDetail;
//        fileDetail = new QFileSystemModel(this);
//        delete fileDetailArticle;
//        fileDetailArticle = new QFileSystemModel(this);
        cancelCreateArticle();
        afterDeleteCarModelSlot();
   }

}

void MainWindow::carModelChanged(QModelIndex t)
{
    this->modelPath = fileModelCarModel->fileInfo(t).absoluteFilePath();
    ui->detailCategory->setModel(fileModelDetailCategory);
    this->ui->detailCategory->setRootIndex(fileModelDetailCategory->setRootPath(modelPath));
//    QString selectedDetailtmp;


//    if(selectedDetailCategory!="") {
//        QModelIndex tmpIndex = fileModelDetailCategory->index(modelPath+"/"+selectedDetailCategory,0);
//        ui->detailCategory->setCurrentIndex(tmpIndex);
//        selectedDetailtmp = selectedDetail;
//        carDetailCategoryChanged(tmpIndex);
//    } else {
//        clearArticleGallery();
//    }
//    if(selectedDetailtmp!="") {
//        QModelIndex tmpIndex = fileDetail->index(detailCategoryPath+"/"+selectedDetailtmp,0);
//        ui->detail->setCurrentIndex(tmpIndex);
//        carDetailChanged(tmpIndex);
//    } else {
//        clearArticleGallery();
//    }

    QModelIndex* tmpIndex = returnFirstChildIndex(modelPath, fileModelDetailCategory);
    if(tmpIndex) {
        ui->detailCategory->setCurrentIndex(*tmpIndex);
        carDetailCategoryChanged(*tmpIndex);
    } else {
        clearOutput();
        cancelCreateArticle();
//        delete fileDetail;
//        fileDetail = new QFileSystemModel(this);
//        delete fileDetailArticle;
//        fileDetailArticle = new QFileSystemModel(this);

        afterDeleteDetailCategorySlot();
   }

}

void MainWindow::carDetailCategoryChanged(QModelIndex t)
{
    detailCategoryPath = fileModelDetailCategory->fileInfo(t).absoluteFilePath();
    this->selectedDetailCategory = fileModelDetailCategory->fileInfo(t).fileName();
    ui->detail->setModel(fileDetail);
    ui->detail->setRootIndex(fileDetail->setRootPath(detailCategoryPath));
    selectedDetail="";
    //delete fileDetailArticle;
   // fileDetailArticle = new QFileSystemModel(this);
    clearArticleGallery();

    QModelIndex* tmpIndex = returnFirstChildIndex(detailCategoryPath, fileDetail);
    if(tmpIndex) {
        ui->detail->setCurrentIndex(*tmpIndex);
        carDetailChanged(*tmpIndex);
    } else {
        clearOutput();
        cancelCreateArticle();
//        delete fileDetailArticle;
//        fileDetailArticle = new QFileSystemModel(this);
        afterDeleteDetailSlot();
    }
}

void MainWindow::carDetailChanged(QModelIndex t)
{
    detailPath = fileDetail->fileInfo(t).absoluteFilePath();
    ui->detailArticle->setModel(fileDetailArticle);
    this->selectedDetail = fileDetail->fileInfo(t).fileName();
    ui->detailArticle->setRootIndex(fileDetailArticle->setRootPath(detailPath));
    clearArticleGallery();
//    QDir selectedDetailDir(detailPath);
//    QFileInfoList selectedDetailInfoList = selectedDetailDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
//    if(selectedDetailInfoList.size() != 0) {
//        QString articlePath = detailPath +"/" + selectedDetailInfoList.first().fileName();
//        QModelIndex tmpIndex = fileDetailArticle->index(articlePath,0);
//        ui->detailArticle->setCurrentIndex(tmpIndex);
//        carDetailArticleChanged(tmpIndex);
//    }

    QModelIndex* tmpIndex = returnFirstChildIndex(detailPath, fileDetailArticle);
    if(tmpIndex) {
        ui->detailArticle->setCurrentIndex(*tmpIndex);
        carDetailArticleChanged(*tmpIndex);
    } else {
        clearOutput();
        cancelCreateArticle();
    }
}

void MainWindow::openImage(QModelIndex t)
{
    if((ui->gallery->item(t.row()))->type() == 1000) {
        QString path = t.data().toString();
        currentGalleryPath = currentGalleryPath+"/"+path;
        currentTmpPath = currentTmpPath+"/"+path;
        QDir dir(currentTmpPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        updateGallery();
    } else {
        QString path = t.data().toString();
        ImageViewer* iv = new ImageViewer(currentGalleryPath+"/"+path);
        iv->show();
    }
}

void MainWindow::openArticleImage(QModelIndex t)
{
    QString path = t.data().toString();
    QFile openFile(detailArticlePath+"/"+path);
    QFileInfo fileInfo(openFile);
    if(fileInfo.suffix()!="txt") {
        ImageViewer* iv = new ImageViewer(detailArticlePath+"/"+path);
        iv->show();
    } else {
        QProcess process;
        QString fp = detailArticlePath+"/"+path;
        process.start("C:/windows/system32/cmd.exe", QStringList() << "/C" << detailArticlePath +"/"+path);
        process.waitForFinished(-1);
    }
}

void MainWindow::carDetailArticleChanged(QModelIndex t)
{
    this->clearOutput();

    detailArticlePath = fileDetailArticle->fileInfo(t).absoluteFilePath();
    QString articleName=this->fileDetailArticle->fileInfo(this->ui->detailArticle->currentIndex()).fileName();
    QFile describe(detailArticlePath+"/"+articleName+".txt");
    if (describe.open(QIODevice::ReadOnly))
    {
        QTextStream in(&describe);
        QString temp = in.readLine();

        this->ui->makeOutput->setText(temp);

        temp = in.readLine();

        this->ui->modelOutput->setText(temp);

        temp = in.readLine();

        this->ui->categoryOutput->setText(temp);

        temp = in.readLine();

        this->ui->detailOutput->setText(temp);

        temp = in.readLine();

        this->ui->articleOutput->setText(temp);

        temp = in.readLine();

        this->ui->costOutput->setText(temp);

        temp = in.readLine();

        this->ui->originaArtcileOutput->setText(temp);

        temp = in.readLine();

        this->ui->placeOutput->setText(temp);

        temp = in.readLine();

        this->ui->noteOutput->setText(temp);
    }
    updateDetailGallery(detailArticlePath);
    cancelCreateArticle();
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
    requestDetailArticleMenu->removeAction(changeArticle);
    if(ui->detailArticle->indexAt(pos).row()>=0)
    {
        requestDetailArticleMenu->addAction(deleteArticle);
        requestDetailArticleMenu->addAction(changeArticle);
    }
    requestDetailArticleMenu->popup(ui->detailArticle->viewport()->mapToGlobal(pos));
}

void MainWindow::menuRequestGallery(QPoint pos)
{
    requestGalleryMenu->removeAction(deleteImage);
    requestGalleryMenu->removeAction(add2ExistArticle);
    if(ui->gallery->indexAt(pos).row()>=0)
    {
        requestGalleryMenu->addAction(deleteImage);
        requestGalleryMenu->addAction(add2ExistArticle);
    }
    requestGalleryMenu->popup(ui->gallery->viewport()->mapToGlobal(pos));

    //    requestGalleryMenu->exec(ui->gallery->mapToGlobal(pos));
}

void MainWindow::menuRequsetDetailGallery(QPoint pos)
{
    requestDetailGalleryMenu->removeAction(returnImage);
    requestDetailGalleryMenu->removeAction(setMainImage);
    requestDetailGalleryMenu->removeAction(hideImage);
    requestDetailGalleryMenu->removeAction(showImage);
    if(ui->articleGallery->indexAt(pos).row()>=0)
    {
        requestDetailGalleryMenu->addAction(returnImage);
        requestDetailGalleryMenu->addAction(setMainImage);
        requestDetailGalleryMenu->addAction(hideImage);
        requestDetailGalleryMenu->addAction(showImage);
    }
    requestDetailGalleryMenu->popup(ui->articleGallery->viewport()->mapToGlobal(pos));
}

void MainWindow::createCarMakeSlot()
{
    CreateCarMakeWindow* m = new CreateCarMakeWindow();
    m->setPath(globalPath);
    m->show();
}

void MainWindow::renameCarMakeSlot()
{
    RenameCarMakeWindow* rcmw = new RenameCarMakeWindow(globalPath,this->ui->carMake->selectionModel()->selectedIndexes().at(0).data().toString());
    QObject::connect(rcmw,SIGNAL(updateArticlesInfos()),this,SLOT(updateArticlesInfosFiles()));
    this->clearFileSystems();
    rcmw->show();

}

void MainWindow::createCarModelSlot()
{
    CreateCarModelWindow* m = new CreateCarModelWindow();
    m->setPath(sPath);
    m->setCategoriesList(this->detailsMap);
    m->show();
}

void MainWindow::renameCarModelSlot()
{

    QString path2Model = globalPath+"/"+this->ui->carMake->selectionModel()->selectedIndexes().at(0).data().toString();
    RenameCarMakeWindow* rcmw = new RenameCarMakeWindow(path2Model,this->ui->carModel->selectionModel()->selectedIndexes().at(0).data().toString());
    QObject::connect(rcmw,SIGNAL(updateArticlesInfos()),this,SLOT(updateArticlesInfosFiles()));
    this->clearFileSystems();
    rcmw->show();
}

void MainWindow::createDetailCategorySlot()
{
    CreateDetailCategoryWindow* c = new CreateDetailCategoryWindow();
    c->setParameters(globalPath,detailsMap);
    c->show();
}

void MainWindow::renameDetailCategorySlot()
{
    QString selectedDetailCategory = this->ui->detailCategory->selectionModel()->selectedIndexes().at(0).data().toString();
    RenameDetailCategory* rdcw = new RenameDetailCategory(globalPath,selectedDetailCategory,detailsMap);
    QObject::connect(rdcw,SIGNAL(updateArticlesInfos()),this,SLOT(updateArticlesInfosFiles()));
    this->clearFileSystems();
    rdcw->show();
}

void MainWindow::createDetailSlot()
{
    CreateDetailWindow* w = new CreateDetailWindow();
    w->setParameters(globalPath,detailsMap,selectedDetailCategory);
    w->show();
}

void MainWindow::renameDetailSlot()
{
    QString detail = this->ui->detail->selectionModel()->selectedIndexes().at(0).data().toString();
    QString selectedCategory = this->ui->detailCategory->selectionModel()->selectedIndexes().at(0).data().toString();
    RenameDetailWindow* rdw = new RenameDetailWindow(globalPath,selectedCategory,detail,detailsMap);
    QObject::connect(rdw,SIGNAL(updateArticlesInfos()),this,SLOT(updateArticlesInfosFiles()));
    this->clearFileSystems();
    rdw->show();
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

void MainWindow::changeArticleSlot()
{
    ChangeArticleWindow* caw = new ChangeArticleWindow(detailPath+"/"+this->ui->articleOutput->text(),this->ui->articleOutput->text());
    connect(caw,SIGNAL(changed(QString)),this,SLOT(updateArticlesOutput(QString)));
    caw->show();
}

void MainWindow::returnImageSlot()
{
    QList<QListWidgetItem*> files = this->ui->articleGallery->selectedItems();
    QList<QListWidgetItem*>::Iterator i;
    QDir dir(currentGalleryPath);
    for(i=files.begin();i!=files.end();i++)
    {
        QFile f(detailArticlePath+"/"+(*i)->text());
        QFileInfo *d = new QFileInfo(f);

        int num = 1;
        QString fileName;
        if(dir.exists(d->fileName())){
            while (dir.exists(d->baseName()+"("+QString::number(num)+")."+d->suffix())){
                num++;
            }
            fileName = d->baseName()+"("+QString::number(num)+")."+d->suffix();
        } else {
            fileName = d->fileName();
        }

        QFile::copy(detailArticlePath+"/"+d->fileName(),currentGalleryPath+"/"+fileName);
        QIcon* j = new QIcon(currentGalleryPath+"/"+fileName);
        QListWidgetItem* g = new QListWidgetItem((j->pixmap(QSize(300,300))),fileName);
        this->images->append(g);
        double ratio = 4.0/3;
        g->setSizeHint(QSize(imageSize,imageSize/ratio+20));
        this->ui->gallery->addItem(g);
        delete j;
        this->ui->articleGallery->removeItemWidget((*i));
        QFile::remove(detailArticlePath+"/"+(*i)->text());



        delete d;
    }
    for(i=files.begin();i!=files.end();i++)
    {
        delete (*i);
    }

    updateArticleGalleryFileNames();
}

void MainWindow::setMainImageSlot()
{ 
    if(this->ui->articleGallery->selectedItems().size() == 1 &&
                !this->ui->articleGallery->selectedItems().first()->text().contains(".txt")) {
        //qDebug() << "triger";

        QString selectedItemName = this->ui->articleGallery->selectedItems().first()->text();
        QStringList splittedFileName = selectedItemName.split(".");
        clearArticleGallery();
        QFile::rename(detailArticlePath+"/"+ selectedItemName, detailArticlePath+"/tmpName.tmp");
        QFile::rename(detailArticlePath+"/"+ this->ui->articleOutput->text()+"_0."+splittedFileName.at(1),
                      detailArticlePath+"/"+selectedItemName);
        QFile::rename(detailArticlePath+"/tmpName.tmp",
                      detailArticlePath+"/"+ this->ui->articleOutput->text()+"_0."+splittedFileName.at(1));
        updateDetailGallery(detailArticlePath);
    }
}

void MainWindow::deleteImageSlot()
{
    QList<QListWidgetItem*> files = this->ui->gallery->selectedItems();
    QList<QListWidgetItem*>::Iterator i;
    for(i=files.begin();i!=files.end();i++)
    {
        QFile f(currentGalleryPath+"/"+(*i)->text());
        f.remove();
        this->ui->gallery->removeItemWidget((*i));
        delete (*i);
    }
}

void MainWindow::add2ExistArticleSlot()
{
    QList<QListWidgetItem*> files = this->ui->gallery->selectedItems();
    QList<QListWidgetItem*>::Iterator i;
    QString article = this->fileDetailArticle->fileInfo(this->ui->detailArticle->currentIndex()).fileName();
    if(article == "") {
        return;
    }
    QDir existArticle (detailPath+"/"+this->ui->articleOutput->text());
    if(!existArticle.exists() || this->ui->articleOutput->text().isEmpty()){
        showErrorWindow("Артикул еще не сохранен.\nВыделите нужные изображения и нажмите кнопку «Cохранить»");
        return;
    }

    QString path = detailPath+"/"+this->ui->articleOutput->text();
    QDir tmpDir(path);
    QFileInfoList articleFiles = tmpDir.entryInfoList();
    QString lastFileName = articleFiles.last().fileName();
    QRegExp rx("(\\_|\\.)");
    QStringList splittedFileName = lastFileName.split(rx);
    int imageIndex = splittedFileName.at(1).toInt();
    if(splittedFileName.at(1)!= "txt")
        imageIndex++;
    for(i=files.begin();i!=files.end();i++)
    {
        QFile f(currentGalleryPath+"/"+(*i)->text());
        QFileInfo *d = new QFileInfo(f);
        QString h = detailPath+"/"+this->ui->articleOutput->text()+"/"+this->ui->articleOutput->text()+"_"+QString::number(imageIndex)+"."+d->suffix();
        bool b = QFile::copy(currentGalleryPath+"/"+(*i)->text(),h);
        qDebug()<<"Copying file "+currentGalleryPath+"/"+(*i)->text()+" into "+h+" is "+b;
        imageIndex++;
    }
    removeItemFromGallery();
    updateDetailGallery(detailArticlePath);
}

void MainWindow::hideImageSlot()
{

    QList<QListWidgetItem*> files = this->ui->articleGallery->selectedItems();
    QList<QListWidgetItem*>::Iterator i;
    for(i=files.begin();i!=files.end();i++)
    {
        QString selectedItemName = (*i)->text();
        if(!selectedItemName.contains(QRegExp("(\.txt)|(\[H\])")))
            QFile::rename(detailArticlePath+"/"+ selectedItemName, detailArticlePath+"/[H]"+selectedItemName);
    }
    updateDetailGallery(detailArticlePath);
}

void MainWindow::showImageSlot()
{
    QList<QListWidgetItem*> files = this->ui->articleGallery->selectedItems();
    QList<QListWidgetItem*>::Iterator i;
    for(i=files.begin();i!=files.end();i++)
    {
        QString selectedItemName = (*i)->text();
        if(selectedItemName.contains("[H]") && !selectedItemName.contains(".txt")) {
            QString oldame = selectedItemName;
            QFile::rename(detailArticlePath+"/"+ oldame, detailArticlePath+"/"+selectedItemName.remove("[H]"));
        }
    }
    updateDetailGallery(detailArticlePath);
}

void MainWindow::deleteCarModelSlot()
{
    ConfirmWindow* c = new ConfirmWindow();
    c->setPath(modelPath);
    connect(c,SIGNAL(confirm()),this,SLOT(afterDeleteCarModelSlot()));
    c->show();
}

void MainWindow::afterDeleteCarModelSlot()
{
    delete fileModelDetailCategory;
    fileModelDetailCategory = new QFileSystemModel(this);
    delete fileDetail;
    fileDetail = new QFileSystemModel(this);
    delete fileDetailArticle;
    fileDetailArticle = new QFileSystemModel(this);

    fileModelDetailCategory->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileDetail->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    fileDetailArticle->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
}

void MainWindow::deleteDetailCategorySlot()
{
    QString catName = this->fileModelDetailCategory->fileInfo(this->ui->detailCategory->currentIndex()).fileName();
    DeleteDetailCategoryWindow *w = new DeleteDetailCategoryWindow();
    w->setParameters(globalPath,detailsMap,catName);
    connect(w,SIGNAL(deleted()),this,SLOT(afterDeleteDetailCategorySlot()));
    w->show();
}

void MainWindow::afterDeleteDetailCategorySlot()
{
    delete fileDetail;
    fileDetail = new QFileSystemModel(this);
    delete fileDetailArticle;
    fileDetailArticle = new QFileSystemModel(this);

    fileDetail->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    fileDetailArticle->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
}

void MainWindow::deleteCarMakeSlot()
{
    ConfirmWindow* c = new ConfirmWindow();
    c->setPath(sPath);
    connect(c,SIGNAL(confirm()),this,SLOT(afterDeleteCarMakeSlot()));
    c->show();
}

void MainWindow::afterDeleteCarMakeSlot()
{
    delete fileModelCarModel;
    fileModelCarModel = new QFileSystemModel(this);
    delete fileModelDetailCategory;
    fileModelDetailCategory = new QFileSystemModel(this);
    delete fileDetail;
    fileDetail = new QFileSystemModel(this);
    delete fileDetailArticle;
    fileDetailArticle = new QFileSystemModel(this);

    fileModelCarModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileModelDetailCategory->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileDetail->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    fileDetailArticle->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
}

void MainWindow::deleteDetailSlot()
{
    QString catName = this->fileModelDetailCategory->fileInfo(this->ui->detailCategory->currentIndex()).fileName();
    QString detName = this->fileDetail->fileName(this->ui->detail->currentIndex());
    DeleteDetailWindow* w = new DeleteDetailWindow();
    w->setParameters(detName,globalPath,catName,detailsMap);
    connect(w,SIGNAL(deleted()),this,SLOT(afterDeleteDetailSlot()));
    w->show();
}

void MainWindow::afterDeleteDetailSlot()
{
    delete fileDetailArticle;
    fileDetailArticle = new QFileSystemModel(this);

    fileDetailArticle->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
}

MainWindow::~MainWindow()
{
    delete detailsMap;
    delete details;
    delete images;
    delete articleImages;
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
    if(!detailsMap->empty()) {
        this->selectedDetailCategory = detailsMap->firstKey();
        QStringList* detailList = detailsMap->value(selectedDetailCategory);
        if(!detailList->empty())
        this->selectedDetail = detailsMap->value(selectedDetailCategory)->first();
    }
    delete dir;
    delete dir2;
    delete dir3;
}

void MainWindow::showErrorWindow(QString errorMessage)
{
    QMessageBox messageBox;
    messageBox.critical(this,"Ошибка",errorMessage);
    messageBox.setFixedSize(500,200);
}

QModelIndex *MainWindow::returnFirstChildIndex(const QString &path, QFileSystemModel *&model)
{
    QDir selectedDir(path);
    QFileInfoList selectedInfoList = selectedDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    if(selectedInfoList.size() != 0) {
       QString firstItemPath = path +"/" + selectedInfoList.first().fileName();
       QModelIndex* tmpIndex = new QModelIndex(model->index(firstItemPath,0));
       return tmpIndex;
    }
    return NULL;
}

void MainWindow::beforCreateArticle()
{
    if(this->ui->detailCategory->currentIndex().isValid()) {
        if(this->editDetail == false) {
            this->ui->articleOutput->setEnabled(true);
            this->ui->costOutput->setEnabled(true);
            this->ui->originaArtcileOutput->setEnabled(true);
            this->ui->placeOutput->setEnabled(true);
            this->ui->noteOutput->setEnabled(true);


            this->ui->articleOutput->setText("");
            this->ui->costOutput->setText("");
            this->ui->originaArtcileOutput->setText("");
            this->ui->placeOutput->setText("");
            this->ui->noteOutput->setText("");
            this->ui->articleOutput->setFocus();
            this->ui->articleGallery->clear();
            this->ui->gallery->clearSelection();
            this->editDetail = true;
            QList<QModelIndex> list = this->ui->carMake->selectionModel()->selectedIndexes();
            this->ui->makeOutput->setText(this->ui->carMake->selectionModel()->selectedIndexes().at(0).data().toString());
            this->ui->modelOutput->setText(this->ui->carModel->selectionModel()->selectedIndexes().at(0).data().toString());
            this->ui->categoryOutput->setText(this->ui->detailCategory->selectionModel()->selectedIndexes().at(0).data().toString());
            this->ui->detailOutput->setText(this->ui->detail->selectionModel()->selectedIndexes().at(0).data().toString());
            this->ui->createArticleButton->setText("Сохранить");
        } else {
            //создаем артикул
            if(this->ui->articleOutput->text()!="") {
                QDir dir(detailPath+"/"+this->ui->articleOutput->text());
                if(!dir.exists())
                {
                     dir.mkpath(".");
                } else {
                    showErrorWindow("Такой артикул уже есть");
                    //this->editDetail = false;
                    return;
                }
                QFile file(detailPath+"/"+this->ui->articleOutput->text()+"/"+this->ui->articleOutput->text()+".txt");
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
                     stream<<NEW_STATE<<endl;
                     file.close();
                } else {
                    qDebug()<<"Невозможно открыть выходной файл";
                    return;
                }
                QList<QListWidgetItem*> files = this->ui->gallery->selectedItems();
                QList<QListWidgetItem*>::Iterator i;
                int imageNumber=0;
                for(i=files.begin();i!=files.end();i++)
                {
                    QFile f(currentGalleryPath+"/"+(*i)->text());
                    QFileInfo *d = new QFileInfo(f);
                    QString h = detailPath+"/"+this->ui->articleOutput->text()+"/"+this->ui->articleOutput->text()+"_"+QString::number(imageNumber)+"."+d->suffix();
                    bool b = QFile::copy(galleryPath+"/"+(*i)->text(),h);
                    QFile::remove(galleryPath+"/"+(*i)->text());
                    this->ui->gallery->removeItemWidget((*i));
                    delete (*i);
                    delete d;
                    imageNumber++;
                }
                articleCount++;
            } else {
                 showErrorWindow("Введите название артикула");
                 return;
            }
            cancelCreateArticle();

            //установка курсора но созданную деталь
            QModelIndex k = fileDetailArticle->index(detailPath+"/"+this->ui->articleOutput->text(),0);
            this->ui->detailArticle->setCurrentIndex(k);
            this->updateDetailGallery(detailPath+"/"+this->ui->articleOutput->text());
            this->detailArticlePath = detailPath+"/"+this->ui->articleOutput->text();
        }
    }
}

void MainWindow::cancelCreateArticle()
{
    this->editDetail = false;
    this->ui->createArticleButton->setText("Создать новый");
    this->ui->gallery->clearSelection();

    this->ui->articleOutput->setEnabled(false);
    this->ui->costOutput->setEnabled(false);
    this->ui->originaArtcileOutput->setEnabled(false);
    this->ui->placeOutput->setEnabled(false);
    this->ui->noteOutput->setEnabled(false);
}

void MainWindow::openSettingsWindow()
{
    SettingsWindow* w = new SettingsWindow(globalPath,galleryPath,pathTofiles,
                                           pathToSiteFiles,imageSize);
    QObject::connect(w,SIGNAL(setSettings(QString,QString,QString,QString,int)),
                     this,SLOT(setSettings(QString,QString,QString,QString,int)));
    w->show();
}

void MainWindow::setSettings(QString path, QString galleryPath,
                             QString pathToFiles, QString pathToSiteFiles,
                             int imageSize)
{
    this->globalPath = path;
    this->galleryPath = galleryPath;
    this->pathTofiles = pathToFiles;
    this->pathToSiteFiles = pathToSiteFiles;
    this->imageSize = imageSize;
    this->settingsFile->remove();
    settingsFile->open(QIODevice::ReadWrite | QFile::Append | QFile::Text);
    QTextStream out(settingsFile);
    out << this->globalPath << endl;
    out << this->galleryPath << endl;
    out << this->pathTofiles << endl;
    out << this->pathToSiteFiles << endl;
    out << this->imageSize << endl;

    this->updateAll();
    out.flush();
    settingsFile->close();
}

void MainWindow::updateGallery()
{
    //qDebug() << currentGalleryPath;
    //qDebug() << galleryPath;
    if(isUpdateGalleryFinished) {
        isUpdateGalleryFinished = false;
        if(galleryPath == currentGalleryPath)
            ui->backButton->setDisabled(true);
        else
            ui->backButton->setDisabled(false);

        this->ui->gallery->clear();
        this->ui->gallery->setUpdatesEnabled(false);
        this->ui->gallery->setUniformItemSizes(true);
        this->ui->gallery->setSpacing(5);
        ui->gallery->setIconSize(QSize(imageSize,imageSize));

        QString regex = "*";
        QList<QListWidgetItem*> galleryItems = this->ui->gallery->findItems(regex,Qt::MatchWrap | Qt::MatchWildcard);
        GalleryUpateThread* thred = new GalleryUpateThread(&galleryItems,currentGalleryPath,currentTmpPath);

        connect(thred,SIGNAL(sendQListWidgetItem(QListWidgetItem*)),
                this,SLOT(updateGallerySlot(QListWidgetItem*)));
        connect(thred,SIGNAL(finished()),
                this,SLOT(updateGalleryFinishedSlot()));
        this->ui->UpdateGalleryProgressBar->reset();
        this->ui->UpdateGalleryProgressBar->setRange(0,thred->getNumberOfImages());
        this->ui->UpdateGalleryProgressBar->setVisible(true);

        thred->start();
    }
}

void MainWindow::updateArticleGalleryFileNames()
{
    clearArticleGallery();

    int index = 0;
    QDir path(detailArticlePath);
    QFileInfoList articleFiles = path.entryInfoList(QDir::Files);
    QFileInfoList::iterator itr;
    for(itr=articleFiles.begin(); itr!= articleFiles.end(); ++itr)
    {
        if((*itr).suffix() == "txt")
            continue;
        QString oldName = detailArticlePath+"/"+ (*itr).fileName();
        QString prefix = oldName.contains("[H]") ? "[H]" : "";
        QString newName = detailArticlePath+"/"+prefix+this->ui->articleOutput->text()+
                          "_"+QString::number(index)+"."+(*itr).suffix();
        bool sucessful = QFile::rename(oldName,newName);
        qDebug()<<"Renaiming "<<oldName<<" into "<< newName <<" is "<<sucessful;
        ++index;
    }

    updateDetailGallery(detailArticlePath);
}

void MainWindow::saveSizes(QTextStream* stream)
{
    QList<int> fileSystemWidgets = this->ui->splitter->sizes();
    foreach (int size, fileSystemWidgets) {
        *stream<<size<<endl;
    }

    fileSystemWidgets = this->ui->splitter_2->sizes();
    foreach (int size, fileSystemWidgets) {
        *stream<<size<<endl;
    }

    fileSystemWidgets = this->ui->splitter_3->sizes();
    foreach (int size, fileSystemWidgets) {
        *stream<<size<<endl;
    }
}

void MainWindow::saveSelectedValues()
{
    if(savedIndexes->isOpen()) {

        savedIndexes->close();
        savedIndexes->remove();
        savedIndexes->open(QIODevice::ReadWrite | QFile::Append | QFile::Text);

        QTextStream out(savedIndexes);
        if(this->ui->carMake->selectionModel()!=NULL)
            saveSelectedValue(this->ui->carMake->selectionModel()->selectedIndexes(),&out);
        if(this->ui->carModel->selectionModel()!=NULL)
            saveSelectedValue(this->ui->carModel->selectionModel()->selectedIndexes(),&out);
        if(this->ui->detailCategory->selectionModel()!=NULL)
            saveSelectedValue(this->ui->detailCategory->selectionModel()->selectedIndexes(),&out);
        if(this->ui->detail->selectionModel()!=NULL)
            saveSelectedValue(this->ui->detail->selectionModel()->selectedIndexes(),&out);
        if(this->ui->detailArticle->selectionModel()!=NULL)
            saveSelectedValue(this->ui->detailArticle->selectionModel()->selectedIndexes(),&out);
        savedIndexes->close();
    }
}

int MainWindow::saveSelectedValue(QModelIndexList list, QTextStream *out)
{
    if(list.size()!=0) {
        *out<<list.first().data().toString()<<endl;
        return 1;
    } else {
        return 0;
    }
}

void MainWindow::updateAll()
{
    fileModelCarMake->setRootPath(globalPath);
    ui->carMake->setModel(fileModelCarMake);
    ui->carMake->setRootIndex(fileModelCarMake->index(globalPath));
    currentGalleryPath = galleryPath;
    this->updateGallery();

    delete fileModelCarModel;
    delete fileModelDetailCategory;
    delete fileDetail;
    delete fileDetailArticle;

    fileModelCarModel = new QFileSystemModel(this);
    fileModelDetailCategory = new QFileSystemModel(this);
    fileDetail = new QFileSystemModel(this);
    fileDetailArticle = new QFileSystemModel(this);

    fileModelCarModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileModelDetailCategory->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    fileDetail->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    fileDetailArticle->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    this->getDetailCategoriesList();
}

void MainWindow::removeItemFromGallery()
{
    QList<QListWidgetItem*> files = this->ui->gallery->selectedItems();
    QList<QListWidgetItem*>::Iterator i;
    for(i=files.begin();i!=files.end();i++)
    {
        QFile f(currentGalleryPath+"/"+(*i)->text());
        f.remove();
        this->ui->gallery->removeItemWidget((*i));
        delete (*i);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    if(event->key()==16777220)
    {
        beforCreateArticle();
    }
    if ( (event->key() == Qt::Key_F) && QApplication::keyboardModifiers()
                                     && Qt::ControlModifier){
        findeArticlesSlot();
    }
    if ( (event->key() == Qt::Key_X) && QApplication::keyboardModifiers()
                                     && Qt::ControlModifier){
        foreach (QModelIndex index,
                ui->detailArticle->selectionModel()->selectedIndexes())
            movingArticlesList.append(this->fileDetailArticle->fileInfo(index).fileName());
        oldDetailPath = detailPath;
        movingArticlesFlag = true;

    }
    if ( (event->key() == Qt::Key_V) && QApplication::keyboardModifiers()
                                     && Qt::ControlModifier
                                     && movingArticlesFlag){
        MovingAriclesDialog* dialog =
                new MovingAriclesDialog(oldDetailPath,
                                        detailPath,
                                        movingArticlesList.join(","));
        if (dialog->exec() == QDialog::Accepted) {
            delete dialog;
            QString articlesPath = detailPath;
            QString path = globalPath;
            path.replace("\\","/");
            articlesPath.remove(path+"/");
            QStringList categories = articlesPath.split("/");
            QDir dir;
            foreach (QString article, movingArticlesList) {
                if(dir.rename(oldDetailPath+"/"+article,
                                detailPath+"/"+article )) {


                        QFile file(detailPath+"/"+article+"/"+article+".txt");
                        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {

                        QStringList stringList;
                        QTextStream textStream1(&file);

                        while (!textStream1.atEnd())
                            stringList << textStream1.readLine();
                        QMutableStringListIterator i (stringList);
                        i.next();
                        i.setValue(categories.at(0)); i.next();
                        i.setValue(categories.at(1)); i.next();
                        i.setValue(categories.at(2)); i.next();
                        i.setValue(categories.at(3));

                        file.close();
                        file.open((QFile::WriteOnly|QFile::Truncate|QIODevice::Text));
                        QTextStream textStream2(&file);
                        textStream2 << stringList.join('\n');
                        qDebug() << stringList;
                        file.close();
                    }
                }
            }
        }
        movingArticlesList.clear();
        oldDetailPath.clear();
        movingArticlesFlag = false;
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{

}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QFile sizeFile("size.txt");
    if(sizeFile.exists())
    {
       sizeFile.remove();
    }
    sizeFile.open(QIODevice::ReadWrite | QFile::Append | QFile::Text);
    QTextStream out(&sizeFile);
    out<<this->height()<<endl;
    out<<this->width()<<endl;
    saveSizes(&out);
    sizeFile.close();
    saveSelectedValues();
    e->accept();
    savedIndexes->close();
}

//void MainWindow::dropEvent(QDropEvent *e)
//{
//    qDebug() << "dropEvent";
//}

//void MainWindow::dragEnterEvent(QDragEnterEvent *e)
//{
//    qDebug() << "dragEnterEvent";
//}

void MainWindow::applySavedValues()
{


    this->savedIndexes->open(QIODevice::ReadWrite |  QFile::Text);
    QTextStream stream(savedIndexes);
    if(!stream.atEnd()) {
        QString savedCarMake = stream.readLine();
        QModelIndex carMakeIndex = fileModelCarMake->index(globalPath+"/"+savedCarMake,0);
        this->ui->carMake->setCurrentIndex(carMakeIndex);
        //carMakeChanged(fileModelCarMake->index(globalPath+"/"+savedCarMake,0));
        fileModelCarMake->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
        this->sPath = fileModelCarMake
                ->fileInfo(fileModelCarMake->index(globalPath+"/"+savedCarMake,0)).absoluteFilePath();
        ui->carModel->setModel(fileModelCarModel);
        this->ui->carModel->setRootIndex(fileModelCarModel->setRootPath(sPath));
    } else {
        //afterDeleteCarMakeSlot();
    }



    if(!stream.atEnd()) {
        QString savedCarModel = stream.readLine();
        QModelIndex savedCarModelIndex = fileModelCarModel->index(sPath+"/"+savedCarModel,0);
        this->ui->carModel->setCurrentIndex(savedCarModelIndex);
        //carModelChanged(savedCarModelIndex);
        fileModelCarModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
        this->modelPath = fileModelCarModel->fileInfo(savedCarModelIndex).absoluteFilePath();
        ui->detailCategory->setModel(fileModelDetailCategory);
        this->ui->detailCategory->setRootIndex(fileModelDetailCategory->setRootPath(modelPath));
    } else {
        afterDeleteCarMakeSlot();
        clearArticleGallery();
    }

    if(!stream.atEnd()) {
        QString savedDetailCategory = stream.readLine();
        QModelIndex savedDetailCategoryIndex = fileModelDetailCategory->index(modelPath+"/"+savedDetailCategory,0);
        this->ui->detailCategory->setCurrentIndex(savedDetailCategoryIndex);
        //carDetailCategoryChanged(savedDetailCategoryIndex);
        fileModelDetailCategory->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
        detailCategoryPath = fileModelDetailCategory->fileInfo(savedDetailCategoryIndex).absoluteFilePath();
        this->selectedDetailCategory = fileModelDetailCategory->fileInfo(savedDetailCategoryIndex).fileName();
        ui->detail->setModel(fileDetail);
        ui->detail->setRootIndex(fileDetail->setRootPath(detailCategoryPath));
        selectedDetail="";
    } else {
        afterDeleteCarModelSlot();
        clearArticleGallery();
    }

    if(!stream.atEnd()) {
        QString savedDetail = stream.readLine();
        QModelIndex savedDetailIndex = fileDetail->index(detailCategoryPath+"/"+savedDetail,0);
        this->ui->detail->setCurrentIndex(savedDetailIndex);
        //carDetailChanged(savedDetailIndex);
        fileDetail->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
        detailPath = fileDetail->fileInfo(savedDetailIndex).absoluteFilePath();
        ui->detailArticle->setModel(fileDetailArticle);
        this->selectedDetail = fileDetail->fileInfo(savedDetailIndex).fileName();
        ui->detailArticle->setRootIndex(fileDetailArticle->setRootPath(detailPath));
    } else {
        afterDeleteDetailCategorySlot();
        clearArticleGallery();
    }

    if(!stream.atEnd()) {
        QString savedArticle = stream.readLine();
        QModelIndex articleModelIndex = fileDetailArticle->index(detailPath+"/"+savedArticle,0);
        ui->detailArticle->setCurrentIndex(articleModelIndex);
        fileDetailArticle->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
        carDetailArticleChanged(articleModelIndex);
    } else {
        afterDeleteDetailSlot();
        clearArticleGallery();
    }
}

void MainWindow::initWindowSize()
{
    QFile sizeFile("size.txt");
    if(sizeFile.exists()) {
        sizeFile.open(QIODevice::ReadWrite |  QFile::Text);
        QTextStream out(&sizeFile);
        bool isNumber;
        QString height = out.readLine();
        QString width = out.readLine();
        int intHeight = height.toInt(&isNumber);
        if(!isNumber) {
            return;
        }
        int intWidth = width.toInt(&isNumber);
        if(!isNumber) {
            return;
        }
        this->resize(intWidth,intHeight);

        int i;
        QString widgetSize;
        QList<int> fileSystemWidgetsSize;
        for(i = 0;i<5;i++) {
            widgetSize = out.readLine();
            fileSystemWidgetsSize.append(widgetSize.toInt());
        }
        this->ui->splitter->setSizes(fileSystemWidgetsSize);

        QList<int> leftPanelWidgetsSize;
        for(i = 0;i<3;i++) {
            widgetSize = out.readLine();
            leftPanelWidgetsSize.append(widgetSize.toInt());
        }
        this->ui->splitter_2->setSizes(leftPanelWidgetsSize);
        sizeFile.close();

        QList<int> leftAndRightPanelsSize;
        for(i = 0;i<3;i++) {
            widgetSize = out.readLine();
            leftAndRightPanelsSize.append(widgetSize.toInt());
        }
        this->ui->splitter_3->setSizes(leftAndRightPanelsSize);
        sizeFile.close();
    } else {
        return;
    }
 }

void MainWindow::clearFileSystems()
{
    qDebug()<<"in clearFileSystems()";

    delete fileModelCarMake;

    fileModelCarMake = new QFileSystemModel(this);

    fileModelCarMake->setRootPath(globalPath);
    ui->carMake->setModel(fileModelCarMake);
    ui->carMake->setRootIndex(fileModelCarMake->index(globalPath));


    delete fileModelCarModel;
    delete fileModelDetailCategory;
    delete fileDetail;
    delete fileDetailArticle;

    fileModelCarModel = new QFileSystemModel(this);
    fileModelDetailCategory = new QFileSystemModel(this);
    fileDetail = new QFileSystemModel(this);
    fileDetailArticle = new QFileSystemModel(this);

}

void MainWindow::clearArticleGallery()
{
    this->ui->articleGallery->clear();
}

void MainWindow::updateDetails()
{
    QString carMake;
    QString carModel;
    QString detailCategory;
    QString detail;
    QString article;

    QString cost;
    QString originalArticle;
    QString place;
    QString note;

    QDir root(globalPath);
    UpdatingWindow* uw = new UpdatingWindow(root.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot).size(),"Обновление информации");
    uw->show();
    QApplication::processEvents();
    foreach(QString carMakeDirName,root.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir carMakeTempDir(globalPath+"/"+carMakeDirName);
        foreach(QString carModelDirName, carMakeTempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            QDir carModelTempDir(globalPath+"/"+carMakeDirName+"/"+carModelDirName);
            foreach(QString detailCategoryDirName, carModelTempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
                QDir detailCategoryTempDir(globalPath+"/"+carMakeDirName+"/"+carModelDirName+"/"+detailCategoryDirName);
                foreach(QString detailDirName, detailCategoryTempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
                    QDir detailTempDir(globalPath+"/"+carMakeDirName+"/"+carModelDirName+"/"+detailCategoryDirName+"/"+detailDirName);
                    foreach(QString articleDirName, detailTempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
                        QFile file(globalPath+"/"+carMakeDirName+"/"+carModelDirName+"/"+detailCategoryDirName+"/"+detailDirName+"/"+articleDirName+"/"+articleDirName+".txt");
                        if(file.open(QIODevice::ReadWrite |  QFile::Text))
                        {
                             QTextStream stream(&file);
                             carMake = stream.readLine();
                             carModel = stream.readLine();
                             detailCategory = stream.readLine();
                             detail = stream.readLine();
                             article = stream.readLine();
                             cost = stream.readLine();
                             originalArticle = stream.readLine();
                             place = stream.readLine();
                             note = stream.readLine();
                             file.close();

                             if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QFile::Text))
                             {
                                  QTextStream stream(&file);
                                  stream<<carMakeDirName<<"\n";
                                  stream<<carModelDirName<<"\n";
                                  stream<<detailCategoryDirName<<"\n";
                                  stream<<detailDirName<<"\n";
                                  stream<<articleDirName<<"\n";
                                  stream<<cost<<"\n";
                                  stream<<originalArticle<<"\n";
                                  stream<<place<<"\n";
                                  stream<<note<<"\n";
                                  file.close();
                             }
                        } else {
                            qDebug()<<"File wasn't opened in updateDetails()";
                        }
                    }
                }
            }
        }
        uw->increment();
        QApplication::processEvents();
    }
    uw->closeUpdatingWindow();
}

void MainWindow::updateDetailGallery(QString detailPath)
{
    if(isUpdateDetailGalleryFinished){
        this->ui->articleGallery->clear();
        this->ui->articleGallery->setUpdatesEnabled(false);
        this->isUpdateDetailGalleryFinished = false;
        ArticleGalleryUpdateThread* thred = new ArticleGalleryUpdateThread(detailPath+"/");

        connect(thred,SIGNAL(sendQListWidgetItem(QListWidgetItem*)),
                this,SLOT(updateArticleGallerySlot(QListWidgetItem*)));
        connect(thred,SIGNAL(finished()),
                this,SLOT(updateArticleGalleryFinishedSlot()));
        thred->start();
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

void MainWindow::exportToExcelSlot()
{
    ExcelHandler* excleHandlerWindow = new ExcelHandler(this->globalPath,this->pathTofiles, this->pathToSiteFiles);
    excleHandlerWindow->show();
}

void MainWindow::importFromExcelSlot()
{
    ImportFromExcelWindow* importFromExcelWindow = new ImportFromExcelWindow(this->globalPath,detailsMap);
    importFromExcelWindow->show();
}

void MainWindow::updateArticlesInfosFiles()
{
    this->updateDetails();
}

void MainWindow::updateArticlesOutput(QString newName)
{
    QModelIndex index = fileDetailArticle->index(newName,0);
    this->ui->detailArticle->setCurrentIndex(index);

    this->clearOutput();

    detailArticlePath = fileDetailArticle->fileInfo(index).absoluteFilePath();
    QString articleName=this->fileDetailArticle->fileInfo(this->ui->detailArticle->currentIndex()).fileName();
    QFile describe(detailArticlePath+"/"+articleName+".txt");
    if (describe.open(QIODevice::ReadOnly))
    {
        QTextStream in(&describe);
        QString temp = in.readLine();

        this->ui->makeOutput->setText(temp);

        temp = in.readLine();

        this->ui->modelOutput->setText(temp);

        temp = in.readLine();

        this->ui->categoryOutput->setText(temp);

        temp = in.readLine();

        this->ui->detailOutput->setText(temp);

        temp = in.readLine();

        this->ui->articleOutput->setText(temp);

        temp = in.readLine();

        this->ui->costOutput->setText(temp);

        temp = in.readLine();

        this->ui->originaArtcileOutput->setText(temp);

        temp = in.readLine();

        this->ui->placeOutput->setText(temp);

        temp = in.readLine();

        this->ui->noteOutput->setText(temp);
    }
    cancelCreateArticle();

    updateArticleGalleryFileNames();
}

void MainWindow::updateGallerySlot(QListWidgetItem *item)
{   double ratio = 4.0/3;
    if(item->type() == 1000) {
          item->setSizeHint(QSize(imageSize,imageSize/ratio+20));
    } else {
        item->setSizeHint(QSize(imageSize,imageSize/ratio+20));
        this->images->append(item);
    }
    this->ui->gallery->addItem(item);
    int value = this->ui->UpdateGalleryProgressBar->value();
    ++value;
    this->ui->UpdateGalleryProgressBar->setValue(value);
    this->ui->UpdateGalleryProgressBar->update();
}

void MainWindow::updateArticleGallerySlot(QListWidgetItem *item)
{
        this->ui->articleGallery->addItem(item);
}

void MainWindow::updateGalleryFinishedSlot()
{
    this->ui->UpdateGalleryProgressBar->setVisible(false);
    this->ui->gallery->setUpdatesEnabled(true);
    isUpdateGalleryFinished = true;
}

void MainWindow::updateArticleGalleryFinishedSlot()
{
    this->ui->articleGallery->setUpdatesEnabled(true);
    isUpdateDetailGalleryFinished = true;
}

void MainWindow::setArticleCountSlot(int count)
{
    this->articleCount = count;
}

void MainWindow::findeArticlesSlot()
{
    FindArticlesWindow* m = new FindArticlesWindow(globalPath,articleCount);
    connect(m,SIGNAL(articleActivated(QString*)),
            this,SLOT(setArticleSlot(QString*)));
    m->show();

}

void MainWindow::setArticleSlot(QString* path)
{
    QStringList categories = path->split("/",QString::SkipEmptyParts);
    categories.pop_front();
    categories.pop_back();

    clearArticleGallery();

    QString savedCarMake = categories.at(0);
    QModelIndex carMakeIndex = fileModelCarMake->index(globalPath+"/"+savedCarMake,0);
    this->ui->carMake->setCurrentIndex(carMakeIndex);
    fileModelCarMake->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    this->sPath = fileModelCarMake
            ->fileInfo(fileModelCarMake->index(globalPath+"/"+savedCarMake,0)).absoluteFilePath();
    ui->carModel->setModel(fileModelCarModel);
    this->ui->carModel->setRootIndex(fileModelCarModel->setRootPath(sPath));


    QString savedCarModel = categories.at(1);
    QModelIndex savedCarModelIndex = fileModelCarModel->index(sPath+"/"+savedCarModel,0);
    this->ui->carModel->setCurrentIndex(savedCarModelIndex);
    fileModelCarModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    this->modelPath = fileModelCarModel->fileInfo(savedCarModelIndex).absoluteFilePath();
    ui->detailCategory->setModel(fileModelDetailCategory);
    this->ui->detailCategory->setRootIndex(fileModelDetailCategory->setRootPath(modelPath));


    QString savedDetailCategory = categories.at(2);
    QModelIndex savedDetailCategoryIndex = fileModelDetailCategory->index(modelPath+"/"+savedDetailCategory,0);
    this->ui->detailCategory->setCurrentIndex(savedDetailCategoryIndex);
    fileModelDetailCategory->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    detailCategoryPath = fileModelDetailCategory->fileInfo(savedDetailCategoryIndex).absoluteFilePath();
    this->selectedDetailCategory = fileModelDetailCategory->fileInfo(savedDetailCategoryIndex).fileName();
    ui->detail->setModel(fileDetail);
    ui->detail->setRootIndex(fileDetail->setRootPath(detailCategoryPath));
    selectedDetail="";

    QString savedDetail = categories.at(3);
    QModelIndex savedDetailIndex = fileDetail->index(detailCategoryPath+"/"+savedDetail,0);
    this->ui->detail->setCurrentIndex(savedDetailIndex);
    fileDetail->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    detailPath = fileDetail->fileInfo(savedDetailIndex).absoluteFilePath();
    ui->detailArticle->setModel(fileDetailArticle);
    this->selectedDetail = fileDetail->fileInfo(savedDetailIndex).fileName();
    ui->detailArticle->setRootIndex(fileDetailArticle->setRootPath(detailPath));

    QString savedArticle = categories.at(4);
    QModelIndex articleModelIndex = fileDetailArticle->index(detailPath+"/"+savedArticle,0);
    ui->detailArticle->setCurrentIndex(articleModelIndex);
    fileDetailArticle->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    carDetailArticleChanged(articleModelIndex);
}

void MainWindow::on_pushButton_clicked()
{
    updateGallery();
}

void MainWindow::on_homeButton_clicked()
{
    currentGalleryPath = galleryPath;
    currentTmpPath = tmpPath;
    updateGallery();
}

void MainWindow::on_backButton_clicked()
{
    QDir dir1(currentGalleryPath);
    dir1.cdUp();
    currentGalleryPath = dir1.absolutePath();
    QDir dir2(currentTmpPath);
    dir2.cdUp();
    currentTmpPath = dir2.absolutePath();
    updateGallery();
}

void MainWindow::on_createArticleButton_clicked()
{
    beforCreateArticle();
}
