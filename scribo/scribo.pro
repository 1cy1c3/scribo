#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T19:25:18
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scribo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    preference.cpp \
    aes.cpp \
    regex.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    preference.h \
    aes.h \
    regex.h \
    database.h

FORMS    += mainwindow.ui \
    preference.ui

RESOURCES += \
    app.qrc

DISTFILES += \
    conf/scribo.db
