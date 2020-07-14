#ifndef SETTING_H
#define SETTING_H

#include "udp.h"
#include "core.h"
#include <QMainWindow>

namespace Ui {
class Setting;
}

class Setting : public QMainWindow
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

private:
    int moveLogs;

public:
    void toStart(Udp *a, Core *b);

private slots:
    void on_pushButton_clicked();
    void on_checkBox_4_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Setting *ui;
    Udp *udp;
    Core *core;
};

#endif // SETTING_H
