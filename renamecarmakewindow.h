#ifndef RENAMECARMAKEWINDOW_H
#define RENAMECARMAKEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class RenameCarMakeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RenameCarMakeWindow(QString globalPath, QString oldName, QWidget *parent = 0);

private:

    QLabel* carMakeNameLabel;

    QLineEdit* carMakeNameInput;

    QPushButton* proceedButton;

    QString path;

    QString oldName;

signals:
    void updateArticlesInfos();
public slots:
    void renameCarMakeSlot();
};

#endif // RENAMECARMAKEWINDOW_H
