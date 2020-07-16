#ifndef UDP_H
#define UDP_H

#include <QObject>
#include <QUdpSocket>

enum TypeSending
{
    SEND_ERROR = 0,
    SEND_OK = 1,
    SEND_AUTOR = 2,
    SEND_SET_LOG = 3,
    SEND_CHECK_CONNECT = 4,
    SEND_OTVET = 5,
    SEND_END = 6,
    SEND_GETSETTINGS = 7,
    SEND_SETSETTINGS = 8,
    SEND_COMMAND = 9,
    SEND_FILE = 10
};

struct Data
{
    TypeSending type;
    QByteArray arr;

    int getSize()
    {
        return arr.length() + sizeof(TypeSending);
    }

    QByteArray getBuff()
    {
        QByteArray arr;
        arr.append(QString::number(type) + "|");
        arr.append(QString(this->arr + "|"));
        arr.append(QString::number(getSize()));
        return arr;
    }

    void setBuff(QByteArray buff)
    {
        QStringList a = QString(buff).split("|");
        type = TypeSending(a[0].toInt());
        arr.clear();
        arr.append(a[1]);
        if (a[2].toInt() != getSize()){
            type = SEND_ERROR;
        }
    }
};

class Udp : public QObject
{
    Q_OBJECT
public:
    explicit Udp(QObject *parent = 0);

public:
    Udp(QString host, int port, int thisPort);
    int send(QByteArray in, QByteArray &out, TypeSending &type);
    int send(QByteArray in, TypeSending type);
    QByteArray getArray();
    TypeSending getType();
    int getPort();
    int getThisPort();
    QString getHost();
    void setPort(int port);
    void setHost(QString host);

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
    int thisPort;
    Data data;
    QUdpSocket *socket;
};

#endif // UDP_H
