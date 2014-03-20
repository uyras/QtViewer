#-------------------------------------------------
#
# Project created by QtCreator 2013-04-23T16:53:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    generatedialog.cpp \
    mygraphicsview.cpp \
    partgraphicsitem.cpp

HEADERS  += mainwindow.h \
    generatedialog.h \
    mygraphicsview.h \
    partgraphicsitem.h

FORMS    += mainwindow.ui \
    generatedialog.ui

INCLUDEPATH += "../PartsEngine/"

LIBS += -L$$PWD/../PartsEngine -lPartsEngine

RESOURCES += \
    res.qrc
