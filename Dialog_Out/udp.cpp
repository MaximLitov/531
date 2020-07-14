#include "udp.h"
#include <QThread>

Udp::Udp(QObject *parent) : QObject(parent){}

Udp::Udp(QString host, int port, int thisPort){
    socket = new QUdpSocket(this);
    socket->bind(thisPort);
    this->host = host;
    this->port = port;
    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
}

void Udp::read(){
    takeCore();
    emit otvUdp(data.type);
}

int Udp::send(QByteArray in, QByteArray &out, TypeSending type){
    data.arr = in;
    data.type = type;
    sendCore();
    if (takeCore() == 1){
        return 2;
    }
    if (data.type == type){
        out = data.arr;
        return 0;
    }
    return 1;
}

int Udp::send(QByteArray in, TypeSending type){
    data.arr = in;
    data.type = type;
    sendCore();
    return 0;
}

int Udp::sendCore(){
    QByteArray buff = data.getBuff();
    QByteArray out;
    int sData = data.getSize();
    int sSendData = 65507 - 1;
    int count = sData / sSendData;
    for (int i = count; i >= 0; i--){
        QByteArray tmp;
        tmp.append(QString::number(i));
        if (i == 0){
            tmp.append(buff.mid((count - i) * sSendData, buff.length()));
            qDebug() << buff.mid((count - i) * sSendData, buff.length()).length();
        } else {
            tmp.append(buff.mid((count - i) * sSendData, (count - i + 1) * sSendData));
        }
        socket->writeDatagram(tmp, QHostAddress(host), port);
        out = waitUdp();
        if (out == QByteArray()){
            return 1;
        } else {
            if (QString(out).toInt() != i){
                return 2;
            }
        }
    }
    return 0;
}

int Udp::takeCore(){
    QByteArray mess = waitUdp();
    if (mess == QByteArray()){
        return 1;
    }
    QByteArray out = mess.right(mess.length() - sizeof(int));
    for (int i = mess.left(sizeof(int)).toInt() - 1; i >= 0; i--){
        mess.clear();
        mess = waitUdp();
        if (mess.left(sizeof(int)).toInt() == i){
            out = mess.right(mess.length() - sizeof(int));
        } else {
            return 1;
        }
    }
    data.setBuff(out);
    return 0;
}

QByteArray Udp::waitUdp(){
    for(int i = 0; i < 100; i++)
    {
        if(!socket->hasPendingDatagrams()){
            QThread::msleep(10);
        }
        else
        {
            QByteArray out;
            out.resize(socket->pendingDatagramSize());
            socket->readDatagram(out.data(), out.size());
            return out;
        }
    }
    return QByteArray();
}

QByteArray Udp::getArray(){
    return data.arr;
}

TypeSending Udp::getType(){
    return data.type;
}

int Udp::getPort(){
    return port;
}

QString Udp::getHost(){
    return host;
}
