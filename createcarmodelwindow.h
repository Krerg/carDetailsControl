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

    void setPath(QString path);

    void setCategoriesList(QStringList* j);
private:
    QLineEdit* carModelName;

    QString path;

    QStringList* categories;

    QPushButton* createButton;

    QPushButton* cancelButton;
signals:

public slots:
    void createCarModel();
};

#endif // CREATECARMODELWINDOW_H
