# Makefile
#
SOFT_VERSION=2.9.5.001
ROOTDIR = ..
include $(ROOTDIR)/Rules.make


#CFLAGS += -c -g -Wall

CFLAGS += -c -O2 -Wl,--hash-style=sysv -DSOFT_VERSION=\"$(SOFT_VERSION)\"

#CFLAGS += -c -g -Wall -DSOFT_VERSION=\"$(SOFT_VERSION)\"
INCLUDES = -I ../include
INCLUDES += -I ../include/comn



ifeq ($(LIB_FORMAT), static)
LIB_GCC = $(CROSS_COMPILE)
endif


ifeq ($(LIB_FORMAT), dynamic)
LINKLIB_FLAGS = -fPIC
LIB_GCC = $(CROSS_COMPILE) $(LINKLIB_FLAGS)
endif




SRC  = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRC))

all:$(OBJS)

%.o:%.cpp
	$(LIB_GCC) $(CFLAGS)  $(INCLUDES) $< -o $@
	mv $@  ../objs

clean:
	
