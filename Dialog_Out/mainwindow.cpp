#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog = new Dialog();
    core = new Core();
    ui->lineEdit_3->setText("192.168.32.128");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (core->isIP(QString(ui->lineEdit_3->text())) == 0){
        udp = new Udp(QString(ui->lineEdit_3->text()), 7777, 7778);
        QByteArray in;
        in.append(QString(ui->lineEdit->text()) + " ");
        in.append(QString(ui->lineEdit_2->text()) + " ");
        in.append(QString::number(udp->getPort()) + " ");
        in.append(udp->getHost());
        TypeSending type = SEND_AUTOR;
        QByteArray a;
        if (udp->send(in, a, type) == 0){
            if (type == SEND_AUTOR){
                core->setAut(in);
                start();
            } else {
                QMessageBox::warning(this, "Внимание", "Логин или пароль неверен");
                delete udp;
            }
        } else {
            QMessageBox::warning(this, "Внимание", "Ошибка подключения");
            delete udp;
        }
    } else {
        QMessageBox::warning(this, "Внимание", "Неверный формат ip-адресса");
    }
}

void MainWindow::start()
{
    core->setConnect(udp);
    dialog->show();
    dialog->toStart(udp, core);
    this->close();
}
