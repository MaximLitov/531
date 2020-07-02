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
    file = new QFile("../531/Server/config.ini");
    file2 = new QFile("../531/Server/access.ini");
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
    while (socket->hasPendingDatagrams()){
        QByteArray arr;
        arr.resize(socket->pendingDatagramSize());
        socket->readDatagram(arr.data(), arr.size());
        if (!stage){
            if (Autorization(arr))
                break;
        } else {
            //qDebug() << arr;
            QStringList a = QString(arr).split("|||");
            if (a[0] == "Продление"){
                timer1->stop();
                timer1->start(5000);
                printf("The extension of the session.\n");
                socket->writeDatagram(QByteArray("Продление успешно"), QHostAddress(host), port);
            } else if (a[0] == "Завершение"){
                timer();
            } else if (a[0] == "Настройка"){
                if (!file->open(QFile::WriteOnly)){
                    printf("File is not found.\n");
                } else {
                    qDebug() << a.size();
                    QByteArray b;
                    b.append(a[1]);
                    file->write(b);
                    file->close();
                    socket->writeDatagram(QByteArray("Настройка успешна"), QHostAddress(host), port);
                }
            } else if (a[0] == "Настройки"){// nki.ini   config.ini
                if(!file->open(QFile::ReadOnly)){
                   printf("File is not found.\n");
                   //file->open(QIODevice::ReadOnly);
                } else {
                    QByteArray arr;
                    while (!file->atEnd()){
                        arr.append(QString(file->readLine()));
                    }
                    socket->writeDatagram(arr, QHostAddress(host), port);
                }
                file->close();
            } else if (a[0] == "Команда"){// access.ini
                if (!file2->open(QFile::ReadOnly)){
                    printf("File is not found.\n");
                } else {
                    while (!file2->atEnd()){
//                        QByteArray c;
//                        c.append(QString(file2->readLine()));
                        if (a[1] == QString(file2->readLine())){
                            qDebug() << "123";
                        }
                    }
                }
                file2->close();
//                socket->writeDatagram(QByteArray("Настройка успешна"), QHostAddress(host), port);
            } else {
                qDebug() << a[0] << a[1];
            }
        }
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
        timer1->start(5000);
        return true;
    } else {
        printf("Connection error.\n");
    }
    return false;
}
