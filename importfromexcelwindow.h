#ifndef IMPORTFROMEXCELWINDOW_H
#define IMPORTFROMEXCELWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

/**
 * @brief The ImportFromExcelWindow class окно для обработки загрузки данных из Excel файла
 */
class ImportFromExcelWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ImportFromExcelWindow(QString path, QMap<QString, QStringList*>* detailsMap, QWidget *parent = 0);
private:

    /**
     * @brief proceedButton кнопка для загрузки из файла
     */
    QPushButton* proceedButton;

    /**
     * @brief chooseFileButton кнопка для открытия диалога выбора файла
     */
    QPushButton* chooseCarModelsFileButton;

    /**
     * @brief choosedFile надпись с названием файла
     */
    QLabel* choosedCarModelsFile;

    /**
     * @brief chooseFileButton кнопка для открытия диалога выбора файла
     */
    QPushButton* chooseDetailsFileButton;

    /**
     * @brief choosedFile надпись с названием раздел-деталь файла
     */
    QLabel* choosedDetailsFile;

    /**
     * @brief filePath путь к файлу марка-модель
     */
    QString carModelsFilePath;

    /**
     * @brief detailsFilePath путь к файлу раздел-деталь
     */
    QString detailsFilePath;

    QString path;

    QMap<QString, QStringList*>* detailsMap;

signals:

public slots:

    void chooseCarModelsFile();

    void chooseDetailFilePath();

    void loadFromExcel();
    QString reformatString(QString);

};

#endif // IMPORTFROMEXCELWINDOW_H
