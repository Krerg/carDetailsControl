#ifndef CREATEDETAILWINDOW_H
#define CREATEDETAILWINDOW_H

#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QFileDialog>
#include <QStringList>

class CreateDetailWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDetailWindow(QWidget *parent = 0);

    void setPath(QString path);

    QLineEdit* article;

    QLineEdit* note;

    QLineEdit* originalArticle;

    QLineEdit* cost;

    QLineEdit* place;

    QFileDialog* fileChooser;

    QPushButton* confirmButton;

    QPushButton* chooseFiles;
private:
    QString path;

    QStringList files;
signals:

public slots:
    void openFileDialog();
    void CreateDetail();
};

#endif // CREATEDETAILWINDOW_H
