# Microsoft Developer Studio Project File - Name="UnitTests" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=UnitTests - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UnitTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UnitTests.mak" CFG="UnitTests - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UnitTests - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "UnitTests - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UnitTests - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "obj"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 collections.lib fileutils.lib persistence.lib strings.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "UnitTests - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "obj"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\Source\UnitTests\PersistenceExample" /I "..\..\..\Source\UnitTests\Fonts" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 basictypesd.lib collectionsd.lib fileutilsd.lib mathd.lib persistenced.lib protocolsd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "UnitTests - Win32 Release"
# Name "UnitTests - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "PersistenceExample"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\BaseClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassA.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassB.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassD.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassManagerA.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassManagerB.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassManagerC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassManagerD.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\MainObject.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\UnitTests.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "Persistence Example"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\BaseClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassD.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassManagerA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassManagerB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassManagerC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\ClassManagerD.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\PersistenceExample\MainObject.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\minicppunit.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_LZ77.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_Persistence.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_StringUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TBinaryProtocol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TCachedFileStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TCachedMemoryStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TColor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TCompression.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TCustomString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TDate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TDateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TDateTimeCounter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TFilePath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TFileStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TFixedPoint1024.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TFixedPoint128.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TFixedPoint16M.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TGraphics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TLog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TMemoryStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TRandom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TShortString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TSortedDictionary.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TStaticDictionary.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TStaticQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TStringList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TTextFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TWideString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TWindowsBmpFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TXMLDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\Test_TXMLParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\UnitTests\UnitTests.cpp
# End Source File
# End Group
# End Target
# End Project
