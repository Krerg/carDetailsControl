#include "createdetailwindow.h"
#include <QVBoxLayout>
#include <QPushButton>

CreateDetailWindow::CreateDetailWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* v = new QVBoxLayout(this);
    this->article = new QLineEdit("Введите артикул");
    this->confirmButton = new QPushButton("Добавить деталь");
    this->note = new QLineEdit("Введите заметку");
    this->originalArticle = new QLineEdit("Введите оригинальный артикул");
    this->cost = new QLineEdit("Введите стоимость");
    this->place = new QLineEdit("Укажите место на складе");
    this->chooseFiles = new QPushButton("Выбрать файлы");

    v->addWidget(article);
    v->addWidget(note);
    v->addWidget(originalArticle);
    v->addWidget(cost);
    v->addWidget(place);
    v->addWidget(chooseFiles);
    v->addWidget(confirmButton);
    QObject::connect(confirmButton,SIGNAL(clicked()),this,SLOT(CreateDetail()));
    QObject::connect(chooseFiles,SIGNAL(clicked()),this,SLOT(openFileDialog()));
}

void CreateDetailWindow::setPath(QString path)
{
    this->path = path;
}

void CreateDetailWindow::CreateDetail()
{
   this->hide();
   delete this;
}

void CreateDetailWindow::openFileDialog()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec())
        this->files = dialog.selectedFiles();
}
