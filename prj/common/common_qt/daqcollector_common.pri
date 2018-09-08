#
# File daqcollector_common.pri
# File created : 02 Feb 2017
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
#

message("!!! daqcollector_common.pri:")

HEADERS += ../../../src/tools/smallmutex.h \
    ../../../src/tools/lifostack.h \
    ../../../src/tools/fifofast.h \
    ../../../include/alog.h \
    ../../../src/server/pitz_daq_collectorbase.hpp \
    ../../../src/server/pitz_daq_collectorinfo.hpp \
    ../../../include/common_daq_definations.h
SOURCES += ../../../src/utils/unnamedsemaphore.cpp \
    ../../../src/utils/alog.cpp \
    ../../../src/server/pitz_daq_collectorbase.cpp \
    ../../../src/server/pitz_daq_collectorinfo.cpp

include(../../common/common_qt/doocs_server_common.pri)
include(../../common/common_qt/root_no_gui_common.pri)

