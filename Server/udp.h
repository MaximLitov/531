#ifndef UDP_H
#define UDP_H

#include <QObject>
#include <QUdpSocket>

enum TypeSending
{
    SEND_ERROR = 0,
    SEND_AUTOR = 1,
    SEND_SET_LOG = 2,
    SEND_CHECK_CONNECT = 3,
    SEND_OTVET = 4,
    SEND_END = 5,
    SEND_GETSETTINGS = 6,
    SEND_SETSETTINGS = 7,
    SEND_COMMAND = 8
};

struct Data
{
    TypeSending type;
    QByteArray arr;

    int getSize()
    {
        return arr.length() * sizeof(QByteArray) + sizeof(TypeSending);
    }

    QByteArray getBuff()
    {
        QByteArray arr;
        arr.resize(getSize());
        char *d = arr.data();
        memcpy(d, this, getSize());
        return arr;
    }

    void setBuff(QByteArray buff)
    {
        memcpy(this, buff, buff.length());
    }
};

class Udp : public QObject
{
    Q_OBJECT
public:
    explicit Udp(QObject *parent = nullptr);

public:
    Udp(QString host, int port, int thisPort);
    //int inMess(QByteArray arr);
    int send(QByteArray in, QByteArray &out, TypeSending &type);
    int send(QByteArray in, TypeSending type);
    QByteArray getArray();
    TypeSending getType();
    int getPort();
    QString getHost();

private:
    //int sendUdp();
    //int sendCore(QByteArray in, QByteArray &out);
    int sendCore();
    int takeCore();
    QByteArray waitUdp();

signals:
    void otvUdp(TypeSending);

private slots:
    void read();

private:
    //QByteArray tmps;
    QString host;
    int port;
    //int i;
    Data data;
    QUdpSocket *socket;
};

#endif // UDP_H
