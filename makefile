PROJECT = unittests

CC      = arm-none-eabi-gcc
CPP     = arm-none-eabi-g++
LD      = arm-none-eabi-g++

CPPFLAGS = -g -std=c++11                   
CFLAGS   = -g -std=c99
#LDFLAGS = 

INC=-I./\
    -I./Source/BasicTypes\
    -I./Source/Collections\
    -I./Source/FileUtils\
    -I./Source/Graphics\
    -I./Source/Math\
    -I./Source/Persistence\
    -I./Source/Protocols\
    -I./Source/UnitTests\
	-I./Source/UnitTests/Fonts\
    -I./Source/UnitTests\PersistenceExample

SRC=./Source/BasicTypes/StringUtils.cpp\
	./Source/BasicTypes/TDate.cpp\
	./Source/BasicTypes/TDateTime.cpp\
	./Source/BasicTypes/TDateTimeCounter.cpp\
	./Source/BasicTypes/TFilePath.cpp\
	./Source/BasicTypes/TParamString.cpp\
	./Source/BasicTypes/TRandom.cpp\
	./Source/BasicTypes/TShortString.cpp\
	./Source/BasicTypes/TString.cpp\
	./Source/BasicTypes/TStringList.cpp\
	./Source/BasicTypes/TTime.cpp\
	./Source/BasicTypes/TWideString.cpp\
	./Source/BasicTypes/UTF8.c\
	./Source/Collections/TArray.cpp\
	./Source/Collections/TDictionary.cpp\
	./Source/Collections/TEnumerable.cpp\
	./Source/Collections/TEnumerator.cpp\
	./Source/Collections/TList.cpp\
	./Source/Collections/TObjectList.cpp\
	./Source/Collections/TQueue.cpp\
	./Source/Collections/TSafeArray.cpp\
	./Source/Collections/TSortedDictionary.cpp\
	./Source/Collections/TStack.cpp\
	./Source/Collections/TStaticDictionary.cpp\
	./Source/Collections/TStaticList.cpp\
	./Source/FileUtils/TCachedStream.cpp\
	./Source/FileUtils/TEEPROMStream.cpp\
	./Source/FileUtils/TFileLog.cpp\
	./Source/FileUtils/TFileStream.cpp\
	./Source/FileUtils/THandleStream.cpp\
	./Source/FileUtils/TLog.cpp\
	./Source/FileUtils/TLZ77.cpp\
	./Source/FileUtils/TLZ77Streamed.cpp\
	./Source/FileUtils/TMemoryStream.cpp\
	./Source/FileUtils/TROMStream.cpp\
	./Source/FileUtils/TStream.cpp\
	./Source/FileUtils/TTextFile.cpp\
	./Source/FileUtils/TXmlDoc.cpp\
	./Source/FileUtils/TXmlTag.cpp\
	./Source/FileUtils/TXmlTagBasePool.cpp\
	./Source/FileUtils/TXmlTagDynamicPool.cpp\
	./Source/FileUtils/TXmlTagStaticPool.cpp\
	./Source/Graphics/TCachedWindowsBmpFile.cpp\
	./Source/Graphics/TCanvas.cpp\
	./Source/Graphics/TColorConverter.cpp\
	./Source/Graphics/TColorRGB.cpp\
	./Source/Graphics/TColorYUV.cpp\
	./Source/Graphics/TDisplay320x240x65536.cpp\
	./Source/Graphics/TDxtBlockCreator.cpp\
	./Source/Graphics/TEndlessLine.cpp\
	./Source/Graphics/TFilter3x3.cpp\
	./Source/Graphics/TFilter5x5.cpp\
	./Source/Graphics/TFilter7x7.cpp\
	./Source/Graphics/TFont.cpp\
	./Source/Graphics/TFontCharacter.cpp\
	./Source/Graphics/TGraphicsData.cpp\
	./Source/Graphics/TPixelFormatConverter.cpp\
	./Source/Graphics/TPosition.cpp\
	./Source/Graphics/TRectangle.cpp\
	./Source/Graphics/TSize.cpp\
	./Source/Graphics/TVector.cpp\
	./Source/Graphics/TWindowsBmpFile.cpp\
	./Source/Math/TFixedPoint1024.cpp\
	./Source/Math/TFixedPoint128.cpp\
	./Source/Math/TFixedPoint16M.cpp\
	./Source/Persistence/TMainSerializer.cpp\
	./Source/Persistence/TObjectFactory.cpp\
	./Source/Persistence/TSerializedBaseCollection.cpp\
	./Source/Persistence/TSerializedItem.cpp\
	./Source/Persistence/TSerializer.cpp\
	./Source/Persistence/TXMLCache.cpp\
	./Source/Persistence/TXMLParser.cpp\
	./Source/Persistence/TXMLTypes.cpp\
	./Source/Protocols/TBinaryProtocol.cpp\
	./Source/UnitTests/Fonts/Amata13.cpp\
	./Source/UnitTests/Fonts/Bookman50.cpp\
	./Source/UnitTests/Fonts/BrushScript12.cpp\
	./Source/UnitTests/Fonts/Robotron.cpp\
	./Source/UnitTests/Fonts/ZXSpectrum.cpp\
 	./Source/UnitTests/Test_LZ77.cpp\
	./Source/UnitTests/Test_Persistence.cpp\
	./Source/UnitTests/Test_StringUtils.cpp\
	./Source/UnitTests/Test_TBinaryProtocol.cpp\
	./Source/UnitTests/Test_TCachedFileStream.cpp\
	./Source/UnitTests/Test_TCachedMemoryStream.cpp\
	./Source/UnitTests/Test_TColor.cpp\
	./Source/UnitTests/Test_TCompression.cpp\
	./Source/UnitTests/Test_TCustomString.cpp\
	./Source/UnitTests/Test_TDate.cpp\
	./Source/UnitTests/Test_TDateTime.cpp\
	./Source/UnitTests/Test_TDateTimeCounter.cpp\
	./Source/UnitTests/Test_TDictionary.cpp\
	./Source/UnitTests/Test_Template.cpp\
	./Source/UnitTests/Test_TFilePath.cpp\
	./Source/UnitTests/Test_TFileStream.cpp\
	./Source/UnitTests/Test_TFixedPoint1024.cpp\
	./Source/UnitTests/Test_TFixedPoint128.cpp\
	./Source/UnitTests/Test_TFixedPoint16M.cpp\
	./Source/UnitTests/Test_TFont.cpp\
	./Source/UnitTests/Test_TGraphics.cpp\
	./Source/UnitTests/Test_TList.cpp\
	./Source/UnitTests/Test_TLog.cpp\
	./Source/UnitTests/Test_TMemoryStream.cpp\
	./Source/UnitTests/Test_TObjectList.cpp\
	./Source/UnitTests/Test_TQueue.cpp\
	./Source/UnitTests/Test_TRandom.cpp\
	./Source/UnitTests/Test_TShortString.cpp\
	./Source/UnitTests/Test_TSortedDictionary.cpp\
	./Source/UnitTests/Test_TStaticDictionary.cpp\
	./Source/UnitTests/Test_TStaticList.cpp\
	./Source/UnitTests/Test_TString.cpp\
	./Source/UnitTests/Test_TStringList.cpp\
	./Source/UnitTests/Test_TTextFile.cpp\
	./Source/UnitTests/Test_TTime.cpp\
	./Source/UnitTests/Test_TWideString.cpp\
	./Source/UnitTests/Test_TWindowsBmpFile.cpp\
	./Source/UnitTests/Test_TXMLDoc.cpp\
	./Source/UnitTests/Test_TXMLParser.cpp\
	./Source/UnitTests/Test_TDateTimeCounter.cpp\
	./Source/UnitTests/UnitTests.cpp\
	./Source/UnitTests/PersistenceExample/BaseClass.cpp\
	./Source/UnitTests/PersistenceExample/ClassA.cpp\
	./Source/UnitTests/PersistenceExample/ClassB.cpp\
	./Source/UnitTests/PersistenceExample/ClassC.cpp\
	./Source/UnitTests/PersistenceExample/ClassD.cpp\
	./Source/UnitTests/PersistenceExample/ClassManagerA.cpp\
	./Source/UnitTests/PersistenceExample/ClassManagerB.cpp\
	./Source/UnitTests/PersistenceExample/ClassManagerC.cpp\
	./Source/UnitTests/PersistenceExample/ClassManagerD.cpp\
	./Source/UnitTests/PersistenceExample/MainObject.cpp\

    	
#  C++ source files
CPPFILES = $(filter %.cpp, $(SRC))

#  C source files
CFILES = $(filter %.c, $(SRC))    

# Object files
CPPOBJ = $(CPPFILES:.cpp=.o)
COBJ = $(CFILES:.c=.o)
OBJ  = $(CPPOBJ) $(COBJ) $(SOBJ)
    
all: $(SRC)	
	$(CPP) $(INC) $(CPPFILES) $(CPPFLAGS) -c
	$(CC) $(INC) $(CFILES) $(CFLAGS) -c
	$(LD) $(LDFLAGS) $(OBJ) -o $(PROJECT).exe
    
clean:
	rm -f $(PROJECT).exe $(OBJ)

