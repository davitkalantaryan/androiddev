# File remote_plot_spectrum.pro
# File created : 04 Jul 2017
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
# CONFIG += TEST
# For making test: '$qmake "CONFIG+=TEST" daqadcreceiver.pro' , then '$make'

CONFIG += c++11

include(../../common/common_qt/sys_common.pri)
include(../../common/common_qt/gui_with_mainwnd.pri)

greaterThan(QT_MAJOR_VERSION, 4){
DEFINES += MARGINES_OK
}else{
greaterThan(QT_MAJOR_VERSION, 3){
greaterThan(QT_MINOR_VERSION,7): DEFINES += MARGINES_OK
}
}

#QT += webkitwidgets
#DEFINES += webkitwidgets_defined

#DEFINES += DEBUG_APP_NON_RPI
#DEFINES += LOCAL_HOST_NAME=""192.168.0.100""
#QT += network

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../src/client
SOURCES += ../../../src/ui/main_omxplayer_guiclient.cpp \
    ../../../src/ui/omxclient_centralwidget.cpp \
    ../../../src/client/util_ftsclient.cpp \
    ../../../src/common/common_sockettcp.cpp \
    ../../../src/common/common_socketbase.cpp \
    ../../../src/common/common_iodevice.cpp \
    ../../../src/common/common_functionalities.cpp \
    ../../../src/common/common_argument_parser.cpp \
    ../../../src/common/ftssystem_common.cpp \
    ../../../src/ui/omxplayer_centraltab.cpp \
    ../../../src/ui/omxclient_groupwidget.cpp \
    ../../../src/ui/omxclient_qtapplication.cpp \
    ../../../src/ui/omxclient_youtube.cpp
HEADERS += \
    ../../../src/ui/omxclient_centralwidget.hpp \
    ../../../src/client/util_ftsclient.hpp \
    ../../../include/common_sockettcp.hpp \
    ../../../include/common_socketbase.impl.hpp \
    ../../../include/common_socketbase.hpp \
    ../../../include/common_iodevice.hpp \
    ../../../src/ui/omxplayer_centraltab.hpp \
    ../../../src/ui/omxclient_groupwidget.hpp \
    ../../../include/common_hashtbl.hpp \
    ../../../include/common_hashtbl.impl.hpp \
    ../../../include/common_unnamedsemaphorelite.hpp \
    ../../../src/ui/omxclient_qtapplication.hpp \
    ../../../include/ftssystem_interface_common.h \
    ../../../src/ui/omxclient_youtube.hpp \
    ../../../src/ui/omxclient_commondefs.hpp
