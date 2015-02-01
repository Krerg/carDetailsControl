#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWindow(QString globalPath,QWidget *parent = 0);
private:
    QString globalPath;
    QLineEdit* globalPathEdit;
    QPushButton* confirmButton;
signals:
    void setSettings(QString globalPath);
public slots:
    void confirm();
};

#endif // SETTINGSWINDOW_H
