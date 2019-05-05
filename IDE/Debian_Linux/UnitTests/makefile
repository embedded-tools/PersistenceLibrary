#
# Persistence Library / UnitTests / makefile
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

PROJECT = test
OUTPUTFILE =$(PROJECT).so
ROOTDIR = ../../..
LIBDIR = -L../lib
LIBS = -lbasictypes -lcollections -lfileutils -lgraphics -lmath -lpersistence -lprotocols

SRCDIR = $(ROOTDIR)/Source

CC	= gcc
CPP	= g++
AS	= as
LD	= g++
CP	= objcopy
OS	= size
OD	= objdump
MD	= mkdir

INC = 	-I.\
	-I$(SRCDIR)/BasicTypes\
	-I$(SRCDIR)/Collections\
	-I$(SRCDIR)/FileUtils\
	-I$(SRCDIR)/Graphics\
	-I$(SRCDIR)/Math\
	-I$(SRCDIR)/Persistence\
	-I$(SRCDIR)/Protocols\
	-I$(SRCDIR)/UnitTests/Fonts\
	-I$(SRCDIR)/UnitTests/PersistenceExample

SRC = 	$(SRCDIR)/UnitTests/minicppunit.cpp\
        $(SRCDIR)/UnitTests/Test_EscapeSequences.cpp\
	$(SRCDIR)/UnitTests/Test_Persistence.cpp\
	$(SRCDIR)/UnitTests/Test_LZ77.cpp\
	$(SRCDIR)/UnitTests/Test_StringUtils.cpp\
	$(SRCDIR)/UnitTests/Test_TBinaryProtocol.cpp\
	$(SRCDIR)/UnitTests/Test_TCachedFileStream.cpp\
	$(SRCDIR)/UnitTests/Test_TCachedMemoryStream.cpp\
	$(SRCDIR)/UnitTests/Test_TCachedWindowsBmp.cpp\
	$(SRCDIR)/UnitTests/Test_TCanvas.cpp\
	$(SRCDIR)/UnitTests/Test_TColor.cpp\
	$(SRCDIR)/UnitTests/Test_TCompression.cpp\
	$(SRCDIR)/UnitTests/Test_TCustomString.cpp\
	$(SRCDIR)/UnitTests/Test_TDate.cpp\
	$(SRCDIR)/UnitTests/Test_TDateTime.cpp\
	$(SRCDIR)/UnitTests/Test_TDateTimeCounter.cpp\
	$(SRCDIR)/UnitTests/Test_TDictionary.cpp\
	$(SRCDIR)/UnitTests/Test_TEndlessLine.cpp\
	$(SRCDIR)/UnitTests/Test_TEventSender.cpp\
	$(SRCDIR)/UnitTests/Test_TFilePath.cpp\
	$(SRCDIR)/UnitTests/Test_TFileStream.cpp\
	$(SRCDIR)/UnitTests/Test_TFixedPoint1024.cpp\
	$(SRCDIR)/UnitTests/Test_TFixedPoint128.cpp\
	$(SRCDIR)/UnitTests/Test_TFixedPoint16M.cpp\
	$(SRCDIR)/UnitTests/Test_TFont.cpp\
	$(SRCDIR)/UnitTests/Test_TGraphics.cpp\
	$(SRCDIR)/UnitTests/Test_TJsonDoc.cpp\
	$(SRCDIR)/UnitTests/Test_TList.cpp\
	$(SRCDIR)/UnitTests/Test_TLog.cpp\
	$(SRCDIR)/UnitTests/Test_TMemoryStream.cpp\
	$(SRCDIR)/UnitTests/Test_TObjectList.cpp\
	$(SRCDIR)/UnitTests/Test_TParamString.cpp\
	$(SRCDIR)/UnitTests/Test_TQueue.cpp\
	$(SRCDIR)/UnitTests/Test_TRandom.cpp\
	$(SRCDIR)/UnitTests/Test_TShortString.cpp\
	$(SRCDIR)/UnitTests/Test_TSortedDictionary.cpp\
	$(SRCDIR)/UnitTests/Test_TStack.cpp\
	$(SRCDIR)/UnitTests/Test_TStaticDictionary.cpp\
	$(SRCDIR)/UnitTests/Test_TStaticList.cpp\
	$(SRCDIR)/UnitTests/Test_TString.cpp\
	$(SRCDIR)/UnitTests/Test_TStringList.cpp\
	$(SRCDIR)/UnitTests/Test_TTextFile.cpp\
	$(SRCDIR)/UnitTests/Test_TTime.cpp\
	$(SRCDIR)/UnitTests/Test_TWideString.cpp\
	$(SRCDIR)/UnitTests/Test_TWindowsBmpFile.cpp\
	$(SRCDIR)/UnitTests/Test_TXMLDoc.cpp\
	$(SRCDIR)/UnitTests/Test_TXMLParser.cpp\
	$(SRCDIR)/UnitTests/UnitTests.cpp\
	$(SRCDIR)/UnitTests/Fonts/Amata13.cpp\
	$(SRCDIR)/UnitTests/Fonts/Bookman50.cpp\
	$(SRCDIR)/UnitTests/Fonts/BrushScript12.cpp\
	$(SRCDIR)/UnitTests/Fonts/Robotron.cpp\
	$(SRCDIR)/UnitTests/Fonts/ZXSpectrum.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/BaseClass.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/ClassA.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/ClassB.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/ClassC.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/ClassD.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/ClassManagerA.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/ClassManagerB.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/ClassManagerC.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/ClassManagerD.cpp\
	$(SRCDIR)/UnitTests/PersistenceExample/MainObject.cpp

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
	@if [ ! -d $(TMPDIR) ];\
	then \
		$(MD) $(TMPDIR);\
	fi
	@$(LD) $(OBJ) $(LIBDIR) $(LIBS) -o $@
	@echo $(OUTPUTFILE)	
	@$(OS) $@
	
$(CPPOBJ): %.o: %.cpp
	@echo $<;
	@$(CPP) -c $(INC) $(CPPFLAGS) $< -o $@

$(COBJ): %.o: %.c
	@echo $<;
	@$(CC) -c $(INC) $(CFLAGS) $< -o $@

$(SOBJ): %.o: %.s
	@echo $<;
	@$(AS) -c $(ASFLAGS) $< -o $@

clean:
	@rm -f $(OUTPUTFILE) $(OBJ)

