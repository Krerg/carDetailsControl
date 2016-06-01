#ifndef MOVINGARICLESDIALOG_H
#define MOVINGARICLESDIALOG_H

#include <QDialog>



class MovingAriclesDialog : public QDialog
{
public:
    MovingAriclesDialog(QString oldPath, QString newPath, QString articles);
    ~MovingAriclesDialog();
};

#endif // MOVINGARICLESDIALOG_H
