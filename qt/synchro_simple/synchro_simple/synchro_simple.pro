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
    worker.cpp

HEADERS  += dialog.h \
    worker.h

FORMS    += dialog.ui

RESOURCES += \
    resources.qrc

unix|win32: LIBS += -L$$PWD/../../../copy_struct_test/ -llibsynchro

INCLUDEPATH += $$PWD/../../../copy_struct_test
DEPENDPATH += $$PWD/../../../copy_struct_test
