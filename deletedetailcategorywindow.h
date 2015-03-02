#ifndef DELETEDETAILCATEGORYWINDOW_H
#define DELETEDETAILCATEGORYWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class DeleteDetailCategoryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DeleteDetailCategoryWindow(QWidget *parent = 0);

    void setParameters(QString path,QMap<QString, QStringList*>* map, QString categoryName);

private:
    QString categoryName;

    QMap<QString, QStringList*>* map;

    QString path;

    QLabel* msg;

    QPushButton* confirmButton;

    QPushButton* cancelButton;

    QProgressBar* pb;

signals:

public slots:
    void deleteCategory();

    void cancelAction();
};

#endif // DELETEDETAILCATEGORYWINDOW_H
