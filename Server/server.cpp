#include "server.h"
#include <QDebug>
#include <QTextStream>
#include <QThread>

QTextStream cout(stdout);
QTextStream cin(stdin);

Server::Server(QObject *parent) :
    QObject(parent)
{
    ind = 0;
    socket = new QUdpSocket(this);
    timer1 = new QTimer();
    file = new QFile("config.ini");
    file2 = new QFile("access.ini");
    log = new Log();
    stage = false;
    printf("Login: ");
    login = cin.readLine();
    printf("Password: ");
    password = cin.readLine();
    socket->bind(7777);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readUdp()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(timer()));
}

void Server::timer(){
    socket->writeDatagram(QByteArray("Завершение успешно"), QHostAddress(host), port);
    stage = false;
    port = 0;
    host = "";
    timer1->stop();
    log->writeLog("<ИНФОРМАЦИЯ>", "Отключено.");
    printf("Disconnected.\n");
}

void Server::process(QString a){
    //    while (!file2->atEnd()){
    //        QStringList b = a[1].split(" ");
    //        if (b[0] == QString(file2->readLine())){
//    log->writeLog("<ИНФОРМАЦИЯ>", "Запуск команды: " + a);
//    proc.start("sh", QStringList() << "-c" << a);
//    proc.waitForFinished();
//    QByteArray output = QByteArray("Ответ|||"); //65507
//    //output.append("Ответ|||");
//    output.append(proc.readAll());
//    socket->writeDatagram(output, QHostAddress(host), port);
//    log->writeLog("<ИНФОРМАЦИЯ>", "Ответ команды: " + QString(output).split("|||")[1]);
    //        } else {
    //            log->writeLog("<ПРЕДУПРЕЖДЕНИЕ>", "Команда не распознана: " + QString(output).split("|||")[1]);
    //        }
    //    }
}

void Server::readUdp()
{
    while (socket->hasPendingDatagrams()){
        QByteArray arr;
        arr.resize(socket->pendingDatagramSize());
        socket->readDatagram(arr.data(), arr.size());
        if (!stage){
            if (Autorization(arr)){
                break;
            } else {
                printf("Connection error.\n");
            }
        } else {
            QStringList a = QString(arr).split("|||");
            if (a[0] == "Продление"){
                timer1->stop();
                timer1->start(3000);
                log->writeLog("<ИНФОРМАЦИЯ>", "Продление сеанса.");
                printf("The extension of the session.\n");
                socket->writeDatagram(QByteArray("Продление успешно"), QHostAddress(host), port);
            } else if (a[0] == "Завершение"){
                timer();
            } else if (a[0] == "Настройка"){
                if (!file->open(QIODevice::ReadWrite)){
                    log->writeLog("<ОШИБКА>", "Файл не найден.");
                    printf("File is not found.\n");
                    socket->writeDatagram(QByteArray(""), QHostAddress(host), port);
                } else {
                    QByteArray b;
                    b.append(a[1]);
                    file->write(b);
                    file->close();
                    log->writeLog("<ИНФОРМАЦИЯ>", "Изменение настроек.");
                    socket->writeDatagram(QByteArray("Настройка успешна"), QHostAddress(host), port);
                }
            } else if (a[0] == "Настройки"){
                if(!file->open(QIODevice::ReadWrite)){
                    log->writeLog("<ОШИБКА>", "Файл не найден.");
                   printf("File is not found.\n");
                   socket->writeDatagram(QByteArray(""), QHostAddress(host), port);
                } else {
                    QByteArray arr;
                    while (!file->atEnd()){
                        arr.append(QString(file->readLine()));
                    }
                    log->writeLog("<ИНФОРМАЦИЯ>", "Запрос настроек.");
                    socket->writeDatagram(arr, QHostAddress(host), port);
                }
                file->close();
            } else if (a[0] == "Команда"){
                if (!file2->open(QIODevice::ReadWrite)){
                    log->writeLog("<ОШИБКА>", "Файл не найден.");
                    printf("File is not found.\n");
                } else {
                    process(a[1]);
                }
                file2->close();
            } else {
                QString a1;
                foreach (QString aa, a) {
                    a.append(aa);
                }
                log->writeLog("<ПРЕДУПРЕЖДЕНИЕ>", "Запрос не распознан: " + a1);
            }
        }
    }
}

bool Server::Autorization(QByteArray arr){
    QStringList a = QString(arr).split(" ");
    if (a[0] == login && a[1] == password){
        stage = true;
        port = a[2].toInt();
        host = a[3];
        socket->writeDatagram(QByteArray("Вход"), QHostAddress(host), port);
        log->writeLog("<ИНФОРМАЦИЯ>", "Подключено.");
        printf("Connected.\n");
        timer1->start(3000);
        return true;
    } else if (a.size() >= 3) {
        socket->writeDatagram(QByteArray("Ошибка входа"), QHostAddress(a[3]), a[2].toInt());
        log->writeLog("<ПРЕДУПРЕЖДЕНИЕ>", "Ошибка подключения.");
    }
    return false;
}
