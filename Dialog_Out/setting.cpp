#include "setting.h"
#include "ui_setting.h"
#include <QMessageBox>

Setting::Setting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    socket = new QUdpSocket(this);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::toStart(){
    socket->bind(7778);
    ui->comboBox->addItem("Коммутирующая линия - резерв");
    ui->comboBox->addItem("Только РРР");
    ui->comboBox->addItem("Без РРР");
    ui->comboBox_2->addItem("TAP");
    ui->comboBox_2->addItem("TUN");
    //Запрос на сервер - настройки
}

void Setting::on_pushButton_clicked()
{
    QStringList a = ui->lineEdit_2->text().split(".");
    bool b = false;
    if (a.size() == 4){
        if (0 <= a[0].toInt() && a[0].toInt() <= 255 &&
            0 <= a[1].toInt() && a[1].toInt() <= 255 &&
            0 <= a[2].toInt() && a[2].toInt() <= 255 &&
            0 <= a[3].toInt() && a[3].toInt() <= 255){
            bool c = true;
            for (int i = 0; i < 4; i++){
                a[i].toInt(&c);
                if (!c){
                    break;
                }
            }
            b = c;
        }
    }
    if (b){
        QByteArray arr;
        arr.append("Настройки|||");
        arr.append("moveLogs=1\n");
        arr.append("usePPP=" + QString::number(2 - ui->comboBox->currentIndex()) + "\n");
        arr.append("useTUN=" + QString::number(ui->comboBox_2->currentIndex()) + "\n");
        arr.append("use3GModem=");
        if (ui->checkBox->isChecked()) {arr.append("1\n");} else {arr.append("0\n");}
        arr.append("delKeys=");
        if (ui->checkBox_2->isChecked()) {arr.append("1\n");} else {arr.append("0\n");}
        arr.append("startSsh=");
        if (ui->checkBox_3->isChecked()) {arr.append("1\n");} else {arr.append("0\n");}
        arr.append("useBond=");
        if (ui->checkBox_4->isChecked()) {arr.append("1\n");} else {arr.append("0\n");}
        arr.append("bondList=\"");
        if (ui->checkBox_4->isChecked()) {
            arr.append(ui->lineEdit->text() + "\"\n");
        } else {
            arr.append("\"\n");
        }
        arr.append("ipNTPServer=\"" + ui->lineEdit_2->text() + "\"");
        socket->writeDatagram(arr, QHostAddress::LocalHost, 7777);
    } else {
        QMessageBox::warning(this, "Внимание","Неверный формат ip-адресса");
    }
}

void Setting::on_checkBox_4_clicked()
{
    if (ui->checkBox_4->isChecked()){
        ui->label_3->setEnabled(true);
        ui->lineEdit->setEnabled(true);
    } else {
        ui->label_3->setEnabled(false);
        ui->lineEdit->setEnabled(false);
    }
}
