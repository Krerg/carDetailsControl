#ifndef CREATECARMODELWINDOW_H
#define CREATECARMODELWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

class CreateCarModelWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CreateCarModelWindow(QWidget *parent = 0);

    void setPath(QString path);

    void setCategoriesList(QMap<QString, QStringList*>* map);
private:
    QLineEdit* carModelName;

    QString path;

    QMap<QString, QStringList*>* detailsMap;

    QPushButton* createButton;

    QPushButton* cancelButton;

    void keyPressEvent(QKeyEvent *event);

signals:

public slots:
    void createCarModel();
};

#endif // CREATECARMODELWINDOW_H
