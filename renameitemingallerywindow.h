#ifndef RENAMEITEMINGALLERYWINDOW_H
#define RENAMEITEMINGALLERYWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include "abstractrenamewindow.h"

class RenameItemInGalleryWindow : public AbstractRenameWindow
{
    Q_OBJECT

public:
    explicit RenameItemInGalleryWindow(QString globalPath, QString oldName, QWidget *parent = 0) ;

private:

    QLabel* newNameLabel;

    QString path;

    QString oldName;

signals:
    void updateGallerySignal();
public slots:
    void renameGalleryItemSlot();


public slots:

};

#endif // RENAMEITEMINGALLERYWINDOW_H
