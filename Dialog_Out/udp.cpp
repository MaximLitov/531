#include "udp.h"
#include <QThread>

Udp::Udp(QObject *parent) :
    QObject(parent)
{

}

Udp::Udp(QString host, int port, int portIn){
    socket = new QUdpSocket(this);
    socket->bind(portIn);
    this->portIn = portIn;
    this->host = host;
    this->port = port;
}

void Udp::timer(){
    QByteArray a;
    if (send(QByteArray("Продление"), a) == 0 && QString(a) == "Продление успешно"){
        emit lampochka(1);
        state = true;
    } else {
        emit lampochka(0);
        state = false;
    }
}

int Udp::Connect(QString login, QString password){
    QByteArray arr;
    arr.append(login + " ");
    arr.append(password + " ");
    arr.append(QString::number(portIn) + " ");
    arr.append(host);
    QByteArray ar;
    if (send(arr, ar) == 0 && QString(ar) == "Вход"){
        timer();
        timer1 = new QTimer();
        timer1->start(2000);
        connect(timer1, SIGNAL(timeout()), this, SLOT(timer()));
    } else {
        return 1;
    }
    return 0;
}

int Udp::Disconnect(){
    timer1->stop();
    QByteArray ar;
    if (send(QByteArray("Завершение"), ar) == 0 && QString(ar) == "Завершение успешно"){
        emit lampochka(2);
        state = false;
        return 0;
    }
    return 1;
}

bool Udp::isConnect(){
    return state;
}

int Udp::send(QByteArray in, QByteArray &out){
    socket->writeDatagram(in, QHostAddress(host), port);
    for(int i = 0; i < 10; i++)
    {
        if(!socket->hasPendingDatagrams()){
            QThread::msleep(10);
        }
        else
        {
            QByteArray arr;
            arr.resize(socket->pendingDatagramSize());
            socket->readDatagram(arr.data(), arr.size());
            out = arr;
            return 0;
        }
    }
    return 1;
}
