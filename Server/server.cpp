#include "server.h"
#include <QDebug>
//#include <iostream>
//#include <string>
//#include <QString>
#include <QTextStream>
QTextStream cout(stdout);
QTextStream cin(stdin);

Server::Server(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    timer1 = new QTimer();
    stage = false;
    printf("Login: ");
    login = cin.readLine();
    printf("Password: ");
    password = cin.readLine();
    socket->bind(7777);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readUdp()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(timer()));
}

void Server::timer(){
    socket->writeDatagram(QByteArray("Завершение успешно"), QHostAddress(host), port);
    stage = false;
    port = 0;
    host = "";
    timer1->stop();
    printf("Disconnected.\n");
}

void Server::readUdp()
{
    if (!stage){
        while (socket->hasPendingDatagrams()){
           //printf("Connect... \n");
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(), datagram.size());
            if (Autorization(datagram)){
                break;
            }
        }
    } else {
        while (socket->hasPendingDatagrams()){
            QByteArray arr;
            arr.resize(socket->pendingDatagramSize());
            socket->readDatagram(arr.data(), arr.size());
            QStringList a = QString(arr).split("|||");
            if (a[0] == "Продление"){
                timer1->stop();
                timer1->start(5000);
                printf("The extension of the session.\n");                
                socket->writeDatagram(QByteArray("Продление успешно"), QHostAddress(host), port);
            } else if (a[0] == "Завершение"){
                timer();
            } else if (a[0] == "Настройка"){
                socket->writeDatagram(QByteArray("Настройка успешна"), QHostAddress(host), port);
                action(a[1]);
            } else {

            }
        }
    }
}

void Server::action(QString a){
        qDebug() << QString(a);
}

bool Server::Autorization(QByteArray arr){
    QStringList a = QString(arr).split(" ");
    if (a[0] == login && a[1] == password){
        stage = true;
        port = a[2].toInt();
        host = a[3];
        socket->writeDatagram(QByteArray("Вход"), QHostAddress(host), port);
        printf("Connected.\n");
        timer1->start(5000);
        return true;
    } else {
        printf("Connection error.\n");
    }
    return false;
}
