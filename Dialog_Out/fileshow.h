#ifndef FILESHOW_H
#define FILESHOW_H

#include "udp.h"
#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class FileShow;
}

class FileShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileShow(QWidget *parent = 0);
    ~FileShow();

public:
    void toStart(Udp *a, QString b);

private:
    QString file;
    Ui::FileShow *ui;
    Udp *udp;
    QStandardItemModel *model;
};

#endif // FILESHOW_H
