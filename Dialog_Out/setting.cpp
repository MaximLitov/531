#include "setting.h"
#include "ui_setting.h"
#include <QMessageBox>

Setting::Setting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::toStart(Udp *a){
    udp = a;
    moveLogs = 1;
    ui->comboBox->addItem("Коммутирующая линия - резерв");
    ui->comboBox->addItem("Только РРР");
    ui->comboBox->addItem("Без РРР");
    ui->comboBox_2->addItem("TAP");
    ui->comboBox_2->addItem("TUN");
    QByteArray ar;
    if (udp->send(QByteArray("Настройки"), ar) == 0){
        QStringList a = QString(ar).split("\n");
        for (int i = 0; i < a.size(); i++){
            QStringList b = a[i].split("=");
            if (b[0] == "moveLogs"){
                moveLogs = b[1].toInt();
            }
            if (b[0] == "usePPP"){
                ui->comboBox->setCurrentIndex(2 - b[1].toInt());
            }
            if (b[0] == "useTUN"){
                ui->comboBox_2->setCurrentIndex(b[1].toInt());
            }
            if (b[0] == "use3GModem"){
                if (b[1].toInt() == 1) {ui->checkBox->setChecked(true);} else {ui->checkBox->setChecked(false);}
            }
            if (b[0] == "delKeys"){
                if (b[1].toInt() == 1) {ui->checkBox_2->setChecked(true);} else {ui->checkBox_2->setChecked(false);}
            }
            if (b[0] == "startSsh"){
                if (b[1].toInt() == 1) {ui->checkBox_3->setChecked(true);} else {ui->checkBox_3->setChecked(false);}
            }
            if (b[0] == "useBond"){
                if (b[1].toInt() == 1) {ui->checkBox_4->setChecked(true);} else {ui->checkBox_4->setChecked(false);}
                on_checkBox_4_clicked();
            }
            if (b[0] == "bondList"){
                b[1].remove(b[1].length() - 1, 1);
                b[1].remove(0, 1);
                ui->lineEdit->setText(b[1]);
            }
            if (b[0] == "ipNTPServer"){
                b[1].remove(b[1].length() - 1, 1);
                b[1].remove(0, 1);
                ui->lineEdit_2->setText(b[1]);
            }
        }
    }
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
        arr.append("Настройка|||");
        arr.append("moveLogs=" + QString::number(moveLogs) + "\n");
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
            arr.append(ui->lineEdit->text());
        }
        arr.append("\"\n");
        arr.append("ipNTPServer=\"" + ui->lineEdit_2->text() + "\"");
        QByteArray ar;
        if (udp->send(arr, ar) == 0 && QString(ar) == "Настройка успешна"){
            QMessageBox::warning(this, "", "Настройки сохранены");
        } else {
            QMessageBox::warning(this, "Ошибка", "Настройки не сохранены");
        }
    } else {
        QMessageBox::warning(this, "Внимание", "Неверный формат ip-адресса");
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

void Setting::on_pushButton_2_clicked()
{
    this->close();
}
