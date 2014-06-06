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
    worker.cpp \
    options.cpp

HEADERS  += dialog.h \
    worker.h \
    version.h \
    options.h \
    settings.h

FORMS    += dialog.ui \
    options.ui

RESOURCES += \
    resources.qrc

unix|win32: LIBS += -L$$PWD/../../../synchro_test/ -llibsynchro

INCLUDEPATH += $$PWD/../../../synchro_test
DEPENDPATH += $$PWD/../../../synchro_test

RC_FILE = resources.rc
