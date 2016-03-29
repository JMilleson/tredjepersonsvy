#-------------------------------------------------
#
# Project created by QtCreator 2016-03-24T17:35:59
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpclient.cpp \
    occulussensorcontroller.cpp

HEADERS  += mainwindow.h \
    tcpclient.h \
    occulussensorcontroller.h

FORMS    += mainwindow.ui
