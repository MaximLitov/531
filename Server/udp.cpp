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
    //emit otvUdp(getType());
    qDebug() << "1";
}

int Udp::send(QByteArray in, QByteArray &out, TypeSending &type){
    data.arr = in;
    data.type = type;
    sendCore();
    takeCore();
    out = data.arr;
    type = data.type;
    return 0;
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
        tmp.append(i);
        if (i == 0){
            tmp = buff.mid((count - i) * sSendData, tmp.length());
        } else {
            tmp = buff.mid((count - i) * sSendData, (count - i + 1) * sSendData);
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
    QByteArray out = mess.right(mess.length() - 1);
    for (int i = mess.left(sizeof(int)).toInt() - 1; i >= 0; i--){
        mess.clear();
        mess = waitUdp();
        if (mess.left(sizeof(int)).toInt() == i){
            out.append(mess.right(mess.length() - 1));
        } else {
            return 1;
        }
    }
    data.setBuff(out);
    return 0;
}

QByteArray Udp::waitUdp(){
    for(int i = 0; i < 100; i++) {
        if(!socket->hasPendingDatagrams()){
            QThread::msleep(10);
        } else {
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
//int Udp::send(QByteArray in, QByteArray &out, TypeSending type){
//    data.arr = in;
//    data.type = type;
//    int i = sendUdp();

//    return i;
//}

//int Udp::send(QByteArray in, TypeSending type){
//    data.arr = in;
//    data.type = type;
//    return sendUdp();
//}

//int Udp::sendUdp(){
//    QByteArray b = data.getBuff();
//    QByteArray out;
//    int sData = data.getSize();
//    int sSendData = 65507 - sizeof(int);
//    int count = sData / sSendData;
//    for (int i = count; i >= 0; i--){
//        QByteArray tmp;
//        tmp.append(i);
//        if (i == 0){
//            tmp = b.mid((count - i) * sSendData, tmp.length() - 1);
//        } else {
//            tmp = b.mid((count - i) * sSendData, (count - i + 1) * sSendData);
//        }
//        if (sendCore(tmp, out) != 0){
//            i++;
//        }
//    }
//    return 0;
//}

//int Udp::sendCore(QByteArray in, QByteArray &out){
//    socket->writeDatagram(in, QHostAddress(host), port);
//    for(int i = 0; i < 100; i++)
//    {
//        if(!socket->hasPendingDatagrams()){
//            QThread::msleep(10);
//        }
//        else
//        {
//            out.resize(socket->pendingDatagramSize());
//            socket->readDatagram(out.data(), out.size());
//            return 0;
//        }
//    }
//    return 1;
//}

//int Udp::inMess(QByteArray arr){
//    if (i == 0){
//        i = arr.left(sizeof(int)).toInt();
//    } else {
//        if (arr.left(sizeof(int)).toInt() == i - 1){
//            i--;
//        } else {
//            return -1 * (i - 1);
//        }
//    }
//    tmps.append(arr.right(arr.length() - sizeof(int)));
//    if (i == 0){
//        data.setBuff(tmps);
//    }
//    return i;
//}
