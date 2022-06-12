#
# Persistence Library / Collections / makefile
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

PROJECT = libcollections
OUTPUTDIR =../lib
OUTPUTFILE =$(PROJECT).a
ROOTDIR = ../../..
SRCDIR = $(ROOTDIR)/Source
LIBDIR = -L../lib
LIBS =


CC	= gcc
CPP	= g++
AS	= as
AR	= ar rcs
LD	= ld
CP	= cp
OS	= size
OD	= objdump
MD  = mkdir

INC = -I$(SRCDIR)/Collections

SRC = $(SRCDIR)/Collections/TArray.cpp\
	  $(SRCDIR)/Collections/TDictionary.cpp\
	  $(SRCDIR)/Collections/TEnumerable.cpp\
	  $(SRCDIR)/Collections/TEnumerator.cpp\
	  $(SRCDIR)/Collections/TList.cpp\
	  $(SRCDIR)/Collections/TObjectList.cpp\
	  $(SRCDIR)/Collections/TQueue.cpp\
	  $(SRCDIR)/Collections/TSortedDictionary.cpp\
	  $(SRCDIR)/Collections/TStack.cpp\
	  $(SRCDIR)/Collections/TStaticDictionary.cpp\
	  $(SRCDIR)/Collections/TStaticList.cpp\

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

CPPFLAGS  = -g -lstdc++ -shared -fPIC
CFLAGS  = -g -shared -fPIC
ASFLAGS = -g
LDFLAGS  = -g

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

