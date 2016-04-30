#-------------------------------------------------
#
# Project created by QtCreator 2016-04-08T21:53:52
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KWView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/mwptr.cpp \
    src/kwmenu.cpp \
    src/kwgraphicsview.cpp \
    src/kwstatusbar.cpp \
    src/kwmotor.cpp \
    src/3rdforms/kw3rdgif.cpp

HEADERS  += mainwindow.h \
    hplik.h \
    src/mwptr.h \
    src/kwmenu.h \
    src/kwgraphicsview.h \
    src/kwstatusbar.h \
    src/kwmotor.h \
    src/3rdforms/kw3rdgif.h
