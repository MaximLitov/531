#include "server.h"
#include <QDebug>
#include <QTextStream>
#include <QThread>
#include <QNetworkInterface>

QTextStream cout(stdout);
QTextStream cin(stdin);

Server::Server(QObject *parent) :
    QObject(parent)
{
    ind = 0;
    timer1 = new QTimer();
    file = new QFile("config.ini");
    file2 = new QFile("access.ini");
    log = new Log();
    stage = false;
    qDebug() << "IP: " + QNetworkInterface::allAddresses()[2].toString();
    printf("Login: ");
    login = cin.readLine();
    printf("Password: ");
    password = cin.readLine();
    udp = new Udp("", 0, 7777);
    connect(udp, SIGNAL(otvUdp(TypeSending)), this, SLOT(tree(TypeSending)));
    connect(timer1, SIGNAL(timeout()), this, SLOT(timer()));
    log->writeLog("<ИНФОРМАЦИЯ>", "Сервер запущен.");
    toStart();
}

void Server::toStart(){
    if (!file2->open(QIODevice::ReadWrite)){
        log->writeLog("<ОШИБКА>", "Файл не найден.");
        printf("File is not found.\n");
    } else {
        maxtime = QString(file2->readLine()).toInt();
        file2->close();
    }
}

void Server::timer(){
    udp->send(QByteArray(), TypeSending::SEND_END);
    stage = false;
    udp->setPort(-1);
    udp->setHost("");
    timer1->stop();
    log->writeLog("<ИНФОРМАЦИЯ>", "Отключено.");
    printf("Disconnected.\n");
}

void Server::process(){
    if (!file2->open(QIODevice::ReadWrite)){
        log->writeLog("<ОШИБКА>", "Файл не найден.");
        printf("File is not found.\n");
    } else {
        bool c = true;
        file2->readLine();
        QString b = QString(udp->getArray()).split(" ")[0];
        while (!file2->atEnd() && c){
            if (b == QString(file2->readLine()).split("\n")[0]){
                log->writeLog("<ИНФОРМАЦИЯ>", "Запуск команды: " + QString(udp->getArray()));
                proc.start("sh", QStringList() << "-c" << QString(udp->getArray()));
                proc.waitForFinished(maxtime);
                QByteArray out = QByteArray(proc.readAll());
                udp->send(out, TypeSending::SEND_OTVET);
                log->writeLog("<ИНФОРМАЦИЯ>", "Ответ команды:\n" + QString(out));
                c = false;
            }
        }
        if (c){
            udp->send(QByteArray("Команда не распознана"), TypeSending::SEND_OTVET);
            log->writeLog("<ПРЕДУПРЕЖДЕНИЕ>", "Команда не распознана: " + b);
        }
    file2->close();
    }
}

void Server::tree(TypeSending type){
    if (!stage){
        if (type == TypeSending::SEND_AUTOR){
            if (!Autorization()){
                printf("Connection error.\n");
            }
        }
    } else {
        if (type == TypeSending::SEND_CHECK_CONNECT){
            timer1->stop();
            timer1->start(5000);
            log->writeLog("<ИНФОРМАЦИЯ>", "Продление сеанса.");
            printf("The extension of the session.\n");
            udp->send(QByteArray(), TypeSending::SEND_CHECK_CONNECT);
        } else if (type == TypeSending::SEND_END){
            timer();
        } else if (type == TypeSending::SEND_SETSETTINGS){
            if (!file->open(QIODevice::ReadWrite)){
                log->writeLog("<ОШИБКА>", "Файл не найден.");
                printf("File is not found.\n");
                udp->send(QByteArray(), TypeSending::SEND_ERROR);
            } else {
                QByteArray b = udp->getArray();
                file->write(b);
                file->close();
                log->writeLog("<ИНФОРМАЦИЯ>", "Изменение настроек.");
                udp->send(QByteArray(), TypeSending::SEND_SETSETTINGS);
            }
        } else if (type == TypeSending::SEND_GETSETTINGS){
            if(!file->open(QIODevice::ReadWrite)){
                log->writeLog("<ОШИБКА>", "Файл не найден.");
               printf("File is not found.\n");
               udp->send(QByteArray(), TypeSending::SEND_ERROR);
            } else {
                QByteArray arr;
                while (!file->atEnd()){
                    arr.append(QString(file->readLine()));
                }
                log->writeLog("<ИНФОРМАЦИЯ>", "Запрос настроек.");
                udp->send(arr, TypeSending::SEND_GETSETTINGS);
            }
            file->close();
        } else if (type == TypeSending::SEND_COMMAND){
            process();
        } else if (type == TypeSending::SEND_FILE){
            QFile *f = new QFile(QString(udp->getArray()));
            if(!f->open(QIODevice::ReadWrite)){
                log->writeLog("<ОШИБКА>", "Файл не найден.");
               printf("File is not found.\n");
               udp->send(QByteArray(), TypeSending::SEND_ERROR);
            } else {
                QByteArray arr;
                while (!f->atEnd()){
                    arr.append(QString(f->readLine()));
                }
                log->writeLog("<ИНФОРМАЦИЯ>", "Запрос файла: " + QString(udp->getArray()));
                udp->send(arr, TypeSending::SEND_FILE);
            }
            f->close();
        }else {
            log->writeLog("<ПРЕДУПРЕЖДЕНИЕ>", "Запрос не распознан.");
        }
    }
}


bool Server::Autorization(){
    QStringList a = QString(udp->getArray()).split(" ");
    if (a[0] == login && a[1] == password){
        stage = true;
        udp->setPort(a[2].toInt());
        udp->setHost(a[3]);
        udp->send(QByteArray(), TypeSending::SEND_AUTOR);
        log->writeLog("<ИНФОРМАЦИЯ>", "Подключено.");
        printf("Connected.\n");
        timer1->start(5000);
        return true;
    } else if (a.size() == 4) {
        udp->setPort(a[2].toInt());
        udp->setHost(a[3]);
        udp->send(QByteArray(), TypeSending::SEND_ERROR);
        udp->setPort(-1);
        udp->setHost("");
        log->writeLog("<ПРЕДУПРЕЖДЕНИЕ>", "Ошибка подключения.");
    }
    return false;
}
