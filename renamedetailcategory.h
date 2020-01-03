#ifndef RENAMEDETAILCATEGORY_H
#define RENAMEDETAILCATEGORY_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class RenameDetailCategory : public QWidget
{
    Q_OBJECT
public:
    explicit RenameDetailCategory(QString path, QString oldName,QMap<QString, QStringList*>* map, QWidget *parent = 0);

private:

    QLabel* detailCategoryLabel;

    QLineEdit* detailCategoryInput;

    QPushButton* proceedButton;

    QString path;

    QString oldName;

    QMap<QString, QStringList*>* map;

    void keyPressEvent(QKeyEvent *event);

signals:
    void updateArticlesInfos();
public slots:
    void renameDetailCategorySlot();
};

#endif // RENAMEDETAILCATEGORY_H
