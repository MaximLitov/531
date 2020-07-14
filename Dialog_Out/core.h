#ifndef CORE_H
#define CORE_H

#include "udp.h"
#include <QObject>
#include <QTimer>

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

public:
    int isIP(QString ip);
    void returnConnect();
    void setConnect(Udp *a);
    void setAut(QByteArray aut);

private:
    bool con;
    QByteArray aut;
    QTimer *tmr;
    Udp *udp;

signals:
    void isConnect(bool);

private slots:
    void timer();
};

#endif // CORE_H
