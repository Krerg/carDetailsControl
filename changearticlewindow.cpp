#include "changearticlewindow.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QGridLayout>
#include <QLabel>
#include <QDir>
#include <QDebug>
ChangeArticleWindow::ChangeArticleWindow(QString path, QString article, QWidget *parent)
{
    //QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QGridLayout* mainLayout = new QGridLayout(this);
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

    QLabel* markLabel = new QLabel("Марка",this);
    markLabel->setAlignment(Qt::AlignRight);
    QLabel* modelLabel = new QLabel("Модель",this);
    modelLabel->setAlignment(Qt::AlignRight);
    QLabel* categoryLabel = new QLabel("Раздел",this);
    categoryLabel->setAlignment(Qt::AlignRight);
    QLabel* detailLabel = new QLabel("Деталь",this);
    detailLabel->setAlignment(Qt::AlignRight);
    QLabel* articleOutputLabel = new QLabel("Артикул",this);
    articleOutputLabel->setAlignment(Qt::AlignRight);
    QLabel* costLabel = new QLabel("Цена",this);
    costLabel->setAlignment(Qt::AlignRight);
    QLabel* originalArticleLabel = new QLabel("Артикул оригинала",this);
    originalArticleLabel->setAlignment(Qt::AlignRight);
    QLabel* placeLabel = new QLabel("Место на складе",this);
    placeLabel->setAlignment(Qt::AlignRight);
    QLabel* noteLabel = new QLabel("Заметка",this);
    noteLabel->setAlignment(Qt::AlignRight);



    mainLayout->addWidget(markLabel,0,0);
    mainLayout->addWidget(make,0,1);
    mainLayout->addWidget(modelLabel,1,0);
    mainLayout->addWidget(model,1,1);
    mainLayout->addWidget(categoryLabel,2,0);
    mainLayout->addWidget(category,2,1);
    mainLayout->addWidget(detailLabel,3,0);
    mainLayout->addWidget(detail,3,1);
    mainLayout->addWidget(articleOutputLabel,4,0);
    mainLayout->addWidget(articleOutput,4,1);
    mainLayout->addWidget(costLabel,5,0);
    mainLayout->addWidget(cost,5,1);
    mainLayout->addWidget(originalArticleLabel,6,0);
    mainLayout->addWidget(originalArticle, 6,1);
    mainLayout->addWidget(placeLabel,7,0);
    mainLayout->addWidget(place,7,1);
    mainLayout->addWidget(noteLabel,8,0);
    mainLayout->addWidget(note,8,1);
    mainLayout->addWidget(proceedButton,9,1);

    mainLayout->setColumnStretch(1,10);


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

void ChangeArticleWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==16777220) {
        if(proceedButton->hasFocus()) {
            proceedButton->click();
        } else {
            proceedButton->setFocus();
        }
    }
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
         file.close();
    }
    file.rename(path+"/"+articleOutput->text()+".txt");


    QDir dir;
    QString oldName = path;
    QString newName= path;
    newName.remove(QRegExp("[^\/]+$"));
    newName += articleOutput->text();
    bool sucessful = dir.rename(oldName, newName);
    qDebug()<<"Renaiming "<<oldName<<" into "<< newName <<" is "<<sucessful;
    emit changed(newName);
    this->close();
    delete this;
}
