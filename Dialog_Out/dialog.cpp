#include "dialog.h"
#include "ui_dialog.h"
#include <QMenu>
#include "udp.h"

Dialog::Dialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setting = new Setting();
    model = new QStandardItemModel();
    connect(ui->action, SIGNAL(triggered()), this, SLOT(settings()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::toStart(Udp *a, Core *b){
    udp = a;
    core = b;
    connect(udp, SIGNAL(otvUdp(TypeSending)), this, SLOT(otvUdp(TypeSending)));
    connect(core, SIGNAL(isConnect(bool)), this, SLOT(con(bool)));
    fs = new FileShow();
}

void Dialog::otvUdp(TypeSending type){
    if (type == SEND_END){
        ui->label->setPixmap(QPixmap(":Migalka/Images/circle_red.png"));
        core->returnConnect();
    } else if (type == SEND_OTVET){
        QStandardItem *item;
        item = new QStandardItem("Ответ:");
        model->appendRow(item);
        QStringList a = QString(udp->getArray()).split("\n");
        for (int i = 0; i < a.size(); i++){
            item = new QStandardItem(QString(a[i]).trimmed());
            model->appendRow(item);
        }
        ui->listView->setModel(model);
    }
}

void Dialog::con(bool a){
    if (a) {
        ui->label->setPixmap(QPixmap(":Migalka/Images/circle_green.png"));
    } else {
        ui->label->setPixmap(QPixmap(":Migalka/Images/circle_red.png"));
    }
}

void Dialog::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
        if (ui->lineEdit->hasFocus() && ui->lineEdit->text().trimmed() != ""){
            QStandardItem *item;
            item = new QStandardItem("Команда: " + ui->lineEdit->text().trimmed());
            model->appendRow(item);
            ui->listView->setModel(model);
            QByteArray ar;
            QByteArray arr;
            arr.append(ui->lineEdit->text().trimmed());
            TypeSending type = SEND_COMMAND;
            udp->send(arr, type);
            ui->lineEdit->setText("");
        }
    }
}

void Dialog::closeEvent(QCloseEvent *event){
    udp->send(QByteArray(), SEND_END);
    event->accept();
}

void Dialog::settings(){
    setting->show();
    setting->toStart(udp, core);
}

void Dialog::on_pushButton_clicked()
{
    fs->toStart(udp, "Logs/Log.ini");
    fs->show();
}
