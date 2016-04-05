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

    void setParameters(QString path,QMap<QString, QStringList*>* map);

private:
    QLineEdit* categoryName;

    QPushButton* createButton;

    QString path;

    QProgressBar *pb;

    QMap<QString, QStringList*>* map;

    void keyPressEvent(QKeyEvent *event);
signals:

public slots:
    void createDetailCategory();
};

#endif // CREATEDETAILCATEGORYWINDOW_H
