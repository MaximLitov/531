#ifndef DIALOG_H
#define DIALOG_H

#include "setting.h"
#include "udp.h"
#include <QMainWindow>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QStandardItemModel>

namespace Ui {
class Dialog;
}

class Dialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public:
    void toStart(Udp *a);
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *event);

private slots:
    void settings();
    void lampa(int a);
    void otvet(QByteArray a);

private:
    Ui::Dialog *ui;
    Setting *setting;
    Udp *udp;
    QStandardItemModel *model;
};

#endif // DIALOG_H
