# Microsoft Developer Studio Project File - Name="F_Socket" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=F_Socket - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "F_Socket.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "F_Socket.mak" CFG="F_Socket - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "F_Socket - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "F_Socket - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "F_Socket - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\F_SocketR.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=COPY /Y ".\*.h" ".\..\include\*.h"	COPY /Y ".\Release\F_SocketR.lib" ".\..\Lib\F_SocketR.lib"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "F_Socket - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\F_SocketD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=COPY /Y ".\*.h" ".\..\include\*.h"	COPY /Y ".\Debug\F_SocketD.lib" ".\..\Lib\F_SocketD.lib"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "F_Socket - Win32 Release"
# Name "F_Socket - Win32 Debug"
# Begin Group "기본클래스"

# PROP Default_Filter ""
# Begin Group "스레드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\F_Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\F_Thread.h
# End Source File
# End Group
# Begin Group "소켓"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\F_CSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\F_CSocket.h
# End Source File
# End Group
# Begin Group "크리티컬 세션"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\F_Critical.cpp
# End Source File
# Begin Source File

SOURCE=.\F_Critical.h
# End Source File
# End Group
# Begin Group "버퍼"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\F_Buffer_L.cpp
# End Source File
# Begin Source File

SOURCE=.\F_Buffer_L.h
# End Source File
# End Group
# End Group
# Begin Group "소켓스레드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\F_WSThread.cpp
# End Source File
# Begin Source File

SOURCE=.\F_WSThread.h
# End Source File
# End Group
# Begin Group "이벤트 소켓"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\F_CSocket_Event.cpp
# End Source File
# Begin Source File

SOURCE=.\F_CSocket_Event.h
# End Source File
# End Group
# End Target
# End Project
