#include "changearticlewindow.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>

ChangeArticleWindow::ChangeArticleWindow(QString path, QString article, QWidget *parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->path = path;
    this->articleName = article;
    this->make = new QLineEdit();
    this->model = new QLineEdit();
    this->category = new QLineEdit();
    this->detail = new QLineEdit();
    this->articleOutput = new QLineEdit();
    this->cost = new QLineEdit();
    this->originalArticle = new QLineEdit();
    this->place = new QLineEdit();
    this->note = new QLineEdit();
    this->proceedButton = new QPushButton("Изменить");
    mainLayout->addWidget(make);
    mainLayout->addWidget(model);
    mainLayout->addWidget(category);
    mainLayout->addWidget(detail);
    mainLayout->addWidget(articleOutput);
    mainLayout->addWidget(cost);
    mainLayout->addWidget(originalArticle);
    mainLayout->addWidget(place);
    mainLayout->addWidget(note);
    mainLayout->addWidget(proceedButton);

    QObject::connect(proceedButton,SIGNAL(clicked()),this,SLOT(changeArticle()));

    QFile describe(path+"/"+article+".txt");
    if (describe.open(QIODevice::ReadOnly))
    {
        QTextStream in(&describe);
        QString temp = in.readLine();
        this->make->setText(temp);
        temp = in.readLine();
        this->model->setText(temp);
        temp = in.readLine();
        this->category->setText(temp);
        temp = in.readLine();
        this->detail->setText(temp);
        temp = in.readLine();
        this->articleOutput->setText(temp);
        temp = in.readLine();
        this->cost->setText(temp);
        temp = in.readLine();
        this->originalArticle->setText(temp);
        temp = in.readLine();
        this->place->setText(temp);
        temp = in.readLine();
        this->note->setText(temp);
    }
    this->show();
}

void ChangeArticleWindow::changeArticle()
{
    QFile file(path+"/"+articleName+".txt");
    file.remove();
    if(file.open(QIODevice::ReadWrite | QFile::Append | QFile::Text))
    {
         QTextStream stream(&file);
         stream<<this->make->text() <<endl;
         stream<<this->model->text() <<endl;
         stream<<this->category->text() <<endl;
         stream<<this->detail->text() <<endl;
         stream<<this->articleOutput->text() <<endl;
         stream<<this->cost->text() <<endl;
         stream<<this->originalArticle->text() <<endl;
         stream<<this->place->text() <<endl;
         stream<<this->note->text() <<endl;
    }
    this->close();
    delete this;
}
