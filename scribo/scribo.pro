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
    preference.cpp \
    aes.cpp \
    regex.cpp

HEADERS  += mainwindow.h \
    preference.h \
    aes.h \
    regex.h

FORMS    += mainwindow.ui \
    preference.ui

RESOURCES += \
    app.qrc
