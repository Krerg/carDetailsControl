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
#include "excelhandler.h"
#include "imageviewer.h"
#include "importfromexcelwindow.h"

const QString MainWindow::NEW_STATE = "NEW";

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute( Qt::WA_DeleteOnClose );
    this->ui->gallery->setIconSize(QSize(5,5));
    this->selectedDetailCategory = "";
    this->editDetail = false;
    this->detailCategories = new QStringList();
    this->detailsMap = new QMap<QString, QStringList*>();
    this->details = new QTreeView();
    this->images = new QList<QListWidgetItem*>();
    this->articleImages = new QList<QListWidgetItem*>();

    initWindowSize();

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
    QObject::connect(returnImage,SIGNAL(triggered()),this,SLOT(returnImageSlot()));
    QObject::connect(setMainImage,SIGNAL(triggered()),this,SLOT(setMainImageSlot()));

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

    ui->gallery->setIconSize(QSize(120,120));


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
        this->globalPath = in.readLine();
        this->galleryPath = in.readLine();
        this->pathTofiles = in.readLine();
        this->imageSize = in.readLine().toInt();
    } else {
        this->globalPath = "c:/carShop/";
        this->galleryPath = "c:/gallery/";
        this->imageSize = 120;
        QDir globalPathDir(globalPath);
        if (!globalPathDir.exists()) {
            globalPathDir.mkpath(".");
        }
        QDir galleryPathDir(galleryPath);
        if (!galleryPathDir.exists()) {
            galleryPathDir.mkpath(".");
        }
        settingsFile->open(QIODevice::ReadWrite | QFile::Append | QFile::Text);
        QTextStream out(settingsFile);
        out << globalPath << endl;
        out << galleryPath << endl;
    }
    this->setSettings(globalPath,galleryPath,pathTofiles,imageSize);

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
    QModelIndex* tmpIndex = returnFirstChildIndex(sPath, fileModelCarModel);
    if(tmpIndex) {
        ui->carModel->setCurrentIndex(*tmpIndex);
        carModelChanged(*tmpIndex);
    } else {
        clearOutput();
        delete fileModelDetailCategory;
        fileModelDetailCategory = new QFileSystemModel(this);
        delete fileDetail;
        fileDetail = new QFileSystemModel(this);
        delete fileDetailArticle;
        fileDetailArticle = new QFileSystemModel(this);
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
        delete fileDetail;
        fileDetail = new QFileSystemModel(this);
        delete fileDetailArticle;
        fileDetailArticle = new QFileSystemModel(this);
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
        delete fileDetailArticle;
        fileDetailArticle = new QFileSystemModel(this);
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
    } else
        clearOutput();
}

void MainWindow::openImage(QModelIndex t)
{
    QString path = t.data().toString();
    ImageViewer* iv = new ImageViewer(galleryPath+"/"+path);
    iv->show();
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
    if(ui->articleGallery->indexAt(pos).row()>=0)
    {
        requestDetailGalleryMenu->addAction(returnImage);
        requestDetailGalleryMenu->addAction(setMainImage);
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
    for(i=files.begin();i!=files.end();i++)
    {
        QFile f(detailArticlePath+"/"+(*i)->text());
        QFileInfo *d = new QFileInfo(f);
        bool b = QFile::copy(detailArticlePath+"/"+d->fileName(),galleryPath+"/"+d->fileName());

        QIcon* j = new QIcon(galleryPath+"/"+d->fileName());
        QListWidgetItem* g = new QListWidgetItem((j->pixmap(QSize(120,120))),d->fileName());
        this->images->append(g);
        g->setSizeHint(QSize(120,120));
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
        QFile::rename(detailArticlePath+"/"+ this->ui->articleOutput->text()+"_1."+splittedFileName.at(1),
                      detailArticlePath+"/"+selectedItemName);
        QFile::rename(detailArticlePath+"/tmpName.tmp",
                      detailArticlePath+"/"+ this->ui->articleOutput->text()+"_1."+splittedFileName.at(1));
        updateDetailGallery(detailArticlePath);
    }
}

void MainWindow::deleteImageSlot()
{
    QList<QListWidgetItem*> files = this->ui->gallery->selectedItems();
    QList<QListWidgetItem*>::Iterator i;
    for(i=files.begin();i!=files.end();i++)
    {
        QFile f(galleryPath+"/"+(*i)->text());
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
    QString path = detailPath+"/"+this->ui->articleOutput->text();
    QDir tmpDir(path);
    QFileInfoList articleFiles = tmpDir.entryInfoList();
    QString lastFileName = articleFiles.last().fileName();
    QRegExp rx("(\\_|\\.)");
    QStringList splittedFileName = lastFileName.split(rx);
    int imageIndex = splittedFileName.at(1).toInt();
    imageIndex++;
    for(i=files.begin();i!=files.end();i++)
    {
        QFile f(galleryPath+"/"+(*i)->text());
        QFileInfo *d = new QFileInfo(f);
        QString h = detailPath+"/"+this->ui->articleOutput->text()+"/"+this->ui->articleOutput->text()+"_"+QString::number(imageIndex)+"."+d->suffix();
        bool b = QFile::copy(galleryPath+"/"+(*i)->text(),h);
        qDebug()<<"Copying file "+galleryPath+"/"+(*i)->text()+" into "+h+" is "+b;
        imageIndex++;
    }
    removeItemFromGallery();
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
}

MainWindow::~MainWindow()
{
    detailsMap->clear();
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

void MainWindow::openSettingsWindow()
{
    SettingsWindow* w = new SettingsWindow(globalPath,galleryPath,pathTofiles, imageSize);
    QObject::connect(w,SIGNAL(setSettings(QString,QString,QString,int)),this,SLOT(setSettings(QString,QString,QString,int)));
    w->show();
}

void MainWindow::setSettings(QString path, QString galleryPath, QString pathToFiles, int imageSize)
{
    this->globalPath = path;
    this->galleryPath = galleryPath;
    this->pathTofiles = pathToFiles;
    this->imageSize = imageSize;
    this->settingsFile->remove();
    settingsFile->open(QIODevice::ReadWrite | QFile::Append | QFile::Text);
    QTextStream out(settingsFile);
    out << this->globalPath << endl;
    out << this->galleryPath << endl;
    out << this->pathTofiles << endl;
    out << this->imageSize << endl;
    this->updateAll();
    out.flush();
    settingsFile->close();
}

void MainWindow::updateGallery()
{
    QString regex = "*";
    QList<QListWidgetItem*> galleryItems = this->ui->gallery->findItems(regex,Qt::MatchWrap | Qt::MatchWildcard);
    this->ui->gallery->clear();
    this->ui->gallery->setUpdatesEnabled(false);
    this->ui->gallery->setUniformItemSizes(true);
    QDir dir(galleryPath);
    QStringList images = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);

    UpdatingWindow* uw = new UpdatingWindow(images.size(),"Загрузка");
    uw->show();

    QList<QString>::iterator i;
    QIcon* j;
    for(i=images.begin();i!=images.end();i++)
    {
        uw->increment();
        QApplication::processEvents();
        j = new QIcon(galleryPath+"/"+(*i));
        QListWidgetItem* g = new QListWidgetItem((j->pixmap(QSize(120,120))),(*i));
        this->images->append(g);
        g->setSizeHint(QSize(120,120));
        this->ui->gallery->addItem(g);
        delete j;

    }
    this->ui->gallery->setUpdatesEnabled(true);

    uw->closeUpdatingWindow();
}

void MainWindow::updateArticleGalleryFileNames()
{
    clearArticleGallery();

    int index = 1;
    QDir path(detailArticlePath);
    QFileInfoList articleFiles = path.entryInfoList(QDir::Files);
    QFileInfoList::iterator itr;
    for(itr=articleFiles.begin(); itr!= articleFiles.end(); ++itr)
    {
        if((*itr).suffix() == "txt")
            continue;
        QString oldName = detailArticlePath+"/"+ (*itr).fileName();
        QString newName = detailArticlePath+"/"+this->ui->articleOutput->text()+
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

void MainWindow::removeItemFromGallery()
{
    QList<QListWidgetItem*> files = this->ui->gallery->selectedItems();
    QList<QListWidgetItem*>::Iterator i;
    for(i=files.begin();i!=files.end();i++)
    {
        QFile f(galleryPath+"/"+(*i)->text());
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
                    QDir dir(detailPath+"/"+this->ui->articleOutput->text());
                    if(!dir.exists())
                    {
                         dir.mkpath(".");
                    } else {
                        showErrorWindow("Такой артикул уже есть");
                        this->editDetail = false;
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
                        QFile f(galleryPath+"/"+(*i)->text());
                        QFileInfo *d = new QFileInfo(f);
                        QString h = detailPath+"/"+this->ui->articleOutput->text()+"/"+this->ui->articleOutput->text()+"_"+QString::number(imageNumber)+"."+d->suffix();
                        bool b = QFile::copy(galleryPath+"/"+(*i)->text(),h);
                        QFile::remove(galleryPath+"/"+(*i)->text());
                        this->ui->gallery->removeItemWidget((*i));
                        delete (*i);
                        delete d;
                        imageNumber++;
                    }
                }
                this->editDetail = false;

                //установка курсора но созданную деталь
                QModelIndex k = fileDetailArticle->index(detailPath+"/"+this->ui->articleOutput->text(),0);
                this->ui->detailArticle->setCurrentIndex(k);
                this->updateDetailGallery(detailPath+"/"+this->ui->articleOutput->text());
                this->detailArticlePath = detailPath+"/"+this->ui->articleOutput->text();
            }
        }
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

void MainWindow::applySavedValues()
{
    this->savedIndexes->open(QIODevice::ReadWrite |  QFile::Text);
    QTextStream stream(savedIndexes);
    if(!stream.atEnd()) {
        QString savedCarMake = stream.readLine();
        QModelIndex carMakeIndex = fileModelCarMake->index(globalPath+"/"+savedCarMake,0);
        this->ui->carMake->setCurrentIndex(carMakeIndex);
        carMakeChanged(fileModelCarMake->index(globalPath+"/"+savedCarMake,0));
    }

    if(!stream.atEnd()) {
        QString savedCarModel = stream.readLine();
        QModelIndex savedCarModelIndex = fileModelCarModel->index(sPath+"/"+savedCarModel,0);
        this->ui->carModel->setCurrentIndex(savedCarModelIndex);
        carModelChanged(savedCarModelIndex);
    }

    if(!stream.atEnd()) {
        QString savedDetailCategory = stream.readLine();
        QModelIndex savedDetailCategoryIndex = fileModelDetailCategory->index(modelPath+"/"+savedDetailCategory,0);
        this->ui->detailCategory->setCurrentIndex(savedDetailCategoryIndex);
        carDetailCategoryChanged(savedDetailCategoryIndex);
    }

    if(!stream.atEnd()) {
        QString savedDetail = stream.readLine();
        QModelIndex savedDetailIndex = fileDetail->index(detailCategoryPath+"/"+savedDetail,0);
        this->ui->detail->setCurrentIndex(savedDetailIndex);
        carDetailChanged(savedDetailIndex);
    }

    if(!stream.atEnd()) {
        QString savedArticle = stream.readLine();
        QModelIndex articleModelIndex = fileDetailArticle->index(detailPath+"/"+savedArticle,0);
        ui->detailArticle->setCurrentIndex(articleModelIndex);
        carDetailArticleChanged(articleModelIndex);
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
    this->ui->articleGallery->clear();
    this->ui->articleGallery->setUpdatesEnabled(false);
    QDir dir(detailPath+"/");
    QStringList images = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    QList<QString>::iterator i;
    QIcon* j;
    for(i=images.begin();i!=images.end();i++)
    {
        j = new QIcon(detailPath+"/"+(*i));
        QListWidgetItem* g = new QListWidgetItem((j->pixmap(QSize(80,80))),(*i));
        this->articleImages->append(g);
        g->setSizeHint(QSize(80,80));
        this->ui->articleGallery->addItem(g);
        delete j;
    }
    this->ui->articleGallery->setUpdatesEnabled(true);
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
    ExcelHandler* excleHandlerWindow = new ExcelHandler(this->globalPath,this->pathTofiles);
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
    carDetailArticleChanged(index);
    updateArticleGalleryFileNames();
}

void MainWindow::on_pushButton_clicked()
{
    updateGallery();
}
