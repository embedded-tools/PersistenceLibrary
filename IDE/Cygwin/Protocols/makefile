#
# Persistence Library / Protocols / makefile
#
# Copyright (c) 2016-2019 Ondrej Sterba <osterba@atlas.cz>
#
# https://github.com/embedded-tools/PersistenceLibrary
#
# Permission to use, copy, modify, distribute and sell this software
# and its documentation for any purpose is hereby granted without fee,
# provided that the above copyright notice appear in all copies and
# that both that copyright notice and this permission notice appear
# in supporting documentation.
# It is provided "as is" without express or implied warranty.
#

PROJECT = libprotocols
OUTPUTDIR =../lib
OUTPUTFILE =$(PROJECT).a
ROOTDIR = ../../..
SRCDIR = $(ROOTDIR)/Source
LIBDIR = -L../lib
LIBS =

CC	= x86_64-pc-cygwin-gcc
CPP	= x86_64-pc-cygwin-g++
AS	= x86_64-pc-cygwin-gcc
AR	= x86_64-pc-cygwin-gcc-ar rcs
LD	= x86_64-pc-cygwin-g++
CP	= cp
OS	= size
OD	= od
MD	= mkdir

LDFLAGS  = -g

INC = -I$/usr/include\
      -I$(SRCDIR)/BasicTypes\
	  -I$(SRCDIR)/Collections\
	  -I$(SRCDIR)/Protocols

SRC = $(SRCDIR)/Protocols/tbinaryprotocol.cpp\
	  $(SRCDIR)/Protocols/tconsolelog.cpp\
	  $(SRCDIR)/Protocols/teventreceiver.cpp\
	  $(SRCDIR)/Protocols/teventsender.cpp\
	  $(SRCDIR)/Protocols/teventserializer.cpp\
	  $(SRCDIR)/Protocols/tfilelog.cpp\
	  $(SRCDIR)/Protocols/tlog.cpp\

#  C++ source files
CPPFILES = $(filter %.cpp, $(SRC))
#  C source files
CFILES = $(filter %.c, $(SRC))
#  Assembly source files
ASMFILES = $(filter %.s, $(SRC))

# Object files
CPPOBJ = $(CPPFILES:.cpp=.o)
COBJ = $(CFILES:.c=.o)
SOBJ = $(ASMFILES:.s=.o)
OBJ  = $(CPPOBJ) $(COBJ) $(SOBJ)

CPPFLAGS  = -g -std=c++11
CFLAGS  = -g -std=c99
ASFLAGS = -g

all: $(OUTPUTFILE)

$(OUTPUTFILE): $(OBJ)
	@if [ ! -d $(OUTPUTDIR) ];\
	then \
		$(MD) $(OUTPUTDIR);\
	fi
	@$(AR) $(OUTPUTDIR)/$(OUTPUTFILE) $(OBJ)
	@echo $(OUTPUTFILE)	

$(CPPOBJ): %.o: %.cpp
	@echo $<;
	@$(CPP) -c $(INC) $(CPPFLAGS) $(LIBDIR)  $< -o $@ 

$(COBJ): %.o: %.c
	@echo $<;
	@$(CC) -c $(INC) $(CFLAGS) $(LIBDIR) $< -o $@ 

$(SOBJ): %.o: %.s
	@echo $<;
	@$(AS) -c $(ASFLAGS) $< -o $@

clean:
	@rm -f $(OUTPUTDIR)/$(OUTPUTFILE) $(OBJ)

