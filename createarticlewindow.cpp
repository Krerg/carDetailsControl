#include "createarticlewindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QFileInfo>

CreateArticleWindow::CreateArticleWindow(QWidget *parent) :
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

void CreateArticleWindow::setPath(QString path)
{
    this->path = path;
}

void CreateArticleWindow::CreateDetail()
{
   QDir dir(path+"/"+article->text());
   if(!dir.exists())
   {
        dir.mkpath(".");
   }
    QFile f(path+"/"+article->text()+"/"+"Описание.txt");
   if(f.open(QIODevice::ReadWrite | QFile::Append | QFile::Text))
   {
        QTextStream stream(&f);
        stream << article->text()<< endl;
        stream << note->text()<< endl;
        stream << originalArticle->text() << endl;
        stream << cost->text() + "RUB" << endl;
        stream << place->text() << endl;
   }
   if(files.size()>0)
   {
       int k=0;
       QList<QString>::iterator i;
       for(i=files.begin();i!=files.end();i++)
       {
           QFile f((*i));
           QFileInfo *d = new QFileInfo(f);
           QString h = path+"/"+article->text()+"/"+d->fileName();
           bool b = QFile::copy((*i),h);
       }

   }
   this->hide();
   delete this;\
}

void CreateArticleWindow::openFileDialog()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec())
        this->files = dialog.selectedFiles();
}
