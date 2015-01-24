#ifndef CONFIRMWINDOW_H
#define CONFIRMWINDOW_H

#include <QObject>
#include <QLabel>
#include <QPushButton>

class ConfirmWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ConfirmWindow(QWidget *parent = 0);
    void setPath(QString path);
private:
    QString path;
    QLabel* confirmText;
    QPushButton* okButton;
    QPushButton* cancelButton;
signals:
    void confirm();
public slots:
    void cancelAction();
    void confirmAction();
};

#endif // CONFIRMWINDOW_H
