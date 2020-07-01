#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

private:
    QString login;
    QString password;
    bool stage;
    int port;
    QString host;

private:
    void action(QString a);

public slots:
    void timer();
    void readUdp();
    bool Autorization(QByteArray arr);

private:
    QTimer *timer1;
    QUdpSocket *socket;
};

#endif // SERVER_H
