#
# Persistence Library / Graphics / makefile
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

PROJECT = libgraphics
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

INC = -I$(SRCDIR)/BaseTypes\
	  -I$(SRCDIR)/Graphics\
	  -I$(SRCDIR)/Math

SRC = $(SRCDIR)/Graphics/TCachedWindowsBmpFile.cpp\
	  $(SRCDIR)/Graphics/TCanvas.cpp\
	  $(SRCDIR)/Graphics/TColorConverter.cpp\
	  $(SRCDIR)/Graphics/TColorRGB.cpp\
	  $(SRCDIR)/Graphics/TColorYUV.cpp\
	  $(SRCDIR)/Graphics/TDxtBlockCreator.cpp\
	  $(SRCDIR)/Graphics/TEndlessLine.cpp\
	  $(SRCDIR)/Graphics/TFilter3x3.cpp\
	  $(SRCDIR)/Graphics/TFilter5x5.cpp\
	  $(SRCDIR)/Graphics/TFilter7x7.cpp\
	  $(SRCDIR)/Graphics/TFont.cpp\
	  $(SRCDIR)/Graphics/TFontCharacter.cpp\
	  $(SRCDIR)/Graphics/TGraphicsData.cpp\
	  $(SRCDIR)/Graphics/TPixelFormatConverter.cpp\
	  $(SRCDIR)/Graphics/TPosition.cpp\
	  $(SRCDIR)/Graphics/TRectangle.cpp\
	  $(SRCDIR)/Graphics/TSize.cpp\
	  $(SRCDIR)/Graphics/TVector.cpp\
	  $(SRCDIR)/Graphics/TWindowsBmpFile.cpp\

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

