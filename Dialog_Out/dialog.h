#ifndef DIALOG_H
#define DIALOG_H

#include "setting.h"
#include "udp.h"
#include <QMainWindow>
#include <QCloseEvent>

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
    void closeEvent(QCloseEvent *event);

private slots:
    void settings();
    void lampa(int a);

private:
    Ui::Dialog *ui;
    Setting *setting;
    Udp *udp;
};

#endif // DIALOG_H
