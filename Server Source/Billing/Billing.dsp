# Microsoft Developer Studio Project File - Name="Billing" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Billing - Win32 UniBWTEST
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Billing.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Billing.mak" CFG="Billing - Win32 UniBWTEST"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Billing - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Billing - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Billing - Win32 BWTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "Billing - Win32 UniBWTEST" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/T-EnterBilling", AEBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Billing - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Billing - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /out:"Billing_d.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Billing - Win32 BWTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Billing___Win32_BWTEST"
# PROP BASE Intermediate_Dir "Billing___Win32_BWTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Billing_bw"
# PROP Intermediate_Dir "Billing_bw"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "BSTEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "BWTEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /out:"Billing_d.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /out:"Billing_d.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Billing - Win32 UniBWTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Billing___Win32_UniBWTEST"
# PROP BASE Intermediate_Dir "Billing___Win32_UniBWTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Billing_UniBWTEST"
# PROP Intermediate_Dir "Billing_UniBWTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "BWTEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "BWTEST" /D "_UNICODE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /out:"Billing_d.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /out:"Billing_d.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Billing - Win32 Release"
# Name "Billing - Win32 Debug"
# Name "Billing - Win32 BWTEST"
# Name "Billing - Win32 UniBWTEST"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Billing.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Compression.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DBCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\DBFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Descriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\doFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\NetMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Random.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerRun.cpp
# End Source File
# Begin Source File

SOURCE=.\UserList.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Cmd.h
# End Source File
# Begin Source File

SOURCE=.\CmdMsg.h
# End Source File
# Begin Source File

SOURCE=.\Compression.h
# End Source File
# Begin Source File

SOURCE=.\Conf.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\ConfigFile.h
# End Source File
# Begin Source File

SOURCE=.\CryptMem.h
# End Source File
# Begin Source File

SOURCE=.\DBCmd.h
# End Source File
# Begin Source File

SOURCE=.\Descriptor.h
# End Source File
# Begin Source File

SOURCE=.\doFunc.h
# End Source File
# Begin Source File

SOURCE=.\InputBuffer.h
# End Source File
# Begin Source File

SOURCE=.\InputQueue.h
# End Source File
# Begin Source File

SOURCE=.\LCString.h
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\MemPool.h
# End Source File
# Begin Source File

SOURCE=.\MessageType.h
# End Source File
# Begin Source File

SOURCE=.\Mutex.h
# End Source File
# Begin Source File

SOURCE=.\MyOLEDB.h
# End Source File
# Begin Source File

SOURCE=.\NetMsg.h
# End Source File
# Begin Source File

SOURCE=.\OutputBuffer.h
# End Source File
# Begin Source File

SOURCE=.\PrepayInfo.h
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# Begin Source File

SOURCE=.\stdhdrs.h
# End Source File
# Begin Source File

SOURCE=.\Sysdep.h
# End Source File
# Begin Source File

SOURCE=.\UserList.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h
# End Source File
# End Group
# Begin Group "Localize"

# PROP Default_Filter "inc;inh"
# Begin Group "KOR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BillMessage_KOR.inh
# End Source File
# Begin Source File

SOURCE=.\Config_KOR.inh
# End Source File
# Begin Source File

SOURCE=.\MessageType_KOR.inh
# End Source File
# End Group
# Begin Group "TWN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BillMessage_TWN.inh
# End Source File
# Begin Source File

SOURCE=.\Config_TWN.inh
# End Source File
# Begin Source File

SOURCE=.\MessageType_TWN.inh
# End Source File
# End Group
# Begin Group "CHN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Config_CHN.inh
# End Source File
# End Group
# Begin Group "TLD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BillMessage_TLD.inh
# End Source File
# Begin Source File

SOURCE=.\Config_TLD.inh
# End Source File
# Begin Source File

SOURCE=.\MessageType_TLD.inh
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\newStobm.bin
# End Source File
# End Target
# End Project
