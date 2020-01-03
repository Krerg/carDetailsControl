#ifndef RENAMECARMAKEWINDOW_H
#define RENAMECARMAKEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "abstractrenamewindow.h"

class RenameCarMakeWindow : public AbstractRenameWindow
{
    Q_OBJECT
public:
    explicit RenameCarMakeWindow(QString globalPath, QString oldName, QWidget *parent = 0);

private:

    QLabel* carMakeNameLabel;

    QString path;

    QString oldName;

signals:
    void updateArticlesInfos();
public slots:
    void renameCarMakeSlot();
};

#endif // RENAMECARMAKEWINDOW_H
