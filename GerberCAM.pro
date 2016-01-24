#-------------------------------------------------
#
# Project created by QtCreator 2014-12-24T11:42:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gerberCAM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gerber.cpp \
    drawPCB.cpp \
    toolpath.cpp \
    preprocess.cpp \
    clipper.cpp \
    treeitem.cpp \
    treemodel.cpp \
    settingwindow.cpp \
    setting.cpp

HEADERS  += mainwindow.h \
    gerber.h \
    drawPCB.h \
    toolpath.h \
    preprocess.h \
    clipper.hpp \
    treeitem.h \
    treemodel.h \
    settingwindow.h \
    setting.h

FORMS    += settingwindow.ui \
    mainwindow.ui

RESOURCES += \
    resources.qrc
