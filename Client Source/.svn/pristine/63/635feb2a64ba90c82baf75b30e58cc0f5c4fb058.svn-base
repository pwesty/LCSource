# Microsoft Developer Studio Project File - Name="tinyxml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=tinyxml - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tinyxml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tinyxml.mak" CFG="tinyxml - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tinyxml - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "tinyxml - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tinyxml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_Release"
# PROP Intermediate_Dir "_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Custom Build
InputPath=.\_Release\tinyxml.lib
InputName=tinyxml
SOURCE="$(InputPath)"

"..\..\lib\$(inputName).lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _Release\$(InputName).lib ..\..\lib >nul

# End Custom Build

!ELSEIF  "$(CFG)" == "tinyxml - Win32 Debug"

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
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Custom Build
InputPath=.\Debug\tinyxml.lib
InputName=tinyxml
SOURCE="$(InputPath)"

"..\..\lib\$(inputName).lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _Debug\$(InputName).lib ..\..\lib >nul

# End Custom Build

!ENDIF 

# Begin Target

# Name "tinyxml - Win32 Release"
# Name "tinyxml - Win32 Debug"
# Begin Source File

SOURCE=.\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxmlparser.cpp
# End Source File
# End Target
# End Project
