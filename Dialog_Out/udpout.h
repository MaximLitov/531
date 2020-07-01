#ifndef UDPOUT_H
#define UDPOUT_H

#include <QObject>
#include <QUdpSocket>

class UdpOut : public QObject
{
    Q_OBJECT
public:
    explicit UdpOut(QObject *parent = 0);

public:
    void sendUdp(QByteArray arr);

private:
    QUdpSocket *socket;
};

#endif // UDPOUT_H
