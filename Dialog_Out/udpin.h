#ifndef UDPIN_H
#define UDPIN_H

#include <dialog.h>
#include <setting.h>
#include <QObject>
#include <QUdpSocket>

class UdpIn : public QObject
{
    Q_OBJECT
public:
    explicit UdpIn(QObject *parent = 0);

private:
    QString ar;

public:
    QByteArray start();

public slots:
    void returnUdp();

private:
    QUdpSocket *socket;
    Dialog *dialog;
    Setting *setting;
};

#endif // UDPIN_H
