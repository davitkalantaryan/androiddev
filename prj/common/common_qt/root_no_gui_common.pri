#
# File root_no_gui_common.pri
# File created : 02 Feb 2017
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
#



MYROOTSYS = /afs/ifh.de/@sys/products/root64/5.20.00
MYROOTCFLAGS = `$$MYROOTSYS/bin/root-config \
    --cflags`
QMAKE_CXXFLAGS += $$MYROOTCFLAGS
QMAKE_CFLAGS += $$MYROOTCFLAGS

message("!!! daqcollector_common.pri: ROOT_FLAGS=$$MYROOTCFLAGS")

INCLUDEPATH += /afs/ifh.de/@sys/products/root64/5.20.00/include

#LIBS += `$$MYROOTSYS/bin/root-config --libs`
LIBS +=  $$system($$MYROOTSYS/bin/root-config --libs)
