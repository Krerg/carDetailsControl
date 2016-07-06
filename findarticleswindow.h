#ifndef FINDARTICLESWINDOW_H
#define FINDARTICLESWINDOW_H

#include <QLineEdit>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>

class FindArticlesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FindArticlesWindow(QString path, int count, QWidget *parent = 0);
    ~FindArticlesWindow();
private:
    QLineEdit* findLineEdit;
    QPushButton* findPushButton;
    QString path;
    QStringList containingWords;
    QStringList foundFiles;
    QListWidget* filesWidget;
    QProgressBar *progressBar;
    int count;
    void keyPressEvent(QKeyEvent *event);
    void showFiles();
signals:
    void articleActivated(QString*);
public slots:
    void findArticles();
    void fileItemActivated(QListWidgetItem * item);
};

#endif // FINDARTICLESWINDOW_H
