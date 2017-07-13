#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "updatingwindow.h"

#include <QWidget>
#include <QFileSystemModel>
#include <QMenuBar>
#include <QStringList>
#include <QTreeView>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QMap>
#include <QResizeEvent>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const QString NEW_STATE;
    static const QString EXPORTED_STATE;

protected:
    void keyPressEvent(QKeyEvent* event);

    void resizeEvent(QResizeEvent* e);

    void closeEvent(QCloseEvent* e);

//    void dropEvent(QDropEvent *e);

//    void dragEnterEvent(QDragEnterEvent *e);

signals:
    void closeArticleGalleryUpdateThread();

private:

    /**
     * @brief applySavedValues установка сохраненных значений
     */
    void applySavedValues();

    /**
     * @brief initWindowSize метод инициализации размера окна при старте
     */
    void initWindowSize();

    /**
     * @brief clearFileSystems очистка всех окон отвечающих за просмотр файловых систем
     */
    void clearFileSystems();

    /**
     * @brief clearArticleGallery очистка галереи артикула
     */
    void clearArticleGallery();

    /**
     * @brief updateDetails
     */
    void updateDetails();

    void updateDetailGallery(QString detailPath);

    void clearOutput();

    void updateAll();

    void removeItemFromGallery();

    void updateGallery();

    void updateArticleGalleryFileNames();

    void saveSizes(QTextStream* stream);

    /**
     * @brief saveSelectedValues сохранение выбранных значений
     */
    void saveSelectedValues();

    int saveSelectedValue(QModelIndexList list, QTextStream* out);

    void getDetailCategoriesList();

    /**
     * @brief showErrorWindow показывает окно с ошибкой
     * @param errorMessage текст ошибки
     */
    void showErrorWindow(QString errorMessage = "Возникла ошибка");

    Ui::MainWindow *ui;

    //UpdatingWindow* updatingWindow;

    QString globalPath;

    QString galleryPath;

    QString pathToSiteFiles;

    QString currentGalleryPath;

    QString tmpPath;

    QString currentTmpPath;

    /**
     * @brief pathTofiles путь для выгрузки файлов
     */
    bool movingArticlesFlag;

    QStringList movingArticlesList;

    QString pathTofiles;

    QString modelPath;

    QString detailPath;

    QString oldDetailPath;

    QString detailArticlePath;

    QFileSystemModel *fileModelCarMake;

    QFileSystemModel *fileModelCarModel;

    QFileSystemModel *fileModelDetailCategory;

    QTreeView* details;

    QFileSystemModel *fileDetail;

    QFileSystemModel *fileDetailArticle;

    QMenu* service;
    QMenu* servicePath;

    QMenuBar* menuBar;
    QMenuBar* disabledMenuBar;

    QAction* settings;

    QAction* importFromExcel;

    QAction* exportToExcel;

    QMenu* requestCarMakeMenu;

    bool editDetail;

    QMenu* requestCarModelMenu;

    QMenu* requestDetailCategoryMenu;

    QMenu* requestDetailMenu;

    QMenu* requestDetailArticleMenu;

    QMenu* requestGalleryMenu;

    bool isBackPushed;

    QMenu* requestDetailGalleryMenu;

    QAction* createCarMake;

    QAction* renameCarMake;

    QAction* deleteCarMake;

    QAction* createCarModel;

    QAction* renameCarModel;

    QAction* deleteCarModel;

    QAction* createDetailCategory;

    QAction* renameDetailCategory;

    QAction* deleteDetailCategory;

    QWidget* createCarMakeWindow;

    QAction* createDetail;

    QAction* renameDetail;

    QAction* deleteDetail;

    QAction* createArticle;

    QAction* deleteArticle;

    QAction* changeArticle;

    QAction* deleteImage;

    QAction* add2ExistArticle;

    QAction* renameImage;

    QAction* createImageFolder;

    QAction* returnImage;

    QAction* setMainImage;

    QAction* hideImage;

    QAction* showImage;

    QString sPath;

    QStringList* detailCategories;

    QMap<QString, QStringList*>* detailsMap;

    QString detailCategoryPath;

    QList<QListWidgetItem*>* images;

    QList<QListWidgetItem*>* articleImages;
    QList<QModelIndex>* history;

    /**
     * @brief imageSize размер изображений
     */
    int imageSize;

    QString selectedDetailCategory;

    QString selectedDetail;

    /**
     * @brief settingsFile файл настроек, где лежат пути
     */
    QFile* settingsFile;

    /**
     * @brief savedIndexes файл где лежат сохраненные значения выбранных деталей
     */
    QFile* savedIndexes;

    QModelIndex* returnFirstChildIndex(const QString &path, QFileSystemModel *&model);
    //QList<Integer> *list;
    QFileSystemModel* filemodel;

    int articleCount;


    bool isUpdateDetailGalleryFinished;
    bool isUpdateGalleryFinished;
    void beforCreateArticle();
    void cancelCreateArticle();

public slots:
    void carMakeChanged(QModelIndex);
    void carModelChanged(QModelIndex);
    void carDetailCategoryChanged(QModelIndex);
    void carDetailChanged(QModelIndex);
    void carDetailArticleChanged(QModelIndex);

    void selectRowSlot(QString);

    void openImage(QModelIndex);
    void selectAndOpen(QModelIndex);
    void selectFromListToGallery(QModelIndex);
    void selectFromGalleryToList(QModelIndex);
    void openArticleImage(QModelIndex);

    void menuRequestCarMake(QPoint pos);
    void menuRequestCarModel(QPoint pos);
    void menuRequsetDetailCategory(QPoint pos);
    void menuRequestDetail(QPoint pos);
    void menuRequestDetailArticle(QPoint pos);
    void menuRequestGallery(QPoint pos);
    void menuRequsetDetailGallery(QPoint pos);

    void createCarMakeSlot();
    void renameCarMakeSlot();
    void deleteCarMakeSlot();
    void afterDeleteCarMakeSlot();

    void createCarModelSlot();
    void renameCarModelSlot();
    void deleteCarModelSlot();
    void afterDeleteCarModelSlot();

    void createDetailCategorySlot();
    void renameDetailCategorySlot();
    void deleteDetailCategorySlot();
    void afterDeleteDetailCategorySlot();

    void createDetailSlot();
    void renameDetailSlot();
    void deleteDetailSlot();
    void afterDeleteDetailSlot();

    void createArticleSlot();
    void deleteArticleSlot();
    void changeArticleSlot();

    void returnImageSlot();
    void setMainImageSlot();
    void deleteImageSlot();
    void deleteImageSlotSlot();
    void add2ExistArticleSlot();
    void hideImageSlot();
    void showImageSlot();
    void renameImageSlot();

    void openSettingsWindow();
    void setSettings(QString path, QString galleryPath,
                     QString pathTofiles,QString pathToSiteFiles,
                     int imageSize);

    void exportToExcelSlot();
    void importFromExcelSlot();

    void updateArticlesInfosFiles();
    void updateArticlesOutput(QString newName);
    void updateGallerySlot(QListWidgetItem* item);
    void updateArticleGallerySlot(QListWidgetItem* item);
    void updateGalleryFinishedSlot();
    void updateArticleGalleryFinishedSlot();
    void setArticleCountSlot(int count);

    void findeArticlesSlot();
    void setArticleSlot(QString *path);
private slots:
    void on_pushButton_clicked();
    void on_homeButton_clicked();
    void on_backButton_clicked();
    void on_createArticleButton_clicked();
};

#endif // MAINWINDOW_H
