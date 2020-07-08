#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QFile>

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = nullptr);

public:
    void writeLog(QString a, QString b);

private:
    QFile *log;
};

//#define log(a,b) new Log().writeLog(a,b);
//#define l Log log;
#endif // LOG_H
