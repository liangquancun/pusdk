# Makefile
#
ROOTDIR = ..
include $(ROOTDIR)/Rules.make


SRC  = $(wildcard *.o)


ifeq ($(LIB_FORMAT), static)
LIB_GCC= ar -rv
TARGET=libmspu.a
endif


ifeq ($(LIB_FORMAT), dynamic)
LINKLIB_FLAGS = -shared -o
LIB_GCC = $(CROSS_COMPILE) $(LINKLIB_FLAGS)
TARGET=libmspu.so
endif




all:$(TARGET)

$(TARGET):$(SRC)
	$(LIB_GCC) $@ $(SRC) 
	mv $(TARGET) ../lib
clean:


