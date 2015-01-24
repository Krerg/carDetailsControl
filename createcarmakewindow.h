#ifndef CREATECARMAKEWINDOW_H
#define CREATECARMAKEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class CreateCarMakeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CreateCarMakeWindow(QWidget *parent = 0);
    void setPath(QString path);
    ~CreateCarMakeWindow();
private:
    QLineEdit* carMakeName;
    QPushButton* createButton;
    QString path;
signals:

public slots:
    void createCarMake();
};

#endif // CREATECARMAKEWINDOW_H
