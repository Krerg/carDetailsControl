#ifndef FINDARTICLESWINDOW_H
#define FINDARTICLESWINDOW_H

#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>

class FindArticlesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FindArticlesWindow(QString path, QWidget *parent = 0);
    ~FindArticlesWindow();
private:
    QLineEdit* findLineEdit;
    QPushButton* findPushButton;
    QString path;
    QStringList containingWords;
    QStringList foundFiles;
    QListWidget* filesWidget;
    void keyPressEvent(QKeyEvent *event);
    void showFiles();
signals:
    articleActivated(QString*);
public slots:
    void findArticles();
    void fileItemActivated(QListWidgetItem * item);
};

#endif // FINDARTICLESWINDOW_H
