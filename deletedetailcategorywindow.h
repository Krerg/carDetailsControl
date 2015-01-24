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

    void setCategoryName(QString name);

    void setPath(QString path);

private:
    QString categoryName;

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
