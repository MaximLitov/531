#ifndef SETTING_H
#define SETTING_H

#include <QMainWindow>
#include <QUdpSocket>

namespace Ui {
class Setting;
}

class Setting : public QMainWindow
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

public:
    void toStart();

private slots:
    void on_pushButton_clicked();
    void on_checkBox_4_clicked();

private:
    Ui::Setting *ui;
    QUdpSocket *socket;
};

#endif // SETTING_H
