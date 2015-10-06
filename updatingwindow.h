#ifndef UPDATINGWINDOW_H
#define UPDATINGWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>

class UpdatingWindow : public QWidget
{
    Q_OBJECT
public:
    explicit UpdatingWindow(int maxValue, QString labelText = "Загрузка", QWidget *parent = 0);

private:

    int currentValue;

    QLabel* updateWindowLabel;

    QProgressBar* updateWindowProgressBar;

signals:

public slots:

    void increment();

    void closeUpdatingWindow();

    void update(int newValue);

};

#endif // UPDATINGWINDOW_H
