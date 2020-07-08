#include "log.h"
#include <QTime>

Log::Log(QObject *parent) : QObject(parent)
{
    log = new QFile("Logs/Log.ini");
    log->open(QIODevice::ReadWrite);
    log->close();
}

void Log::writeLog(QString a, QString b){
    log->open(QIODevice::Append);
    QByteArray f;
    f.append("<" + QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + "> ");
    f.append(a + " " + b + "\n");
    log->write(f);
    log->close();
}
