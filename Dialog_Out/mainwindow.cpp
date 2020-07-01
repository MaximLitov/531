#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog = new Dialog();
    udp = new Udp("127.0.0.1", 7777, 7778);
//    port = 7778;
//    host = "127.0.0.1";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int e = udp->Connect(QString(ui->lineEdit->text()), QString(ui->lineEdit_2->text()));
//    QByteArray arr;
//    arr.append(QString(ui->lineEdit->text()) + " ");
//    arr.append(QString(ui->lineEdit_2->text()) + " ");
//    arr.append(QString::number(port) + " ");
//    arr.append(host);
//    QByteArray ar;
//    int e = udp->send(arr, ar);
//    if (e != 0) {qDebug() << "Ошибка: " << e << endl;}
//    if (QString(ar) == "Вход"){
//        start();
//    } else {
//        qDebug() << "Ошибка входа.";
//    }
}

void MainWindow::start()
{
    dialog->show();
    dialog->toStart();
    this->close();
}
