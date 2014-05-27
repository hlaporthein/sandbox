#-------------------------------------------------
#
# Project created by QtCreator 2014-05-27T08:25:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = synchro_simple
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

unix|win32: LIBS += -L$$PWD/../ -lsynchro

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../synchro.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../libsynchro.a
