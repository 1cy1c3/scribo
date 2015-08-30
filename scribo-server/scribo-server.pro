#-------------------------------------------------
#
# Project created by QtCreator 2015-08-29T00:27:10
#
#-------------------------------------------------

QT       += core xml network

QT       -= gui

TARGET = scribo-server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    scriboserver.cpp \
    task.cpp \
    client.cpp

HEADERS += \
    scriboserver.h \
    task.h \
    client.h

RESOURCES += \
    server.qrc
