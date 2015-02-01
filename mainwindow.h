#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFileSystemModel>
#include <QMenuBar>
#include <QStringList>
#include <QTreeView>

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

    void getDetailCategoriesList();

    Ui::MainWindow *ui;

    QString globalPath;

    QString modelPath;

    QFileSystemModel *fileModelCarMake;

    QFileSystemModel *fileModelCarModel;

    QFileSystemModel *fileModelDetailCategory;

    QTreeView* details;

    QFileSystemModel *fileDetail;

    QMenu* service;

    QMenuBar* menuBar;

    QAction* settings;

    QAction* importToExcel;

    QMenu* requestCarMakeMenu;

    QMenu* requestCarModelMenu;

    QMenu* requestDetailCategoryMenu;

    QMenu* requestDetailMenu;

    QAction* createCarMake;

    QAction* deleteCarMake;

    QAction* createCarModel;

    QAction* deleteCarModel;

    QAction* createDetailCategory;

    QAction* deleteDetailCategory;

    QWidget* createCarMakeWindow;

    QAction* createDetail;

    QAction* deleteDetail;

    QString sPath;

    QStringList* detailCategories;

    QString detailPath;
public slots:
    void carMakeChanged(QModelIndex);
    void carModelChanged(QModelIndex);
    void carDetailCategoryChanged(QModelIndex);

    void menuRequestCarMake(QPoint pos);
    void menuRequestCarModel(QPoint pos);
    void menuRequsetDetailCategory(QPoint pos);
    void menuRequestDetail(QPoint pos);

    void createCarMakeSlot();
    void deleteCarMakeSlot();

    void createCarModelSlot();
    void deleteCarModelSlot();

    void createDetailCategorySlot();
    void deleteDetailCategorySlot();

    void createDetailSlot();
    void deleteDetailSlot();

    void openSettingsWindow();
    void setGlobalPath(QString path);
};

#endif // MAINWINDOW_H
