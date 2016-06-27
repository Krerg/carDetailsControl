#ifndef EXCELHANDLER_H
#define EXCELHANDLER_H

#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QDir>

/**
 * @brief The ExcelHandler class окно для работы с Excel
 */
class ExcelHandler : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief ExcelHandler
     * @param path путь к выходному эксель файлу
     * @param pathToFiles путь для выгрузки фотографий
     * @param parent родительский объект
     */
    explicit ExcelHandler(QString path,QString pathToFiles,QString pathToSiteFiles,QWidget *parent = 0);
private:
    //GUI элементы
    QLabel* outputFileLabel;
    QLineEdit* outputExcelFile;
    QPushButton* confirmButton;
    QPushButton* exportNewButton;
    QString path;
    QProgressBar* pb;
    QString pathToFiles;
    QString pathToSiteFiles;

    bool exportAll = true;

    bool checkState(QString path);
    QString setImgPath(QString);

signals:

public slots:
    /**
     * @brief exportAllToExcel экспорт в эксель всех артикулов
     */
    void exportAllToExcel();

    /**
     * @brief exportNewToExcel экспорт в эксель только новые артикулы
     */
    void exportNewToExcel();
};

#endif // EXCELHANDLER_H
