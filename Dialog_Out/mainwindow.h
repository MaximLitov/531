#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
#include "udp.h"
#include "core.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int port;
    QString host;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void start();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Dialog *dialog;
    Udp *udp;
    Core *core;
};

#endif // MAINWINDOW_H
