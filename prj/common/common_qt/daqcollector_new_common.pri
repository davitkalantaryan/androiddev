#
# File daqcollector_new_common.pri
# File created : 13 Feb 2017
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
#

message("!!! daqcollector_new_common.pri:")

include(../../common/common_qt/daqcollector_common.pri)


SOURCES += ../../../src/server/pitz_daq_collectorbase_new_global_fncs.cpp \
    ../../../src/server/pitz_daq_globalfunctionsiniter.cpp
HEADERS += ../../../src/server/pitz_daq_globalfunctionsiniter.hpp
