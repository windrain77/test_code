# Author : hjmin@pineone.com

####################################################################
#  VARIABLES & FLAGS
####################################################################
PROJECT_ROOT = .
CROSS = 
CC    = $(CROSS)gcc
CXX   = $(CROSS)g++
AR    = $(CROSS)ar
LD    = $(CROSS)ld
STRIP = $(CROSS)strip

SRCS = $(PROJECT_ROOT)/src
INCS = -I$(PROJECT_ROOT)/inc

LDFLAGS = -ldl \
		  -lm \

CFLAGS = -O2 -Wall \
		 -D_REENTRANT \
		 $(INCS) \
		 $(sdl_flags)

CXXFLAGS = $(CFLAGS)
		 
####################################################################
# Add object files
OBJS = \
	   $(SRCS)/main.o \
	   $(SRCS)/common.o 

####################################################################
#  COMPILE 
####################################################################
CP  = cp
RM  = rm
MV  = mv
ZIP = gzip
TAR = tar

TARGET = app_test

.SUFFIXES: .c .cc .cpp .cxx .h .hh .hpp .hxx .o 

.c.o:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $< 

.PHONY : all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo
	$(CC) -o $@ $(OBJS) $(CXXFLAGS) $(LDFLAGS)
	@$(RM) -f `find . -name *.o | grep -e [A-Za-z0-9*][.]o`
	@$(STRIP) $@
	@echo "[1;34m-------------------- Build $(TARGET) --------------------[0m"

clean:
	$(RM) -f `find . -name *.o | grep -e [A-Za-z0-9*][.]o`
	$(RM) -f $(TARGET)
	@echo "[1;31m-------------------- Clean $(TARGET) --------------------[0m"
