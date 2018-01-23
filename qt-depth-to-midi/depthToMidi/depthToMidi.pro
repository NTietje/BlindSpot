#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T06:58:42
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = depthToMidi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        processor.cpp

HEADERS  += mainwindow.h\
            processor.h


FORMS    += mainwindow.ui

include(../drumstick/drumstick.pro)
include(../opencv/videoengine.pri)
