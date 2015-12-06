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
    explicit SettingsWindow(QString globalPath,QString galleryPath,QString pathToFiles, QWidget *parent = 0);
    ~SettingsWindow();
private:
    QString globalPath;
    QString galleryPath;
    QLabel *globalPathEditLabel;
    QLineEdit* globalPathEdit;
    QLabel* imageGalleryEditLabel;
    QLabel* pathToFilesLabel;
    QLineEdit* imageGalleryEdit;
    QPushButton* confirmButton;
    QLineEdit* pathToFiles;
signals:
    void setSettings(QString globalPath, QString galleryPath, QString pathTofiles);
public slots:
    void confirm();
};

#endif // SETTINGSWINDOW_H
