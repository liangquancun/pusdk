# Makefile
#
ROOTDIR = ..
include $(ROOTDIR)/Rules.make


LIB_INCLUDES = -I ./
LIB_INCLUDES += -I ../include
LIB_INCLUDES += -I ../include/comn

ifeq ($(LIB_FORMAT), static)
LIB_LOCAL = ../lib/libmspu.a
endif

ifeq ($(LIB_FORMAT), dynamic)
LIB_LOCAL = -L ../lib/ -lmspu
endif


GCC =$(CROSS_COMPILE) -g
CFLAGS += -c -D__MS_DEBUG__  -g
SYSLIB = -lpthread  -lrt


TARGET = putest

SRC  = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRC))

all:$(TARGET)

$(TARGET):$(OBJS)
	$(GCC) -o $@ $(OBJS) $(LIB_LOCAL) $(SYSLIB) 
	mv $(TARGET)  ../
%.o:%.cpp
	$(GCC)  $(CFLAGS) $< -o $@  $(LIB_INCLUDES)

clean:
	rm -rf $(OBJS) $(TARGET) 
