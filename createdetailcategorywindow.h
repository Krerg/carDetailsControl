#ifndef CREATEDETAILCATEGORYWINDOW_H
#define CREATEDETAILCATEGORYWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>

class CreateDetailCategoryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDetailCategoryWindow(QWidget *parent = 0);

    void setPath(QString path);

private:
    QLineEdit* categoryName;

    QPushButton* createButton;

    QString path;

    QProgressBar *pb;
signals:

public slots:
    void createDetailCategory();
};

#endif // CREATEDETAILCATEGORYWINDOW_H
