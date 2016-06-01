#include "movingariclesdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

MovingAriclesDialog::MovingAriclesDialog(QString oldPath, QString newPath, QString articles)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QPushButton* pcmdOk = new QPushButton("&Да");
    QPushButton* pcmdCancel = new QPushButton("&Нет");
    QLabel* label = new QLabel("Перенести артикулы:\n"+articles+"\nИз:\n"+oldPath+"\nВ:\n"+newPath);
    QGridLayout* grid = new QGridLayout();
    grid->addWidget(label,0,0,1,2);
    grid->addWidget(pcmdOk,1,0);
    grid->addWidget(pcmdCancel,1,1);
    this->setLayout(grid);

    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));
}

MovingAriclesDialog::~MovingAriclesDialog()
{

}

