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
        dialog.cpp \
    synchronizer.cpp

HEADERS  += dialog.h \
    synchronizer.h

FORMS    += dialog.ui

unix|win32: LIBS += -L$$PWD/../../../copy_struct_test/ -llibsynchro

INCLUDEPATH += $$PWD/../../../copy_struct_test
DEPENDPATH += $$PWD/../../../copy_struct_test

RESOURCES += \
    resources.qrc
