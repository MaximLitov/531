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
        int s = getSize();

//        size_t s2 = sizeof(Data);
//        size_t s3 = sizeof(QByteArray);

        arr.resize(getSize());
        char *d = arr.data();
        memcpy(d, this, getSize());
//        Data d2;
//        d2.setBuff(arr);
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
    explicit Udp(QObject *parent = 0);

public:
    Udp(QString host, int port, int thisPort);
    int send(QByteArray in, QByteArray &out, TypeSending type);
    int send(QByteArray in, TypeSending type);
    QByteArray getArray();
    TypeSending getType();
    int getPort();
    QString getHost();

private:
    int sendCore();
    int takeCore();
    QByteArray waitUdp();

signals:
    void otvUdp(TypeSending);

private slots:
    void read();

private:
    QString host;
    int port;
    Data data;
    QUdpSocket *socket;
};

#endif // UDP_H
