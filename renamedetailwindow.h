#ifndef RENAMEDETAILWINDOW_H
#define RENAMEDETAILWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "abstractrenamewindow.h"

class RenameDetailWindow : public AbstractRenameWindow
{
    Q_OBJECT
public:
    explicit RenameDetailWindow(QString path, QString selectedCategory, QString oldName, QMap<QString, QStringList*>* map, QWidget *parent = 0);

private:

    QLabel* detailInfoLabel;

    QString path;

    QString selectedCategory;

    QString oldName;

    QMap<QString, QStringList*>* map;

signals:

    void updateArticlesInfos();

public slots:

    void renameDetailSlot();

};

#endif // RENAMEDETAILWINDOW_H
