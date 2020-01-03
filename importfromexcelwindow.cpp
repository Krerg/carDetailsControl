#include "importfromexcelwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include "3rdparty/QtXlsxWriter-master/src/xlsx/xlsxdocument.h"


ImportFromExcelWindow::ImportFromExcelWindow(QString path, QMap<QString, QStringList *> *detailsMap, QWidget *parent)
{
    this->path = path;
    this->detailsMap = detailsMap;

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* loadCarModelsFileLayout = new QHBoxLayout();
    QHBoxLayout* loadDetailsFileLayout = new QHBoxLayout();
    mainLayout->addLayout(loadCarModelsFileLayout);
    mainLayout->addLayout(loadDetailsFileLayout);

    choosedCarModelsFile = new QLabel("Файл структуры не выбран");
    chooseCarModelsFileButton = new QPushButton("Выбрать файл");

    choosedDetailsFile = new QLabel("Файл Разделы-Детали не выбран");
    chooseDetailsFileButton = new QPushButton("Выбрать файл");

    loadCarModelsFileLayout->addWidget(choosedCarModelsFile);
    loadCarModelsFileLayout->addWidget(chooseCarModelsFileButton);

    //loadDetailsFileLayout->addWidget(choosedDetailsFile);
    //loadDetailsFileLayout->addWidget(chooseDetailsFileButton);

    proceedButton = new QPushButton("Загрузить данные");
    mainLayout->addWidget(proceedButton);

    QObject::connect(chooseCarModelsFileButton,SIGNAL(clicked()),this,SLOT(chooseCarModelsFile()));
    QObject::connect(chooseDetailsFileButton,SIGNAL(clicked()),this,SLOT(chooseDetailFilePath()));
    QObject::connect(proceedButton,SIGNAL(clicked()),this,SLOT(loadFromExcel()));
}

void ImportFromExcelWindow::chooseCarModelsFile()
{
    QFileDialog fileDialog;
    QFile f(fileDialog.getOpenFileName());
    QFileInfo fi(f);
    carModelsFilePath = fi.absoluteFilePath();
    if(fi.fileName()!="") {
        choosedCarModelsFile->setText(fi.fileName());
    }
}

void ImportFromExcelWindow::chooseDetailFilePath()
{
    QFileDialog fileDialog;
    QFile f1(fileDialog.getOpenFileName());
    QFileInfo fi1(f1);
    detailsFilePath = fi1.absoluteFilePath();
    if(fi1.fileName()!="") {
        choosedDetailsFile->setText(fi1.fileName());
    }
}

QString ImportFromExcelWindow::reformatString(QString s){
    return s.simplified().replace("\\", "-").replace("/", "-");
}

void ImportFromExcelWindow::loadFromExcel()
{
    if(carModelsFilePath=="") {
        return;
    }

    //очищаем папки
    QDir dir(path);
    QStringList folders = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    foreach (QString folder, folders) {
        QDir tmpDir(path+"/"+folder);
        tmpDir.removeRecursively();
    }

    QStringList* tempDetailList;
    QStringList::Iterator listIterator;
    QMap<QString, QStringList*>::const_iterator i = detailsMap->begin();
    while(i != detailsMap->constEnd()) {
        tempDetailList = i.value();
        delete tempDetailList;
        ++i;
    }
    detailsMap->clear();

//    QXlsx::Document detailsExcelFile(detailsFilePath);
    QXlsx::Document carModelsExcelFile(carModelsFilePath);
    QString detailCategory;
    QString detail;
    int row = 1;
    while(true) {

        detailCategory = reformatString(carModelsExcelFile.read(row,3).toString());
        if(detailCategory=="") {
            break;
        }
        if(!detailsMap->contains(detailCategory)) {
            detailsMap->insert(detailCategory, new QStringList());
        }

        detail = reformatString(carModelsExcelFile.read(row,4).toString());
        detailsMap->value(detailCategory)->append(detail);

        row++;
    }

    //QXlsx::Document carModelsExcelFile(carModelsFilePath);
    row=1;
    QString carMake;
    QString carModel;

    while(true) {
        carMake = reformatString(carModelsExcelFile.read(row,1).toString());
        if(carMake=="") {
            break;
        }

        QDir tmpDir(path+"/"+carMake);
        if(!tmpDir.exists()) {
            tmpDir.mkpath(".");
        }

        carModel = reformatString(carModelsExcelFile.read(row,2).toString());
        QDir tmpDir2(path+"/"+carMake+"/"+carModel);
        if(!tmpDir2.exists()) {
            tmpDir2.mkpath(".");
        }

        QStringList* tempDetailList;
        QStringList::Iterator listIterator;
        QDir temp;
        QMap<QString, QStringList*>::const_iterator i = detailsMap->begin();
        while(i != detailsMap->constEnd()) {
            dir.setPath(this->path+"/"+carMake+"/"+carModel+"/"+i.key());
            if (!dir.exists()) {
                dir.mkpath(".");
            }
            tempDetailList = i.value();
            for(listIterator = tempDetailList->begin();listIterator!=tempDetailList->end();listIterator++) {
                temp.setPath(dir.path()+"/"+(*listIterator));
                if(!temp.exists()) {
                    temp.mkpath(".");
                }
            }
            ++i;
        }
        row++;
    }
    this->close();
    delete this;
}


