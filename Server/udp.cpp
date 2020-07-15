#include "udp.h"
#include <QThread>

Udp::Udp(QObject *parent) : QObject(parent){}

Udp::Udp(QString host, int port, int thisPort){
    socket = new QUdpSocket(this);
    socket->bind(thisPort);
    this->host = host;
    this->port = port;
    this->thisPort = thisPort;
    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
}

void Udp::read(){
    takeCore();
    emit otvUdp(data.type);
}

int Udp::send(QByteArray in, QByteArray &out, TypeSending &type){
    data.arr = in;
    data.type = type;
    TypeSending a = type;
    type = SEND_ERROR;
    sendCore();
    if (takeCore() == 1){
        return 2;
    }
    if (data.type == a){
        type = SEND_OK;
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
    int sData = data.getSize();
    int sSendData = 65507;
    int count = sData / sSendData;
    for (int i = count; i >= 0; i--){
        QByteArray tmp;
        if (i == 0){
            tmp.append(buff.mid((count - i) * sSendData));
        } else {
            tmp.append(buff.mid((count - i) * sSendData, sSendData));
        }
        socket->writeDatagram(tmp, QHostAddress(host), port);
    }
    return 0;
}

int Udp::takeCore(){
    QByteArray out;
    while (true){
        QByteArray a = waitUdp();
        if (a == QByteArray()){
            return 1;
        }
        out.append(a);
        if (a.length() < 65507){
            break;
        }
    }
    data.setBuff(out);
    return 0;
}

QByteArray Udp::waitUdp(){
    for(int i = 0; i < 10; i++)
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

int Udp::getThisPort(){
    return thisPort;
}

QString Udp::getHost(){
    return host;
}

void Udp::setPort(int port){
    this->port = port;
}

void Udp::setHost(QString host){
    this->host = host;
}
