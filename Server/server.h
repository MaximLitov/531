#ifndef SERVER_H
#define SERVER_H

#include "log.h"
#include "udp.h"
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
    int ind;
    int maxtime;

private:
    void process();
    bool Autorization();
    void toStart();

public slots:
    void tree(TypeSending type);
    void timer();

private:
    QTimer *timer1;
    QFile *file;
    QFile *file2;
    QProcess proc;
    Log *log;
    Udp *udp;
};

#endif // SERVER_H
