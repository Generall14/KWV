#-------------------------------------------------
#
# Project created by QtCreator 2016-04-08T21:53:52
#
#-------------------------------------------------

QT       += core gui

RC_ICONS = ics/ikona.ico

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
    src/3rdforms/kw3rdgif.cpp \
    src/osrc/MyQLineEdit.cpp \
	src/3rdforms/kw3rdfileinfo.cpp

HEADERS  += mainwindow.h \
    hplik.h \
    src/mwptr.h \
    src/kwmenu.h \
    src/kwgraphicsview.h \
    src/kwstatusbar.h \
    src/kwmotor.h \
    src/3rdforms/kw3rdgif.h \
    src/osrc/MyQLineEdit.hpp \
	src/3rdforms/kw3rdfileinfo.h

RESOURCES += \
    ikony.qrc
