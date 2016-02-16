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
    mygraphicsview.cpp \
    partgraphicsitem.cpp \
    mygraphicsscene.cpp \
    magneticcircle.cpp \
    systemproperties.cpp

HEADERS  += mainwindow.h \
    mygraphicsview.h \
    partgraphicsitem.h \
    mygraphicsscene.h \
    magneticcircle.h \
    systemproperties.h

FORMS    += mainwindow.ui \
    systemproperties_old.ui \
    systemproperties.ui

INCLUDEPATH += "../partsEngine/"

LIBS += -L$$PWD/../partsEngine -lPartsEngine

win32{
    RC_FILE = QtViewer.rc
    #CONFIG(release,debug|release){
     #   DESTDIR = "build-windows/$$PWD"
    #}
}

RESOURCES += \
    icons.qrc

CONFIG += c++11
