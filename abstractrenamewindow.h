#ifndef ABSTRACTRENAMEWINDOW_H
#define ABSTRACTRENAMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QKeyEvent>

class AbstractRenameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractRenameWindow(QWidget *parent = 0);

protected:

    QLineEdit* newName;

    QPushButton* proceedButton;

    void keyPressEvent(QKeyEvent* event);

signals:

public slots:

};

#endif // ABSTRACTRENAMEWINDOW_H
