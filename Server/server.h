#ifndef SERVER_H
#define SERVER_H

#include "log.h"
#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QFile>
#include <QProcess>

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
    int ind;

private:
    void process(QString a);
    bool Autorization(QByteArray arr);

public slots:
    void timer();
    void readUdp();

private:
    QTimer *timer1;
    QUdpSocket *socket;
    QFile *file;
    QFile *file2;
    QProcess proc;
    Log *log;
};

#endif // SERVER_H
