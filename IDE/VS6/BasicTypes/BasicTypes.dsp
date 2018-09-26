# Microsoft Developer Studio Project File - Name="BasicTypes" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BasicTypes - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BasicTypes.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BasicTypes.mak" CFG="BasicTypes - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BasicTypes - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BasicTypes - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BasicTypes - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x405 /d "NDEBUG"
# ADD RSC /l 0x405 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BasicTypes - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x405 /d "_DEBUG"
# ADD RSC /l 0x405 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\BasicTypesD.lib"

!ENDIF 

# Begin Target

# Name "BasicTypes - Win32 Release"
# Name "BasicTypes - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\EscapeSequences.c
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\StringUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TDate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TDateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TDateTimeCounter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TFilePath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TParamString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TRandom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TShortString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TStringList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TWideString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\UTF8.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\EscapeSequences.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\stringutils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\tcustomstring.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\tdate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\tdatetime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\tdatetimecounter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\tfilepath.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TParamString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TRandom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\tshortstring.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\tstring.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\TStringList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\ttime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\twidestring.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\BasicTypes\UTF8.h
# End Source File
# End Group
# End Target
# End Project
