#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFileSystemModel>
#include <QMenuBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    QString globalPath;

    QFileSystemModel *fileModelCarMake;

    QFileSystemModel *fileModelCarModel;

    QFileSystemModel *fileModelDetailCategory;

    QMenu* service;

    QMenuBar* menuBar;

    QAction* settings;

    QAction* importToExcel;

    QMenu* requestCarMakeMenu;

    QMenu* requestCarModelMenu;

    QMenu* requestDetailCategoryMenu;

    QAction* createCarMake;

    QAction* deleteCarMake;

    QAction* createCarModel;

    QAction* deleteCarModel;

    QAction* createDetailCategory;

    QAction* deleteDetailCategory;

    QWidget* createCarMakeWindow;

    QString sPath;
public slots:
    void carMakeChanged(QModelIndex);
    void carModelChanged(QModelIndex);

    void menuRequestCarMake(QPoint pos);
    void menuRequestCarModel(QPoint pos);
    void menuRequsetDetailCategory(QPoint pos);

    void createCarMakeSlot();
    void deleteCarMakeSlot();

    void createCarModelSlot();
    void deleteCarModelSlot();

    void createDetailCategorySlot();
    void deleteDetailCategorySlot();
};

#endif // MAINWINDOW_H
