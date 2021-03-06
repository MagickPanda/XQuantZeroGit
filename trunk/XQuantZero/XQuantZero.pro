#-------------------------------------------------
#
# Project created by QtCreator 2018-09-24T17:57:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XQuantZero
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# uses C++ 11 features(for thread-safe singleton)
CONFIG += c++11


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    CBaseTrader.cpp \
    CCTPProxy.cpp \
    cfgutil.cpp \
    swbase.cpp \
    cconsoleform.cpp \
    clogindialog.cpp \
    cconfighandler.cpp \
    ceventproxy.cpp \
    ctraderproxy.cpp \
    ceventqueue.cpp \
    cevent.cpp \
    cctpevent.cpp \
    CCTPTrader.cpp \
    CCTPQuoter.cpp \
    ceventfactory.cpp \
    cctpeventfactory.cpp \
    cabstractevent.cpp \
    cabstracteventhandler.cpp \
    cctpeventhandler.cpp

HEADERS += \
        mainwindow.h \
    swbase.h \
    CBaseTrader.h \
    CCTPProxy.h \
    cfgutil.h \
    STDLL.h \
    cconsoleform.h \
    clogindialog.h \
    cconfighandler.h \
    ceventproxy.h \
    ctraderproxy.h \
    ceventqueue.h \
    cevent.h \
    cctpevent.h \
    CCTPTrader.h \
    CCTPTraderMD.h \
    CCTPTraderRsp.h \
    CCTPQuery.h \
    CCTPQuoter.h \
    ceventfactory.h \
    cctpeventfactory.h \
    cctpshared.h \
    cabstractevent.h \
    cabstracteventhandler.h \
    cctpeventhandler.h

FORMS += \
        mainwindow.ui \
    cconsoleform.ui \
    clogindialog.ui

# this must be your path of ctp headers
INCLUDEPATH += C:/dev_package/ctp/win32/
