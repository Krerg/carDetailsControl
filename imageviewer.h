#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QWheelEvent>

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
     * @brief imageLabel лэйбл для вывода изображения.
     */
    QLabel* imageLabel;

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

protected:

    /**
     * @brief wheelEvent ивент прокручивания колесика
     * @param event
     */
    void wheelEvent(QWheelEvent* event);

    bool eventFilter(QObject *, QEvent *);

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
