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
    socket->writeDatagram(QByteArray("Завершено"), QHostAddress(host), port);
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
           printf("Connect...\n");
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(), datagram.size());
            if (Autorization(datagram)){
                break;
            }
        }
    } else {
        while (socket->hasPendingDatagrams()){
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(), datagram.size());
            if (QString(datagram) == "Продление"){
                timer1->stop();
                timer1->start(50000);
                printf("The extension of the session.\n");                
                socket->writeDatagram(QByteArray("Продление успешно"), QHostAddress(host), port);
            } else if (QString(datagram) == "Завершение"){
                timer();
            } else {
                action(datagram);
            }
        }
    }
}

void Server::action(QByteArray arr){
    QStringList a = QString(arr).split("|||");
    if (a[0] == "Настройки"){
        qDebug() << a[1];
    }
}

bool Server::Autorization(QByteArray arr){
    QStringList a = QString(arr).split(" ");
    if (a[0] == login && a[1] == password){
        stage = true;
        port = a[2].toInt();
        host = a[3];
        socket->writeDatagram(QByteArray("Вход"), QHostAddress(host), port);
        printf("Connected.\n");
        timer1->start(50000);
        return true;
    } else {
        printf("Connection error.\n");
    }
    return false;
}
