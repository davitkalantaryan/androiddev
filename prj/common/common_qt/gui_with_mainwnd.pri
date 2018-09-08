# File remote_plot_spectrum.pro
# File created : 04 Jul 2017
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
# CONFIG += TEST
# For making test: '$qmake "CONFIG+=TEST" daqadcreceiver.pro' , then '$make'

include(../../common/common_qt/sys_common.pri)

greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
CONFIG += c++11
INCLUDEPATH += ../../../include

SOURCES += \
    ../../../src/common/common_ui_qt_mainwindow.cpp \
    ../../../src/common/common_ui_qt_centralwidget.cpp

HEADERS += \
    ../../../include/common_ui_qt_mainwindow.hpp \
    ../../../include/common_ui_qt_centralwidget.hpp

RESOURCES += \
    ../../../src/resources/toolbar1.qrc

