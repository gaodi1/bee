#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T10:25:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    qextserialbase.cpp \
    win_qextserialport.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    qextserialbase.h \
    win_qextserialport.h

FORMS    += mainwindow.ui
