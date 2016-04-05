#ifndef CREATEDETAILWINDOW_H
#define CREATEDETAILWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QProgressBar>

class CreateDetailWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDetailWindow(QWidget *parent = 0);
    void setParameters(QString path, QMap<QString, QStringList*>* map, QString category);
private:
    QString path;

    QMap<QString, QStringList*>* map;

    QLineEdit* detailName;

    QPushButton* confirmButton;

    QProgressBar *pb;

    QString selectedCategory;

    void keyPressEvent(QKeyEvent *event);
signals:

public slots:
    void createDetail();
};

#endif // CREATEDETAILWINDOW_H
