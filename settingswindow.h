#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWindow(QString globalPath,QString galleryPath, QWidget *parent = 0);
private:
    QString globalPath;
    QString galleryPath;
    QLabel *globalPathEditLabel;
    QLineEdit* globalPathEdit;
    QLabel* imageGalleryEditLabel;
    QLineEdit* imageGalleryEdit;
    QPushButton* confirmButton;
signals:
    void setSettings(QString globalPath, QString galleryPath);
public slots:
    void confirm();
};

#endif // SETTINGSWINDOW_H
