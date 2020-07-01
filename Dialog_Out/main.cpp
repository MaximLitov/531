#include "mainwindow.h"
#include <QApplication>
void test(QString param1, QString& param2);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
