#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T19:25:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scribo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    settings.h

FORMS    += mainwindow.ui \
    settings.ui

RESOURCES += \
    app.qrc
