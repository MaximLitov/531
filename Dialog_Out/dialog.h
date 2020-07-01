#ifndef DIALOG_H
#define DIALOG_H

#include <setting.h>
#include <QMainWindow>
#include <QTimer>
#include <QCloseEvent>
#include <udpout.h>

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
    void toStart();
    void returnUdp(QString a);
    void setPng(QPixmap a);
    void closeEvent(QCloseEvent *event);

private slots:
    void slotTimerAlarm();
    void settings();

private:
    Ui::Dialog *ui;
    QTimer *timer;
    Setting *setting;
    UdpOut *udpOut;
};

#endif // DIALOG_H
