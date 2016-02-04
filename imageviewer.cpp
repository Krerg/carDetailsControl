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

    imgOriginalWidth = 0;
    imgOriginalHeight = 0;

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

    imgOriginalHeight = image.height();
    imgOriginalWidth = image.width();

    imageLabel->resize(this->windowWidth*k-24, this->windowHeight*k-24);

}

void ImageViewer::scaleImage(double scale)
{
    if(scale<0) {
       scale = 2+scale;
    }
    scaleFactor*=scale;
    if(scale*imageLabel->width()<windowWidth ||
       scale*imageLabel->width() > imgOriginalWidth||
       scale*imageLabel->height() > imgOriginalHeight) {
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

void ImageViewer::scaleImageToWindowSize()
{
   double ratio = double(imgOriginalWidth)/imgOriginalHeight;
   if(double(windowWidth)/windowHeight - ratio >= 0 )
      imageLabel->resize((windowHeight-24)*ratio,windowHeight-24);
   else
      imageLabel->resize(windowWidth-24,double(windowWidth-24)/ratio);
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

void ImageViewer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        scaleImageToWindowSize();
    }
}

void ImageViewer::zoomIn()
{

}

void ImageViewer::zoomOut()
{

}
