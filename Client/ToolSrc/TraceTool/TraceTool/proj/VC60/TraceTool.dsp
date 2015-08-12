# Microsoft Developer Studio Project File - Name="TraceTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TraceTool - Win32 DebugStatic
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TraceTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TraceTool.mak" CFG="TraceTool - Win32 DebugStatic"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TraceTool - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "TraceTool - Win32 DebugStatic" (based on "Win32 (x86) Static Library")
!MESSAGE "TraceTool - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TraceTool - Win32 ReleaseStatic" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TraceTool - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug\TraceTool.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\TraceToolVC60D.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=COPY /Y ..\..\Trace.h ..\inc\Trace.h
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TraceTool - Win32 DebugStatic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugStatic"
# PROP BASE Intermediate_Dir "DebugStatic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "DebugStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"DebugStatic\TraceTool.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\TraceToolVC60DS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=COPY /Y ..\..\Trace.h ..\inc\Trace.h
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TraceTool - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release\TraceTool.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\TraceToolVC60R.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=COPY /Y ..\..\Trace.h ..\inc\Trace.h
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TraceTool - Win32 ReleaseStatic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseStatic"
# PROP BASE Intermediate_Dir "ReleaseStatic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "ReleaseStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"ReleaseStatic\TraceTool.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\TraceToolVC60RS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=COPY /Y ..\..\Trace.h ..\inc\Trace.h
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TraceTool - Win32 Debug"
# Name "TraceTool - Win32 DebugStatic"
# Name "TraceTool - Win32 Release"
# Name "TraceTool - Win32 ReleaseStatic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Trace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Trace.h
# End Source File
# Begin Source File

SOURCE=..\..\TraceTool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\TraceTool.h
# End Source File
# End Group
# End Target
# End Project
