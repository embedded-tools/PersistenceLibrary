PROJECT = PersistenceLibrary
OUTPUTFILE =$(PROJECT).a

CC	= gcc
CPP	= g++
AS	= as
LD	= ld
CP	= objcopy
OS	= size
OD	= objdump

INC = -I../../Source/BasicTypes\
      -I../../Source/Collections\
      -I../../Source/FileUtils\
      -I../../Source/Graphics\
      -I../../Source/Logs\
      -I../../Source/Math\
      -I../../Source/Networking\
      -I../../Source/Persistence\
      -I../../Source/Protocols\
      -I../../Source/Signaling\
      -I../../Source/UnitTests\


SRC = ../../Source/BasicTypes/EscapeSequences.c\
      ../../Source/BasicTypes/UTF8.c\
      ../../Source/BasicTypes/TDate.cpp\
      ../../Source/BasicTypes/TDateTime.cpp\
      ../../Source/BasicTypes/TParamString.cpp\
      ../../Source/BasicTypes/TStringList.cpp\
      ../../Source/BasicTypes/TString.cpp\
      ../../Source/BasicTypes/TWideString.cpp\
      ../../Source/BasicTypes/TRandom.cpp\
      ../../Source/basicTypes/TDateTimeCounter.cpp\
      ../../Source/BasicTypes/TTime.cpp\
      ../../Source/BasicTypes/TShortString.cpp\
      ../../Source/BasicTypes/TFilePath.cpp\
      ../../Source/BasicTypes/StringUtils.cpp\
      ../../Source/Collections/TEnumerator.cpp\
      ../../Source/Collections/TEnumerable.cpp\
      ../../Source/Collections/TSortedDictionary.cpp\
      ../../Source/Collections/TStaticDictionary.cpp\
      ../../Source/Collections/TStaticList.cpp\
      ../../Source/Collections/TStack.cpp\
      ../../Source/Collections/TDictionary.cpp\
      ../../Source/Collections/TQueue.cpp\
      ../../Source/Collections/TList.cpp\
      ../../Source/Collections/TObjectList.cpp\
      ../../Source/Collections/TArray.cpp\
      ../../Source/FileUtils/TCachedStream.cpp\
      ../../Source/FileUtils/TFileStream.cpp\
      ../../Source/FileUtils/THandleStream.cpp\
      ../../Source/FileUtils/TLZ77Streamed.cpp\
      ../../Source/FileUtils/TEEPROMStream.cpp\
      ../../Source/FileUtils/TJsonDoc.cpp\
      ../../Source/FileUtils/TJsonTag.cpp\
      ../../Source/FileUtils/TJsonTagBasePool.cpp\
      ../../Source/FileUtils/TJsonTagStaticPool.cpp\
      ../../Source/FileUtils/TJsonTagDynamicPool.cpp\
      ../../Source/FileUtils/TLZ77.cpp\
      ../../Source/FileUtils/TMemoryStream.cpp\
      ../../Source/FileUtils/TROMStream.cpp\
      ../../Source/FileUtils/TStream.cpp\
      ../../Source/FileUtils/TTextFile.cpp\
      ../../Source/FileUtils/TXmlDoc.cpp\
      ../../Source/FileUtils/TXmlTag.cpp\
      ../../Source/FileUtils/TXmlTagBasePool.cpp\
      ../../Source/FileUtils/TXmlTagDynamicPool.cpp\
      ../../Source/FileUtils/TXmlTagStaticPool.cpp\
      ../../Source/Graphics/TCanvas.cpp\
      ../../Source/Graphics/TCachedWindowsBmpFile.cpp\
      ../../Source/Graphics/TColorConverter.cpp\
      ../../Source/Graphics/TColorRGB.cpp\
      ../../Source/Graphics/TColorYUV.cpp\
      ../../Source/Graphics/TDisplay320x240x65536.cpp\
      ../../Source/Graphics/TDxtBlockCreator.cpp\
      ../../Source/Graphics/TEndlessLine.cpp\
      ../../Source/Graphics/TFilter3x3.cpp\
      ../../Source/Graphics/TFilter5x5.cpp\
      ../../Source/Graphics/TFilter7x7.cpp\
      ../../Source/Graphics/TFont.cpp\
      ../../Source/Graphics/TFontCharacter.cpp\
      ../../Source/Graphics/TGraphicsData.cpp\
      ../../Source/Graphics/TPixelFormatConverter.cpp\
      ../../Source/Graphics/TPosition.cpp\
      ../../Source/Graphics/TRectangle.cpp\
      ../../Source/Graphics/TSize.cpp\
      ../../Source/Graphics/TVector.cpp\
      ../../Source/Graphics/TWindowsBmpFile.cpp\
      ../../Source/Logs/TConsoleLog.cpp\
      ../../Source/Logs/TCustomLog.cpp\
      ../../Source/Logs/TFileLog.cpp\
      ../../Source/Logs/TLogBase.cpp\
      ../../Source/Math/TFixedPoint16M.cpp\
      ../../Source/Math/TFixedPoint128.cpp\
      ../../Source/Math/TFixedPoint1024.cpp\
      ../../Source/Persistence/TMainSerializer.cpp\
      ../../Source/Persistence/TObjectFactory.cpp\
      ../../Source/Persistence/TSerializedBaseCollection.cpp\
      ../../Source/Persistence/TSerializedItem.cpp\
      ../../Source/Persistence/TSerializer.cpp\
      ../../Source/Persistence/TXMLCache.cpp\
      ../../Source/Persistence/TXMLParser.cpp\
      ../../Source/Persistence/TXMLTypes.cpp\
      ../../Source/Protocols/Sha1.c\
      ../../Source/Protocols/TBinaryProtocol.cpp\
      ../../Source/Protocols/TTextProtocol.cpp\
      ../../Source/Protocols/TUserManager.cpp\
      ../../Source/Signaling/TEventReceiver.cpp\
      ../../Source/Signaling/TEventSender.cpp\
      ../../Source/Signaling/TEventSerializer.cpp\


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
	@echo Archiving
	@$(AR) rc $(OUTPUTFILE) $(OBJ)
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
	@rm -f $(OUTPUTFILE) $(OBJ)

