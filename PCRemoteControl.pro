#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T18:26:03
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PCRemoteControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Server.cpp

HEADERS  += mainwindow.h \
    Server.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    pictures.qrc
