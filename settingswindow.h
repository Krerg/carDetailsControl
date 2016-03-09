#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>

class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWindow(QString globalPath,QString galleryPath,QString pathToFiles, int imageSize, QWidget *parent = 0);
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
    QPushButton* selectDirectoryBtn1;
    QPushButton* selectDirectoryBtn2;
    QPushButton* selectDirectoryBtn3;

    /**
     * @brief imageSizeLabel люлбл для изменения размера изображений
     */
    QLabel* imageSizeLabel;

    /**
     * @brief imageSizeSpinBox компонент для изменения размера изображений в галерее
     */
    QSpinBox* imageSizeSpinBox;
signals:
    void setSettings(QString globalPath, QString galleryPath, QString pathTofiles, int imageSize);
public slots:
    void confirm();
    void selectDirectoryBtnClickedSlot();
};

#endif // SETTINGSWINDOW_H
