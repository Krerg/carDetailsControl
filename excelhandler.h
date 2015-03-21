#ifndef EXCELHANDLER_H
#define EXCELHANDLER_H

#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>

class ExcelHandler : public QWidget
{
    Q_OBJECT
public:
    explicit ExcelHandler(QString path,QWidget *parent = 0);
private:
    QLabel* outputFileLabel;
    QLineEdit* outputExcelFile;
    QPushButton* confirmButton;
    QString path;
    QProgressBar* pb;
signals:

public slots:
    void exportToExcel();
};

#endif // EXCELHANDLER_H
