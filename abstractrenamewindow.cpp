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
        if(newName->hasFocus()) {
            proceedButton->click();
        } else if(!proceedButton->hasFocus()) {
            newName->setFocus();
        }
    }
}
