#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog = new Dialog();
    udp = new Udp("127.0.0.1", 7777, 7778);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (udp->Connect(QString(ui->lineEdit->text()), QString(ui->lineEdit_2->text())) == 0){
        start();
    } else {
        QMessageBox::warning(this, "Ошибка", "Логин или пароль неверны.");
    }
}

void MainWindow::start()
{
    dialog->show();
    dialog->toStart(udp);
    this->close();
}
