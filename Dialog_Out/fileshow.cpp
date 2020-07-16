#include "fileshow.h"
#include "ui_fileshow.h"

FileShow::FileShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileShow)
{
    ui->setupUi(this);
}

FileShow::~FileShow()
{
    delete ui;
}

void FileShow::toStart(Udp *a, QString b){
    udp = a;
    model = new QStandardItemModel();
    file = b;
    QByteArray out;
    TypeSending type = SEND_FILE;
    QByteArray c;
    c.append(file);
    if (udp->send(c, out, type) == 0 && type == SEND_OK){
        QStandardItem *item;
        QStringList d = QString(udp->getArray()).split("\n");
        for (int i = 0; i < d.size(); i++){
            qDebug() << d[i];
            item = new QStandardItem(QString(d[i]).trimmed());
            model->appendRow(item);
            ui->listView->setModel(model);
        }
    } else {
        close();
    }
}
