#ifndef DIALOG_H
#define DIALOG_H

#include "setting.h"
#include "udp.h"
#include "core.h"
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
    void toStart(Udp *a, Core *b);
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *event);

private slots:
    void settings();
    void otvUdp(TypeSending type);
    void con(bool a);

private:
    Ui::Dialog *ui;
    Setting *setting;
    Udp *udp;
    QStandardItemModel *model;
    Core *core;
};

#endif // DIALOG_H
