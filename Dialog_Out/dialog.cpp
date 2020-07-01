#include "dialog.h"
#include "ui_dialog.h"
#include <QMenu>

Dialog::Dialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    timer = new QTimer();
    setting = new Setting();
    udpOut = new UdpOut();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    connect(ui->action, SIGNAL(triggered()), this, SLOT(settings()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::toStart(){
    timer->start(2000);
}

void Dialog::returnUdp(QString a){
    setPng(QPixmap(a));
    qDebug() << "1";
    //":Migalka/Images/circle_red.png"
    //setPng(QPixmap(":Migalka/Images/circle_red.png"));
}

void Dialog::setPng(QPixmap a){
    ui->label->setPixmap(a);
}

void Dialog::slotTimerAlarm(){
    udpOut->sendUdp(QByteArray("Продление сеанса"));
    //setPng(QPixmap(":Migalka/Images/circle_grey.png"));
}

void Dialog::closeEvent(QCloseEvent *event){
    udpOut->sendUdp(QByteArray("Завершение"));
    event->accept();
}

void Dialog::settings(){
    setting->show();
    setting->toStart();
}
