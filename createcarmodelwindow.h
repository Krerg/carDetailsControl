#ifndef CREATECARMODELWINDOW_H
#define CREATECARMODELWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QpushButton>
#include <QComboBox>

class CreateCarModelWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CreateCarModelWindow(QWidget *parent = 0);
private:
    QLineEdit* carModelName;

    QString path;

    QComboBox* carMake;

    QPushButton* createButton;
signals:

public slots:

};

#endif // CREATECARMODELWINDOW_H
