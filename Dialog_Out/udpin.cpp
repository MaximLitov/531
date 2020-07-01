#include "udpin.h"

UdpIn::UdpIn(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    dialog = new Dialog();
    setting = new Setting();
    socket->bind(7778);
    connect(socket, SIGNAL(readyRead()), this, SLOT(returnUdp()));
}

QByteArray UdpIn::start(){
    if (ar == "Вход"){
        return QByteArray("Вход");
    } else {
        return QByteArray("");
    }
}

void UdpIn::returnUdp(){
    while (socket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size());
        QStringList a = QString(datagram).split("|||");
        if (a[0] == "Вход"){
            ar = "Вход";
        } else if (a[0] == "Продление успешно"){
            dialog->returnUdp(":Migalka/Images/circle_green.png");
        } else if (a[0] == "Завершено"){
            dialog->returnUdp(":Migalka/Images/circle_red.png");
        } else {
            qDebug() << a[0];
        }
    }
}
