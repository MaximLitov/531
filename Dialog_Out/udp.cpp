#include "udp.h"
#include <QThread>
#include <QNetworkInterface>

Udp::Udp(QObject *parent) :
    QObject(parent)
{

}

Udp::Udp(QString host, int port, int portIn){
    socket = new QUdpSocket(this);
    timer1 = new QTimer();
    socket->bind(portIn);
    this->portIn = portIn;
    this->host = host;
    this->port = port;
    state = false;
    connect(timer1, SIGNAL(timeout()), this, SLOT(timer()));
}

void Udp::timer(){
    QByteArray a;
    if (send(QByteArray("Продление"), a) == 0 && QString(a) == "Продление успешно"){
        emit lampochka(1);
        state = true;
    } else {
        Connect(login, password);
        emit lampochka(2);
        state = false;
    }
}

int Udp::Connect(QString login, QString password){
    if (!state){
        if (!isIp(host)){
            return 3;
        }
        this->login = login;
        this->password = password;
        QByteArray arr;
        arr.append(login + " ");
        arr.append(password + " ");
        arr.append(QString::number(portIn) + " ");
        {
            QList<QHostAddress> addr = QNetworkInterface::allAddresses();
            arr.append(addr[1].toString());
        }
        QByteArray ar;
        if (send(arr, ar) == 0 && QString(ar) == "Вход"){
            timer();
            if (timer1->isActive()){
                emit lampochka(1);
                timer1->stop();
            }
            timer1->start(2000);
            state = true;
            return 0;
        } else if (QString(ar) == "Ошибка входа"){
            return 2;
        }
    }
    return 1;
}

int Udp::Disconnect(){
    if (state){
        QByteArray ar;
        if (send(QByteArray("Завершение"), ar) == 0 && QString(ar) == "Завершение успешно"){
            timer1->stop();
            emit lampochka(2);
            state = false;
            delete socket;
            return 0;
        } else {
            return 1;
        }
    } else {
        delete socket;
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
            if (QString(arr).split("|||")[0] == "Ответ"){
                QByteArray a;
                a.append(QString(arr).split("|||")[1]);
                emit otvet(a);
                return 2;
            } else {
                while (socket->hasPendingDatagrams()){
                    QByteArray arr1;
                    arr1.resize(socket->pendingDatagramSize());
                    socket->readDatagram(arr1.data(), arr1.size());
                }
            }
            out = arr;
            return 0;
        }
    }
    return 1;
}

bool Udp::isIp(QString d){
    QStringList a = d.split(".");
    bool b = false;
    if (a.size() == 4){
        if (0 <= a[0].toInt() && a[0].toInt() <= 255 &&
            0 <= a[1].toInt() && a[1].toInt() <= 255 &&
            0 <= a[2].toInt() && a[2].toInt() <= 255 &&
            0 <= a[3].toInt() && a[3].toInt() <= 255){
            bool c = true;
            for (int i = 0; i < 4; i++){
                a[i].toInt(&c);
                if (!c){
                    break;
                }
            }
            b = c;
        }
    }
    return b;
}
