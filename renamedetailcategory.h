#ifndef RENAMEDETAILCATEGORY_H
#define RENAMEDETAILCATEGORY_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "abstractrenamewindow.h"

class RenameDetailCategory : public AbstractRenameWindow
{
    Q_OBJECT
public:
    explicit RenameDetailCategory(QString path, QString oldName,QMap<QString, QStringList*>* map, QWidget *parent = 0);

private:

    QLabel* detailCategoryLabel;

    QString path;

    QString oldName;

    QMap<QString, QStringList*>* map;

signals:
    void updateArticlesInfos();
public slots:
    void renameDetailCategorySlot();
};

#endif // RENAMEDETAILCATEGORY_H
