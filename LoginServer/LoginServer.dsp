# Microsoft Developer Studio Project File - Name="LoginServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=LoginServer - Win32 PLD
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LoginServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LoginServer.mak" CFG="LoginServer - Win32 PLD"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LoginServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 TEST_SERVER_Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 KYDTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 WJKTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 WJKTEST_BRZ" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 holylance_JPN" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 HOLYLANCE_TEST" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 HOLYLANCE_RUS" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 PGMAN_MAL" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 HOLYLANCE_USA" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 swkwonTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 MEXICO" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 WORLD_TOURNAMENT" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 ESP_USA" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 JPNEP" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 HK" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 KJRTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 TestServer" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 DEREK" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 TURKEY" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 ITALY" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 GER" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 SPN" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 FRC" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginServer - Win32 PLD" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LoginServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "zlib" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "LoginServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "DEREK_DEBUG_DONT_SOAPHELPER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 TEST_SERVER_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_TEST_SERVER_Debug"
# PROP BASE Intermediate_Dir "LoginServer___Win32_TEST_SERVER_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "TEST_SERVER_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_td.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 KYDTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_KYDTEST"
# PROP BASE Intermediate_Dir "LoginServer___Win32_KYDTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_HBK=11 /D "TEST_SERVER" /D "KJTEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_kj.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 WJKTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_WJKTEST"
# PROP BASE Intermediate_Dir "LoginServer___Win32_WJKTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "WJKTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "WJKTEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 WJKTEST_BRZ"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_WJKTEST_BRZ"
# PROP BASE Intermediate_Dir "LoginServer___Win32_WJKTEST_BRZ"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "WJKTEST_BRZ"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "WJKTEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_BRZ=10 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_brz.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 holylance_JPN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_holylance_JPN"
# PROP BASE Intermediate_Dir "LoginServer___Win32_holylance_JPN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_holylance_JPN"
# PROP Intermediate_Dir "LoginServer___Win32_holylance_JPN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_kj.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_jpn.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 HOLYLANCE_TEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_HOLYLANCE_TEST"
# PROP BASE Intermediate_Dir "LoginServer___Win32_HOLYLANCE_TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "HOLYLANCE_TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "WJKTEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "HOLYLANCE_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 HOLYLANCE_RUS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_HOLYLANCE_RUS"
# PROP BASE Intermediate_Dir "LoginServer___Win32_HOLYLANCE_RUS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_HOLYLANCE_RUS"
# PROP Intermediate_Dir "LoginServer___Win32_HOLYLANCE_RUS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_PLD=16 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_RUS=17 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_imso.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_rus.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 PGMAN_MAL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_PGMAN_MAL"
# PROP BASE Intermediate_Dir "LoginServer___Win32_PGMAN_MAL"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_PGMAN_MAL"
# PROP Intermediate_Dir "LoginServer___Win32_PGMAN_MAL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MAL=7 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 HOLYLANCE_USA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_HOLYLANCE_USA"
# PROP BASE Intermediate_Dir "LoginServer___Win32_HOLYLANCE_USA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_HOLYLANCE_USA"
# PROP Intermediate_Dir "LoginServer___Win32_HOLYLANCE_USA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 swkwonTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_swkwonTEST"
# PROP BASE Intermediate_Dir "LoginServer___Win32_swkwonTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_swkwonTEST"
# PROP Intermediate_Dir "LoginServer___Win32_swkwonTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 MEXICO"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_MEXICO"
# PROP BASE Intermediate_Dir "LoginServer___Win32_MEXICO"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_MEXICO"
# PROP Intermediate_Dir "LoginServer___Win32_MEXICO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_RUS=17 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MEX=20 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_rus.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_mex.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 WORLD_TOURNAMENT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_WORLD_TOURNAMENT"
# PROP BASE Intermediate_Dir "LoginServer___Win32_WORLD_TOURNAMENT"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_WORLD_TOURNAMENT"
# PROP Intermediate_Dir "LoginServer___Win32_WORLD_TOURNAMENT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /D WORLD_TOURNAMENT=100 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 ESP_USA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_ESP_USA"
# PROP BASE Intermediate_Dir "LoginServer___Win32_ESP_USA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_ESP_USA"
# PROP Intermediate_Dir "LoginServer___Win32_ESP_USA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MEX=20 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_ESP_USA=21 /D "DEREK_INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_mex.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_espusa.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 JPNEP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_JPNEP"
# PROP BASE Intermediate_Dir "LoginServer___Win32_JPNEP"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_JPNEP"
# PROP Intermediate_Dir "LoginServer___Win32_JPNEP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /D "DEREK_DEBUG_DONT_SOAPHELPER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN_EP=6 /D "DEREK_DEBUG_DONT_SOAPHELPER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_jpn.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_jpnep.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 HK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_HK"
# PROP BASE Intermediate_Dir "LoginServer___Win32_HK"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_HK"
# PROP Intermediate_Dir "LoginServer___Win32_HK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_HBK=11 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /pdb:"LoginServer.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 KJRTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_KJRTEST"
# PROP BASE Intermediate_Dir "LoginServer___Win32_KJRTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "KJRTEST"
# PROP Intermediate_Dir "KJRTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "KJR_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_td.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 TestServer"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_TestServer0"
# PROP BASE Intermediate_Dir "LoginServer___Win32_TestServer0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_TestServer"
# PROP Intermediate_Dir "LoginServer___Win32_TestServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_td.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_td.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 DEREK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_DEREK"
# PROP BASE Intermediate_Dir "LoginServer___Win32_DEREK"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_DEREK"
# PROP Intermediate_Dir "LoginServer___Win32_DEREK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_td.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_derek.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 TURKEY"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_TURKEY"
# PROP BASE Intermediate_Dir "LoginServer___Win32_TURKEY"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_TURKEY"
# PROP Intermediate_Dir "LoginServer___Win32_TURKEY"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "KJR_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_TUR=18 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 ITALY"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_ITALY"
# PROP BASE Intermediate_Dir "LoginServer___Win32_ITALY"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_ITALY"
# PROP Intermediate_Dir "LoginServer___Win32_ITALY"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "KJR_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_ITA=19 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 GER"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_GER"
# PROP BASE Intermediate_Dir "LoginServer___Win32_GER"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_GER"
# PROP Intermediate_Dir "LoginServer___Win32_GER"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_td.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_ger.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 SPN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_SPN"
# PROP BASE Intermediate_Dir "LoginServer___Win32_SPN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_SPN"
# PROP Intermediate_Dir "LoginServer___Win32_SPN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_SPN=14 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_ger.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_spn.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 FRC"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_FRC"
# PROP BASE Intermediate_Dir "LoginServer___Win32_FRC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_FRC"
# PROP Intermediate_Dir "LoginServer___Win32_FRC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GFRC=15 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_ger.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_frc.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginServer - Win32 PLD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginServer___Win32_PLD"
# PROP BASE Intermediate_Dir "LoginServer___Win32_PLD"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginServer___Win32_PLD"
# PROP Intermediate_Dir "LoginServer___Win32_PLD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_PLD=16 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_ger.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"LoginServer_pld.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "LoginServer - Win32 Release"
# Name "LoginServer - Win32 Debug"
# Name "LoginServer - Win32 TEST_SERVER_Debug"
# Name "LoginServer - Win32 KYDTEST"
# Name "LoginServer - Win32 WJKTEST"
# Name "LoginServer - Win32 WJKTEST_BRZ"
# Name "LoginServer - Win32 holylance_JPN"
# Name "LoginServer - Win32 HOLYLANCE_TEST"
# Name "LoginServer - Win32 HOLYLANCE_RUS"
# Name "LoginServer - Win32 PGMAN_MAL"
# Name "LoginServer - Win32 HOLYLANCE_USA"
# Name "LoginServer - Win32 swkwonTEST"
# Name "LoginServer - Win32 MEXICO"
# Name "LoginServer - Win32 WORLD_TOURNAMENT"
# Name "LoginServer - Win32 ESP_USA"
# Name "LoginServer - Win32 JPNEP"
# Name "LoginServer - Win32 HK"
# Name "LoginServer - Win32 KJRTEST"
# Name "LoginServer - Win32 TestServer"
# Name "LoginServer - Win32 DEREK"
# Name "LoginServer - Win32 TURKEY"
# Name "LoginServer - Win32 ITALY"
# Name "LoginServer - Win32 GER"
# Name "LoginServer - Win32 SPN"
# Name "LoginServer - Win32 FRC"
# Name "LoginServer - Win32 PLD"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

SOURCE=.\Descriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\IPCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginServer.cpp
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

SOURCE=.\Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitPlayerList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=.\InputBuffer.h
# End Source File
# Begin Source File

SOURCE=.\InputQueue.h
# End Source File
# Begin Source File

SOURCE=.\IPCheck.h
# End Source File
# Begin Source File

SOURCE=.\LCList.h
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

SOURCE=.\NetMsg.h
# End Source File
# Begin Source File

SOURCE=.\OutputBuffer.h
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

SOURCE=.\Utils.h
# End Source File
# Begin Source File

SOURCE=.\WaitPlayerList.h
# End Source File
# End Group
# Begin Group "Setting Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\newStobm.bin
# End Source File
# Begin Source File

SOURCE=.\newStobm.bin.new
# End Source File
# End Group
# Begin Group "Makefile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Makefile
# End Source File
# End Group
# Begin Group "Localize"

# PROP Default_Filter "inc;inh"
# Begin Source File

SOURCE=.\Config_Localize.inh
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
