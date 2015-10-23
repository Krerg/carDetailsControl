#include "imageviewer.h"
#include <QImage>
#include <QMessageBox>
#include <QLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QEvent>
#include <QScrollBar>

ImageViewer::ImageViewer(QString pathToFile, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);

    imageLabel = new QLabel(this);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea(this);
    scrollArea->viewport()->installEventFilter(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    //this->setCentralWidget(scrollArea);

    layout()->addWidget(scrollArea);

    setWindowTitle(tr("ImageViewer"));
    resize(500, 400);

    if(!open(pathToFile)) {
        this->close();
    }
    scrollArea->viewport()->installEventFilter(this);
}

bool ImageViewer::open(QString pathToFile)
{
    QImage image(pathToFile);
    if (image.isNull()) {
        QMessageBox::information(this, tr("Image Viewer"),
            tr("Cannot load %1.").arg(pathToFile));
        return false;
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));
    imageLabel->adjustSize();
}

void ImageViewer::scaleImage(double scale)
{
    if(scale<0) {
       scale = 2+scale;
    }
    scaleFactor*=scale;
    imageLabel->resize(scaleFactor*imageLabel->pixmap()->size());
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{

}

bool ImageViewer::eventFilter(QObject *obj, QEvent *e)
{

    if(e->type()==QEvent::Wheel) {
        if(((QWheelEvent*) e)->modifiers().testFlag(Qt::ControlModifier)) {
            qDebug()<<"sfdf";
            scaleImage((double)((QWheelEvent*) e)->delta()/110);
            return true;
        } else {
            return QObject::eventFilter(obj, e);
        }
    }
    return QObject::eventFilter(obj, e);
}

void ImageViewer::zoomIn()
{

}

void ImageViewer::zoomOut()
{

}
