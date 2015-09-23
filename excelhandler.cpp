#include "excelhandler.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
#include "3rdparty/QtXlsxWriter-master/src/xlsx/xlsxdocument.h"

ExcelHandler::ExcelHandler(QString path, QWidget *parent) :
    QWidget(parent)
{
    this->path = path;
    QVBoxLayout* v = new QVBoxLayout(this);
    this->outputFileLabel = new QLabel("Выходной файл");
    this->outputExcelFile = new QLineEdit("output.xlsx");
    this->confirmButton = new QPushButton("Экспорт");
    this->pb = new QProgressBar();
    pb->setValue(0);
    pb->setMinimum(0);
    pb->setMaximum(0);
    pb->setVisible(false);
    v->addWidget(outputFileLabel);
    v->addWidget(outputExcelFile);
    v->addWidget(confirmButton);
    v->addWidget(pb);
    QObject::connect(this->confirmButton,SIGNAL(clicked()),this,SLOT(exportToExcel()));
}

void ExcelHandler::exportToExcel()
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

//    excelFile.write("A2","Название запчасти");
//    excelFile.write("B2","Описание запчасти (примечания)");
//    excelFile.write("C2","Артикул");
//    excelFile.write("D2","Марка авто");
//    excelFile.write("E2","Модель");
//    excelFile.write("F2","Категория запчасти");
//    excelFile.write("G2","Цена");
//    excelFile.write("H2","Основная фотография");
//    excelFile.write("I2","Дополнительная фотография");

    pb->setVisible(true);
    QDir dir(path);
    QDir carModelsDir;
    QDir detailCategoriesDir;
    QDir detailsDir;
    QDir articlesDir;
    QDir imageDir;
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
                        foreach(QString image,imageDir.entryList(QDir::Files | QDir::NoDotAndDotDot)) {
                            qDebug()<<carMark<<" "<<carModel<<" "<<detailCategory<<" "<<detail<<" "<<article<<" "<<image<<endl;
                            if(image!="Описание.txt") {
                                QString j = QString("H%1").arg(currentExcelRow);
                                excelFile.write(QString("H%1").arg(currentExcelRow),imageDir.path()+"/"+image);
                                excelFile.write(QString("A%1").arg(currentExcelRow),detail);
                                excelFile.write(QString("D%1").arg(currentExcelRow),carMark);
                                excelFile.write(QString("E%1").arg(currentExcelRow),carModel);
                                excelFile.write(QString("F%1").arg(currentExcelRow),detailCategory);
                                excelFile.write(QString("C%1").arg(currentExcelRow),article);
                                QFile desc(imageDir.path()+"/Описание.txt");
                                if(desc.open(QIODevice::ReadOnly)) {
                                    QTextStream in(&desc);
                                    for(int i=0;i<5;i++) {
                                        in.readLine();
                                    }
                                    excelFile.write(QString("G%1").arg(currentExcelRow),in.readLine());
                                    in.readLine();
                                    in.readLine();
                                    excelFile.write(QString("B%1").arg(currentExcelRow),in.readLine());
                                }
                                currentExcelRow++;
                            }
                        }
                    }
                }
            }
        }
    }
    excelFile.save();
    this->close();
    delete this;
}