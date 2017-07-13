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
    this->outputExcelFile = new QLineEdit("_output.xlsx");
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
    excelFile.write("A1","IE_NAME");//detail
    excelFile.write("B1","IE_PREVIEW_TEXT");//preview
    excelFile.write("C1","IP_PROP18");//article
    excelFile.write("D1","IC_GROUP0");//carMark
    excelFile.write("E1","IC_GROUP1");//carModel
    excelFile.write("F1","IC_GROUP2");//detailcategory
    excelFile.write("G1","CV_PRICE_1");//цена
    excelFile.write("H1","IE_DETAIL_PICTURE");//mainImage
    excelFile.write("I1","IP_PROP23");//image
    excelFile.write("J1","IP_PROP26");//место на складе
    excelFile.write("K1","IP_PROP32");//оригинальный артикул


    excelFile.write("L1","IE_DETAIL_TEXT");
    excelFile.write("M1","meta_title");
    excelFile.write("N1","meta_keywords");
    excelFile.write("O1","meta_description");
    excelFile.write("P1","IE_XML_ID");
    excelFile.write("Q1", "comment");//комментарий


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
                        bool isWritten=false;
                        foreach(QString image,imageDir.entryList(QDir::Files | QDir::NoDotAndDotDot)) {
                            QRegExp *re= new QRegExp();
                            re->setPattern(".*(\.(txt|jpg|png|gif|bmp))");
                            if(re->exactMatch(image.toLower())){//|(\[H\])
                                qDebug()<<imageDir.path()<<"<<"<<image;
                                if(!isWritten){
                                    QFile desc(imageDir.path()+"/"+article+".txt");
                                    QString original;
                                    if(desc.open(QIODevice::ReadOnly)) {
                                        QTextStream in(&desc);
                                        for(int i=0;i<5;i++) {
                                            in.readLine();
                                        }
                                        excelFile.write(QString("G%1").arg(currentExcelRow),in.readLine());//price
                                        //in.readLine();
                                        original = in.readLine();
                                        excelFile.write(QString("K%1").arg(currentExcelRow),original);//оригинальный артикул
                                        excelFile.write(QString("J%1").arg(currentExcelRow),in.readLine());//место на складе
                                        excelFile.write(QString("Q%1").arg(currentExcelRow),in.readLine());//заметка

                                    }
                                    excelFile.write(QString("A%1").arg(currentExcelRow),detail);
                                    excelFile.write(QString("D%1").arg(currentExcelRow),carMark);
                                    excelFile.write(QString("E%1").arg(currentExcelRow),carModel);
                                    excelFile.write(QString("F%1").arg(currentExcelRow),detailCategory);
                                    excelFile.write(QString("C%1").arg(currentExcelRow),article);
                                    excelFile.write(QString("P%1").arg(currentExcelRow),article);
                                    //добавление мета-информации
                                    //meta-title
                                    excelFile.write(QString("B%1").arg(currentExcelRow),detail+" "+carMark+" "+carModel);//preview
                                    excelFile.write(QString("L%1").arg(currentExcelRow),detail+" для "+carMark+" "+carModel);//IE_DETAIL_TEXT
                                    excelFile.write(QString("N%1").arg(currentExcelRow),detail+","+
                                                    article+","+carMark+","+carModel+","+detailCategory+","+original);//meta_keywords
                                    excelFile.write(QString("O%1").arg(currentExcelRow),detail+" "+
                                                    article+" "+carMark+" "+carModel+" "+detailCategory+" "+original);//meta_description

                                    excelFile.write(QString("M%1").arg(currentExcelRow),detail+" "+original
                                                    +" для "+carMark+" " +carModel);//meta_title

                                    isWritten=true;
                                }
                                if(!image.contains(".txt")){
                                    if(mainImage=="") {

                                        mainImage=image;
                                        QFile::copy(imageDir.path()+"/"+mainImage,imageOutDir.path()+"/"+mainImage);
                                        excelFile.write(QString("H%1").arg(currentExcelRow),setImgPath(imageDir.path())+"/"+mainImage);
                                        continue;
                                    }
                                    excelFile.write(QString("H%1").arg(currentExcelRow),setImgPath(imageDir.path())+"/"+mainImage);
                                    excelFile.write(QString("I%1").arg(currentExcelRow),setImgPath(imageDir.path())+"/"+image);
                                    isWritten=false;
                                    currentExcelRow++;
                                }
                                QFile::copy(imageDir.path()+"/"+image,imageOutDir.path()+"/"+image);
                            }

                            QApplication::processEvents();
                        }
                        if(isWritten){
                            currentExcelRow++;
                        }
                        mainImage="";
                    }
                }
            }
        }
    }
    exportAll=true;
    excelFile.save();
    this->close();
    delete this;
}

void ExcelHandler::exportNewToExcel()
{
    exportAll = false;
    exportAllToExcel();
}
