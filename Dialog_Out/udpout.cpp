#include "udpout.h"

UdpOut::UdpOut(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
}

void UdpOut::sendUdp(QByteArray arr){
    socket->writeDatagram(arr, QHostAddress::LocalHost, 7777);
}
