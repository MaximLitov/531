#-------------------------------------------------
#
# Project created by QtCreator 2020-06-24T08:53:44
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dialog_Out
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    setting.cpp \
    udp.cpp \
    core.cpp \
    fileshow.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    setting.h \
    udp.h \
    core.h \
    fileshow.h

FORMS    += mainwindow.ui \
    dialog.ui \
    setting.ui \
    fileshow.ui

RESOURCES += \
    Mig.qrc
