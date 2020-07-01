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
    send(QByteArray("Продление"), a);
    if (QString(a) != "Продление успешно"){
        qDebug() << "Ошибка продления.";
        state = false;
    } else {
        state = true;
    }
}

int Udp::Connect(QString login, QString password){

    //
    QByteArray arr;
    arr.append(login + " ");
    arr.append(password + " ");
    arr.append(QString::number(portIn) + " ");
    arr.append(host);
    QByteArray ar;
    send(arr, ar);
    timer();
    timer1 = new QTimer();
    timer1->start(2000);
    connect(timer1, SIGNAL(timeout()), this, SLOT(timer()));
}

bool Udp::isConnect(){
    return state;
}

int Udp::send(QByteArray in, QByteArray &out){
    try {
        socket->writeDatagram(in, QHostAddress(host), port);
        out = getMessage(1000);
//        while (socket->hasPendingDatagrams()){
//            QByteArray arr;
//            arr.resize(socket->pendingDatagramSize());
//            socket->readDatagram(arr.data(), arr.size());
//            out = arr;
//            qDebug() << out;

//        }
    } catch (int e){
        return e;
    }
    return 0;
}

QByteArray Udp::getMessage(int ms)
{
    for(int i=0; i<5; i++)
    {
        if(!socket->hasPendingDatagrams())
            QThread::msleep(ms);
        else
        {
            QByteArray arr;
            arr.resize(socket->pendingDatagramSize());
            socket->readDatagram(arr.data(), arr.size());
            qDebug() << arr;
            return arr;
        }
    }
    return QByteArray();
}
