#-------------------------------------------------
#
# Project created by QtCreator 2015-04-28T13:33:54
#
#-------------------------------------------------


QT       += core gui\
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2048
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    copyrightinformation.cpp \
    settings.cpp \
    howtoplay.cpp \
    whatis2048.cpp \
    sound.cpp \
    data.cpp \
    gameview.cpp

HEADERS  += mainwindow.h \
    copyrightinformation.h \
    settings.h \
    howtoplay.h \
    whatis2048.h \
    sound.h \
    data.h \
    gameview.h

FORMS    += mainwindow.ui \
    copyrightinformation.ui \
    settings.ui \
    howtoplay.ui \
    whatis2048.ui \
    gameview.ui

RESOURCES += \
    myresource.qrc

DISTFILES +=
