#-------------------------------------------------
#
# Project created by QtCreator 2020-03-09T21:41:07
#
#-------------------------------------------------

QT       += core gui

CONFIG += C++11 static
QMAKE_LFLAGS += -static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = intercepmgr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
