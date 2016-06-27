#include "excelhandler.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
#include "3rdparty/QtXlsxWriter-master/src/xlsx/xlsxdocument.h"
#include <QApplication>
#include "mainwindow.h"

ExcelHandler::ExcelHandler(QString path, QString pathToFiles, QString pathToSiteFiles, QWidget *parent) :
    QWidget(parent),pathToFiles(pathToFiles),pathToSiteFiles(pathToSiteFiles)
{
    this->path = path;
    QVBoxLayout* v = new QVBoxLayout(this);
    this->outputFileLabel = new QLabel("Выходной файл");
    this->outputExcelFile = new QLineEdit("output.xlsx");
    this->confirmButton = new QPushButton("Экспорт");
    this->exportNewButton = new QPushButton("Экспорт новых артикулов");

    this->pb = new QProgressBar();
    pb->setValue(0);
    pb->setMinimum(0);
    pb->setMaximum(0);
    pb->setVisible(false);
    v->addWidget(outputFileLabel);
    v->addWidget(outputExcelFile);
    v->addWidget(confirmButton);
    v->addWidget(exportNewButton);
    v->addWidget(pb);
    QObject::connect(this->confirmButton,SIGNAL(clicked()),this,SLOT(exportAllToExcel()));
    QObject::connect(this->exportNewButton,SIGNAL(clicked()),this,SLOT(exportNewToExcel()));

}

bool ExcelHandler::checkState(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadWrite |  QFile::Text))
    {
         QTextStream stream(&file);
         QString state;
         state = stream.readLine();
         state = stream.readLine();
         state = stream.readLine();
         state = stream.readLine();
         state = stream.readLine();
         state = stream.readLine();
         state = stream.readLine();
         state = stream.readLine();
         state = stream.readLine();
         state = stream.readLine();
          file.close();
         if(state.compare(MainWindow::NEW_STATE)) {
            return true;
         } else {
             return false;
         }
    } else {
        qDebug()<<"Невозможно открыть выходной файл";
        return false;
    }
}

QString ExcelHandler::setImgPath(QString imgPath)
{
    return (pathToSiteFiles == "") ? imgPath : pathToSiteFiles;
}

void ExcelHandler::exportAllToExcel()
{
    QFile outputFile(outputExcelFile->text());
    if(outputFile.exists()) {
        outputFile.remove();
    }
    QXlsx::Document excelFile(outputExcelFile->text());
    //шапка эксель файла
    excelFile.write("A1","IE_NAME");
    excelFile.write("B1","IE_PREVIEW_TEXT");
    excelFile.write("C1","IP_PROP18");
    excelFile.write("D1","IC_GROUP0");
    excelFile.write("E1","IC_GROUP1");
    excelFile.write("F1","IC_GROUP2");
    excelFile.write("G1","CV_PRICE_1");
    excelFile.write("H1","IE_DETAIL_PICTURE");
    excelFile.write("I1","IP_PROP23");
    excelFile.write("J1","IP_PROP26");
    excelFile.write("K1","IP_PROP32");

    //проверяем папку для выгрузки
    QDir imageOutDir(pathToFiles);
    if(!imageOutDir.exists()) {
        imageOutDir.mkpath(".");
    }

    pb->setVisible(true);
    QDir dir(path);
    QDir carModelsDir;
    QDir detailCategoriesDir;
    QDir detailsDir;
    QDir articlesDir;
    QDir imageDir;
    QString mainImage="";
    QStringList carMarks = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    int count = carMarks.size();
    int currentCount=0;
    int currentExcelRow=2;
    foreach(QString carMark,carMarks) {
        ++currentCount;
        pb->setValue((int)(((double)currentCount/count)*100));
        pb->update();
        carModelsDir.setPath(path+"/"+carMark);
        //detail category cycle
        foreach(QString carModel, carModelsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            detailCategoriesDir.setPath(carModelsDir.path()+"/"+carModel);
            foreach(QString detailCategory,detailCategoriesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
                detailsDir.setPath(detailCategoriesDir.path()+"/"+detailCategory);
                foreach(QString detail, detailsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
                    articlesDir.setPath(detailsDir.path()+"/"+detail);
                    foreach(QString article, articlesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
                        imageDir.setPath(articlesDir.path()+"/"+article);
                        if(!exportAll && checkState(imageDir.path()+"/"+article+".txt")) {
                            continue;
                        }
                        foreach(QString image,imageDir.entryList(QDir::Files | QDir::NoDotAndDotDot)) {
                            if(!image.contains(QRegExp("(\.txt)|(\[H\])"))) {
                                if(mainImage=="") {
                                    mainImage=image;
                                    QFile::copy(imageDir.path()+"/"+mainImage,imageOutDir.path()+"/"+mainImage);
                                    continue;
                                }

                                excelFile.write(QString("I%1").arg(currentExcelRow),setImgPath(imageDir.path())+"/"+image);
                                excelFile.write(QString("H%1").arg(currentExcelRow),setImgPath(imageDir.path())+"/"+mainImage);
                                excelFile.write(QString("A%1").arg(currentExcelRow),detail);
                                excelFile.write(QString("D%1").arg(currentExcelRow),carMark);
                                excelFile.write(QString("E%1").arg(currentExcelRow),carModel);
                                excelFile.write(QString("F%1").arg(currentExcelRow),detailCategory);
                                excelFile.write(QString("C%1").arg(currentExcelRow),article);
                                QFile desc(imageDir.path()+"/"+article+".txt");
                                if(desc.open(QIODevice::ReadOnly)) {
                                    QTextStream in(&desc);
                                    for(int i=0;i<5;i++) {
                                        in.readLine();
                                    }
                                    excelFile.write(QString("G%1").arg(currentExcelRow),in.readLine());
                                    //in.readLine();
                                    excelFile.write(QString("K%1").arg(currentExcelRow),in.readLine());
                                    excelFile.write(QString("J%1").arg(currentExcelRow),in.readLine());
                                    excelFile.write(QString("B%1").arg(currentExcelRow),in.readLine());
                                }
                                QFile::copy(imageDir.path()+"/"+image,imageOutDir.path()+"/"+image);
                                currentExcelRow++;
                            }
                            QApplication::processEvents();
                        }
                        mainImage="";
                    }
                }
            }
        }
    }
    excelFile.save();
    this->close();
    delete this;
}

void ExcelHandler::exportNewToExcel()
{
    exportAll = false;
    exportAllToExcel();
}
