#ifndef DELETEDETAILWINDOW_H
#define DELETEDETAILWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QPushButton>
#include <QProgressBar>

class DeleteDetailWindow : public QWidget
{
    Q_OBJECT
public:

    explicit DeleteDetailWindow(QWidget *parent = 0);

    void setParameters(QString detailName,QString path, QString detailCategory,QMap<QString, QStringList*>* map);

private:
    QLabel* msg;

    QMap<QString, QStringList*>* map;

    QString detailCategory;

    QString detailName;

    QString path;

    QPushButton* confirmButton;

    QProgressBar* pb;
signals:

public slots:
    void deleteDetail();
};

#endif // DELETEDETAILWINDOW_H
