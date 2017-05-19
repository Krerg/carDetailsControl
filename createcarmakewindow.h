#ifndef CREATECARMAKEWINDOW_H
#define CREATECARMAKEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>

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
    void keyPressEvent(QKeyEvent *event);
signals:

public slots:
    void createCarMake();

signals:
void selectRowSignal(QString);
};

#endif // CREATECARMAKEWINDOW_H
