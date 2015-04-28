#-------------------------------------------------
#
# Project created by QtCreator 2015-04-28T13:33:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2048
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    copyrightinformation.cpp \
    settings.cpp \
    howtoplay.cpp \
    whatis2048.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    copyrightinformation.h \
    settings.h \
    howtoplay.h \
    whatis2048.h

FORMS    += mainwindow.ui \
    dialog.ui \
    copyrightinformation.ui \
    settings.ui \
    howtoplay.ui \
    whatis2048.ui

RESOURCES += \
    myresource.qrc

DISTFILES +=
