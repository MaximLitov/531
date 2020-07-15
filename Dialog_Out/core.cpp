#include "core.h"
#include <QStringList>

Core::Core(QObject *parent) :
    QObject(parent)
{
    con = false;
}

void Core::timer(){
    if (con){
        TypeSending type = SEND_CHECK_CONNECT;
        QByteArray a;
        udp->send(QByteArray(), a, type);
       if (type != SEND_OK){
            emit isConnect(false);
            con = false;
        }
    } else {
        TypeSending type = SEND_AUTOR;
        QByteArray a;
        if (udp->send(aut, a, type) == 0 && type == SEND_OK){
            emit isConnect(true);
            con = true;
        }
    }
}

int Core::isIP(QString ip){
    QStringList arr = ip.split(".");
    if (arr.size() != 4){
        return 1;
    }
    for (int i = 0; i < 4; i++){
        bool a;
        int b = arr[i].toInt(&a);
        if (a){
            if (b < 0 || b > 255){
                return 1;
            }
        } else {
            return 1;
        }
    }
    return 0;
}

void Core::setConnect(Udp *a){
    udp = a;
    con = true;
}

void Core::returnConnect(){
    con = false;
}

void Core::setAut(QByteArray aut){
    this->aut = aut;
    tmr = new QTimer();
    tmr->start(2000);
    connect(tmr, SIGNAL(timeout()), this, SLOT(timer()));
}
