#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog = new Dialog();
    ui->lineEdit_3->setText("192.168.32.128");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    udp = new Udp(QString(ui->lineEdit_3->text()), 7777, 7778);
    int err = udp->Connect(QString(ui->lineEdit->text()), QString(ui->lineEdit_2->text()));
    if (err == 0){
        start();
    } else {
        if (err == 1){
            QMessageBox::warning(this, "Ошибка", "Ошибка подключения.");
        } else if (err == 2){
            QMessageBox::warning(this, "Ошибка", "Логин или пароль неверны.");
        } else if (err == 3){
            QMessageBox::warning(this, "Ошибка", "Ip-адрес неверен.");
        }
        delete udp;
    }
}

void MainWindow::start()
{
    dialog->show();
    dialog->toStart(udp);
    this->close();
}
