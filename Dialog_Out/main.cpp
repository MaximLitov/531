#include "mainwindow.h"
#include <QApplication>
void test(QString param1, QString& param2);
int main(int argc, char *argv[])
{
    Data d;
    d.type = SEND_ERROR;
    d.arr.append("hello!");
    QByteArray qb = d.getBuff();

    Data d2;
    d2.setBuff(qb);
    TypeSending t2 = d2.type;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
