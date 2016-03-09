#include "abstractrenamewindow.h"

AbstractRenameWindow::AbstractRenameWindow(QWidget *parent) :
    QWidget(parent)
{
    proceedButton = new QPushButton();
    newName = new QLineEdit();
}

void AbstractRenameWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==16777220) {
        if(proceedButton->hasFocus()) {
            proceedButton->click();
        } else {
            proceedButton->setFocus();
        }
    }
}
