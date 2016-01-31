#-------------------------------------------------
#
# Project created by QtCreator 2016-01-30T18:52:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GerberCAM1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calculation.cpp \
    clipper.cpp \
    drawPCB.cpp \
    gerber.cpp \
    preprocess.cpp \
    setting.cpp \
    settingwindow.cpp \
    toolpath.cpp \
    treeitem.cpp \
    treemodel.cpp \
    aboutwindow.cpp

HEADERS  += mainwindow.h \
    calculation.h \
    drawPCB.h \
    gerber.h \
    preprocess.h \
    Read me.h \
    setting.h \
    settingwindow.h \
    toolpath.h \
    treeitem.h \
    treemodel.h \
    clipper.hpp \
    debug/ui_about.h \
    debug/ui_mainwindow.h \
    debug/ui_settingwindow.h \
    aboutwindow.h

FORMS    += mainwindow.ui \
    settingwindow.ui \
    aboutwindow.ui \
    aboutwindow.ui

RESOURCES += \
    resources.qrc
