#include "updatingwindow.h"
#include <QVBoxLayout>

UpdatingWindow::UpdatingWindow(int maxValue, QString labelText, QWidget *parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    updateWindowLabel = new QLabel(labelText);
    updateWindowProgressBar = new QProgressBar();
    currentValue = 0;
    updateWindowProgressBar->setMaximum(maxValue);
    updateWindowProgressBar->setMinimum(0);
    updateWindowProgressBar->setValue(currentValue);

    mainLayout->addWidget(updateWindowLabel);
    mainLayout->addWidget(updateWindowProgressBar);

    setWindowFlags(Qt::CustomizeWindowHint);
}

void UpdatingWindow::increment()
{
    currentValue++;
    updateWindowProgressBar->setValue(currentValue);
    updateWindowProgressBar->update();
    updateWindowProgressBar->repaint();
}

void UpdatingWindow::closeUpdatingWindow()
{
    this->close();
    delete updateWindowLabel;
    delete updateWindowProgressBar;
    delete this;
}

void UpdatingWindow::update(int newValue)
{
    currentValue = newValue;
    updateWindowProgressBar->setValue(currentValue);
    updateWindowProgressBar->update();
}
