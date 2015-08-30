#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T19:25:18
#
#-------------------------------------------------

VERSION = 0.1

QT       += core gui sql printsupport multimedia multimediawidgets network

ICON = scribo.icns
RC_ICONS = scribo.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scribo
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    preference.cpp \
    aes.cpp \
    regex.cpp \
    database.cpp \
    about.cpp \
    help.cpp \
    update.cpp

HEADERS  += mainwindow.h \
    preference.h \
    aes.h \
    regex.h \
    database.h \
    about.h \
    help.h \
    update.h

FORMS    += mainwindow.ui \
    preference.ui \
    about.ui \
    update.ui

RESOURCES += \
    app.qrc

DISTFILES += \
    conf/scribo.db

mac {
    Resources.files = ./video
    Resources.path = Contents/Resources
    QMAKE_BUNDLE_DATA += Resources
}

win32 {
    Resources.files = ./video
    Resources.path = .
    QMAKE_BUNDLE_DATA += Resources
}
