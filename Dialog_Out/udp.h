#ifndef UDP_H
#define UDP_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class Udp : public QObject
{
    Q_OBJECT
public:
    explicit Udp(QObject *parent = 0);
    Udp(QString host, int port, int portIn);

public:
    int send(QByteArray in, QByteArray& out);
    int Connect(QString login, QString password);
    int Disconnect();
    bool isConnect();

signals:
    void lampochka(int);

private slots:
    void timer();

private:
    QString host;
    int port;
    int portIn;
    bool state;

private:
    QUdpSocket *socket;
    QTimer *timer1;
};

#endif // UDP_H
