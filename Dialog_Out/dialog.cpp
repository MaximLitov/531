#include "dialog.h"
#include "ui_dialog.h"
#include <QMenu>

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

void Dialog::toStart(Udp *a){
    udp = a;
    connect(udp, SIGNAL(lampochka(int)), this, SLOT(lampa(int)));
}

void Dialog::lampa(int a){
    switch (a) {
    case 1:
        ui->label->setPixmap(QPixmap(":Migalka/Images/circle_green.png"));
        break;
    case 2:
        ui->label->setPixmap(QPixmap(":Migalka/Images/circle_red.png"));
        break;
    default:
        ui->label->setPixmap(QPixmap(":Migalka/Images/circle_grey.png"));
        break;
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
            arr.append("Команда|||");
            arr.append(ui->lineEdit->text().trimmed());
            if (udp->send(arr, ar) == 0){
                item = new QStandardItem("Ответ: " + QString(ar));
                model->appendRow(item);
                ui->listView->setModel(model);
            }
            ui->lineEdit->setText("");
        }
    }
}

void Dialog::closeEvent(QCloseEvent *event){
    for (int i = 0; i < 3; i++){
        if (udp->Disconnect() == 0){
            break;
        }
    }
    event->accept();
}

void Dialog::settings(){
    setting->show();
    setting->toStart(udp);
}
