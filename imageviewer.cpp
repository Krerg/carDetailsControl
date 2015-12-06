#include "imageviewer.h"
#include <QImage>
#include <QMessageBox>
#include <QLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QEvent>
#include <QScrollBar>
#include <QApplication>
#include <QDesktopWidget>
#include <math.h>

int ImageViewer::windowWidth = 400;

int ImageViewer::windowHeight = 500;

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

    //определение размера монитора  подгон окна под его размер
    QRect rec = QApplication::desktop()->screenGeometry();
    int height = rec.height();
    int width = rec.width();

    this->windowHeight = height;
    this->windowWidth = width;

    resize(width*k,height*k);

    if(!open(pathToFile)) {
        this->close();
    }
    this->setAttribute( Qt::WA_DeleteOnClose );
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
    imageLabel->resize(this->windowWidth*k-24, this->windowHeight*k-24);
}

void ImageViewer::scaleImage(double scale)
{
    if(scale<0) {
       scale = 2+scale;
    }
    scaleFactor*=scale;
    if(scale*imageLabel->width()<windowWidth) {
        return;
    }
    imageLabel->resize(scale*imageLabel->width(),scale*imageLabel->height());
}

void ImageViewer::scaleImage(int w, int h)
{
    if(w<0 || h<0) {
        qDebug()<<"Incorrect scale values";
        return;
    }
    imageLabel->resize(w,h);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

ImageViewer::~ImageViewer()
{
    qDebug()<<"Destroy!";
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{

}

bool ImageViewer::eventFilter(QObject *obj, QEvent *e)
{

    if(e->type()==QEvent::Wheel) {
        if(((QWheelEvent*) e)->modifiers().testFlag(Qt::ControlModifier)) {
            scaleImage((double)((QWheelEvent*) e)->delta()/110);
            return true;
        } else {
            return QObject::eventFilter(obj, e);
        }
    }
    return QObject::eventFilter(obj, e);
}

void ImageViewer::resizeEvent(QResizeEvent *e)
{
    this->windowHeight = this->height();
    this->windowWidth = this->width();
}

void ImageViewer::zoomIn()
{

}

void ImageViewer::zoomOut()
{

}
