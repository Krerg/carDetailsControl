#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QWheelEvent>
#include <math.h>

/**
 * @brief The ImageViewer class кастомный просмотрщик изображений
 */
class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief ImageViewer публичный конструктор
     * @param pathToFile путь к изображению
     * @param parent родительский объект
     */
    explicit ImageViewer(QString pathToFile, QWidget *parent = 0);

private:

    /**
     * @brief k коэффициент
     */
    double k = sqrt((double)2/3);

    /**
     * @brief imageLabel лэйбл для вывода изображения.
     */
    QLabel* imageLabel;

    /**
     * @brief imgOriginalWidth изначальная ширина изображения.
     */

    int imgOriginalWidth;

    /**
     * @brief imgOriginalHeight изначальная высота изображения.
     */

    int imgOriginalHeight;

    /**
     * @brief scrollArea для прокручивания изображения.
     */
    QScrollArea* scrollArea;

    bool open(QString pathToFile);

    /**
     * @brief scaleImage изменяет масштаб картинки.
     * @param scale коэффициент масштаба
     */
    void scaleImage(double scale);

    /**
     * @brief scaleImage изменяет масштаб картинки.
     * @param w
     * @param h
     */
    void scaleImage(int w, int h);

    /**
     * @brief scaleImageToWindowSize изменяет масштаб картинки до размеров окна.
     */

    void scaleImageToWindowSize();

    /**
     * @brief scaleFactor текущий коэффициент масштаба.
     */
    double scaleFactor=1;

    /**
     * @brief adjustScrollBar сохранения позиции скрол бара
     *  при масштабировании
     * @param scrollBar сам скролл бар
     * @param factor коэффициент масштабирования
     */
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    /**
     * @brief windowWidth ширина окна (при изменении запоминается)
     */
    static int windowWidth;

    /**
     * @brief wondowHeight высота окна (при изменении запоминается)
     */
    static int windowHeight;

    ~ImageViewer();

protected:

    /**
     * @brief wheelEvent ивент прокручивания колесика
     * @param event
     */
    void wheelEvent(QWheelEvent* event);

    bool eventFilter(QObject *, QEvent *);

    /**
     * @brief resizeEvent ивент изменения размера окна
     */
    void resizeEvent(QResizeEvent *);

    /**
     * @brief keyPressEvent ивент изменения нажатия клавиши
     */
    void keyPressEvent(QKeyEvent* event);

signals:

public slots:

private slots:

    /**
     * @brief zoomIn слот для увеличения изображения
     */
    void zoomIn();

    /**
     * @brief zoomOut слот для уменьшения изображения
     */
    void zoomOut();

};

#endif // IMAGEVIEWER_H
