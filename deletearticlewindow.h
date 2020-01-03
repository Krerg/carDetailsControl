#ifndef DELETEARTICLEWINDOW_H
#define DELETEARTICLEWINDOW_H

#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class DeleteArticleWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DeleteArticleWindow(QWidget *parent = 0);

    void setPath(QString path);

private:

    QString article;

    QString path;

    QLabel* msg;

    QPushButton* confirmButton;

    QPushButton* cancelButton;

    QProgressBar* pb;

signals:

public slots:

    void deleteArticle();

};

#endif // DELETEARTICLEWINDOW_H
