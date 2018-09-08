


CPP = g++
LINK = g++
CFLAGS= -std=c++11 -O2 -DLINUX -D_REENTRANT -D_GNU_SOURCE -D_BSD -D__USE_GNU -D__USE_UNIX98 -I../../../include -I../../../src/client
LFLAGS  = -O2
LIBS = -lnsl -lpthread -lrt


TARGET1=client_test
TARGET1_name=client_test
BIN_DIR_PATH=../../../../sys/jessie/bin
TARGET1_path=$(BIN_DIR_PATH)/$(TARGET1_name)
OBJECT_FILES_DIR1=../../../sys/jessie/.objects/$(TARGET1_name)


default: all
all: $(TARGET1)


##########################################################################################################
common_iodevice.o: 
	mkdir -p $(OBJECT_FILES_DIR1)
	$(CPP) -c $(CFLAGS) -o $(OBJECT_FILES_DIR1)/common_iodevice.o		../../../src/common/common_iodevice.cpp	
common_socketbase.o:
	$(CPP) -c $(CFLAGS) -o $(OBJECT_FILES_DIR1)/common_socketbase.o		../../../src/common/common_socketbase.cpp
common_sockettcp.o:
	$(CPP) -c $(CFLAGS) -o $(OBJECT_FILES_DIR1)/common_sockettcp.o		../../../src/common/common_sockettcp.cpp
ftssystem_common.o:
	$(CPP) -c $(CFLAGS) -o $(OBJECT_FILES_DIR1)/ftssystem_common.o		../../../src/common/ftssystem_common.cpp
main_client_test.o:
	$(CPP) -c $(CFLAGS) -o $(OBJECT_FILES_DIR1)/main_client_test.o		../../../src/test/main_client_test.cpp
util_ftsclient.o:
	$(CPP) -c $(CFLAGS) -o $(OBJECT_FILES_DIR1)/util_ftsclient.o		../../../src/client/util_ftsclient.cpp


	
$(TARGET1): common_iodevice.o common_socketbase.o common_sockettcp.o ftssystem_common.o main_client_test.o util_ftsclient.o 
						mkdir -p $(BIN_DIR_PATH)
						@echo "*** Linking $(TARGET1_name)"
						$(LINK) $(OBJECT_FILES_DIR1)/*.o $(LIBS) $(LFLAGS) -o $(TARGET1_path)
						

# Rule for generating object files from C++ source files
.cpp.o: 
	@echo "*** Compiling $<"
	$(CPP) -c $(CFLAGS) $<
	#$(CPP) -c $(CFLAGS) -o "$@" "$<"
	#$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

clean:	
	@rm -f $(TARGET1_name) $(OBJECT_FILES_DIR1)/*.o

