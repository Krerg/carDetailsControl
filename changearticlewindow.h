#ifndef CHANGEARTICLEWINDOW_H
#define CHANGEARTICLEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class ChangeArticleWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChangeArticleWindow(QString path, QString article, QWidget *parent = 0);

private:

    QString articleName;

    QString path;

    QLineEdit* make;

    QLineEdit* model;

    QLineEdit* category;

    QLineEdit* detail;

    QLineEdit* articleOutput;

    QLineEdit* cost;

    QLineEdit* originalArticle;

    QLineEdit* place;

    QLineEdit* note;

    QPushButton* proceedButton;


signals:

public slots:

    void changeArticle();

};

#endif // CHANGEARTICLEWINDOW_H
