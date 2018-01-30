# Microsoft Developer Studio Project File - Name="Graphics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Graphics - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Graphics.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Graphics.mak" CFG="Graphics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Graphics - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Graphics - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Graphics - Win32 Release"

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

!ELSEIF  "$(CFG)" == "Graphics - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\lib\GraphicsD.lib"

!ENDIF 

# Begin Target

# Name "Graphics - Win32 Release"
# Name "Graphics - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TCachedWindowsBmpFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TCanvas.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TColorConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TColorRGB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TColorYUV.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TDisplay320x240x65536.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TDxtBlockCreator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFilter3x3.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFilter5x5.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFilter7x7.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFontCharacter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TGraphicsData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TPixelFormatConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TPosition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TRectangle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TSize.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TWindowsBmpFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\Source\Graphics\ePixelFormat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TAlign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TCachedWindowsBmpFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TCanvas.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TColorConverter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TColorRGB.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TColorYUV.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TDisplay320x240x65536.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TDxtBlockCreator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFilter3x3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFilter5x5.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFilter7x7.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFont.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TFontCharacter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TGraphicsData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TPixelFormatConverter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TPosition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TRectangle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TSize.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TVerticalAlign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Graphics\TWindowsBmpFile.h
# End Source File
# End Group
# End Target
# End Project
