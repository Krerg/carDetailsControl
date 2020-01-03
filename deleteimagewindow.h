#ifndef DELETEIMAGEWINDOW_H
#define DELETEIMAGEWINDOW_H

#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class DeleteImageWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DeleteImageWindow(QWidget *parent = 0);

    void setPath(QString path);

private:

    QString Image;

    QString path;

    QLabel* msg;

    QPushButton* confirmButton;

    QPushButton* cancelButton;

    QProgressBar* pb;

signals:

public slots:

    void deleteImage();

   signals:
    void updateGallery();
};

#endif // DELETEImageWINDOW_H
