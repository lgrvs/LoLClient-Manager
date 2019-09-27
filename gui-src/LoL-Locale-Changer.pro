#-------------------------------------------------
#
# Project created by QtCreator 2019-09-27T18:43:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoL-Locale-Changer
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

RC_ICONS = icon.ico

#Static linking flags:
CONFIG += static
QMAKE_LFLAGS += -static

RESOURCES += \
    rc.qrc

