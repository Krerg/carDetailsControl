#ifndef RENAMEDETAILWINDOW_H
#define RENAMEDETAILWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class RenameDetailWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RenameDetailWindow(QString path, QString selectedCategory, QString oldName, QMap<QString, QStringList*>* map, QWidget *parent = 0);

private:

    QLabel* detailInfoLabel;

    QLineEdit* detailNameInput;

    QPushButton* proceedButton;

    QString path;

    QString selectedCategory;

    QString oldName;

    QMap<QString, QStringList*>* map;

    void keyPressEvent(QKeyEvent* event);

signals:

    void updateArticlesInfos();

public slots:

    void renameDetailSlot();

};

#endif // RENAMEDETAILWINDOW_H
