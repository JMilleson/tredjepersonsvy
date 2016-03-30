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
    occulussensorcontroller.cpp \
    occulussensocontrollersimulator.cpp

HEADERS  += mainwindow.h \
    tcpclient.h \
    occulussensorcontroller.h \
    occulussensocontrollersimulator.h \
    json.hpp

FORMS    += mainwindow.ui

LIBS += -L$$PWD/'../../../../../Program Files/OculusSDK/LibOVR/Lib/Windows/x64/Release/VS2015/' -lLibOVR

INCLUDEPATH += $$PWD/'../../../../../Program Files/OculusSDK/LibOVR/Lib/Windows/x64/Release/VS2015'
DEPENDPATH += $$PWD/'../../../../../Program Files/OculusSDK/LibOVR/Lib/Windows/x64/Release/VS2015'

INCLUDEPATH += $$PWD/'../../../../../Program Files/OculusSDK/LibOVR/Include'
INCLUDEPATH += $$PWD/'../../../../../Program Files/OculusSDK/LibOVR/Include/extras'

