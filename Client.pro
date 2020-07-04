#-------------------------------------------------
#
# Project created by QtCreator 2019-04-13T15:23:04
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    logindlg.cpp \
    chosysdlg.cpp \
    registerdlg.cpp \
    edituserinfo.cpp \
    meetsys.cpp \
    lwibomeet.cpp \
    bookmeet.cpp \
    lwimeeting.cpp \
    mybtn.cpp \
    mytablewidget.cpp \
    conndb.cpp

HEADERS += \
        mainwindow.h \
    logindlg.h \
    ../build-Client-Desktop_Qt_5_12_2_MSVC2017_64bit-Debug/ui_logindlg.h \
    ../build-Client-Desktop_Qt_5_12_2_MSVC2017_64bit-Debug/ui_mainwindow.h \
    chosysdlg.h \
    registerdlg.h \
    edituserinfo.h \
    meetsys.h \
    lwibomeet.h \
    bookmeet.h \
    lwimeeting.h \
    mybtn.h \
    mytablewidget.h \
    conndb.h

FORMS += \
        mainwindow.ui \
    logindlg.ui \
    chosysdlg.ui \
    registerdlg.ui \
    edituserinfo.ui \
    meetsys.ui \
    lwibomeet.ui \
    bookmeet.ui \
    lwimeeting.ui \
    mytablewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
