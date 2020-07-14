#-------------------------------------------------
#
# Project created by QtCreator 2020-06-25T11:54:12
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT += network

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += debug

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    log.cpp

HEADERS += \
    server.h \
    log.h
