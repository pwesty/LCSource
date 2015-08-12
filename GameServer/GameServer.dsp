# Microsoft Developer Studio Project File - Name="GameServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=GameServer - Win32 FRC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameServer.mak" CFG="GameServer - Win32 FRC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 TestServer_Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 WJKTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 KYDTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 WJKTEST_BRZ" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 holylance_JPN" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 HOLYLANCE_TEST" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 HOLYLANCE_RUS" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 PGMAN_MAL" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 HOLYLANCE_USA" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 FINWELL_TEST" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 swkwonTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 MEXICO" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 WORLD_TOURNAMENT" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 USA_ESP" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 HK" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 KJRTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 GER" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 DEREK" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 SPN" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 PLD" (based on "Win32 (x86) Console Application")
!MESSAGE "GameServer - Win32 FRC" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameServer - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "zlib" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "LACARETTE_SYSTEM" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_TestServer_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_TestServer_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "TestServer_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_t.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_WJKTEST"
# PROP BASE Intermediate_Dir "GameServer___Win32_WJKTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "WJKTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_KYDTEST"
# PROP BASE Intermediate_Dir "GameServer___Win32_KYDTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /D "_CRT_SECURE_NO_WARNINGS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_WJKTEST_BRZ"
# PROP BASE Intermediate_Dir "GameServer___Win32_WJKTEST_BRZ"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_brz.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_holylance_JPN"
# PROP BASE Intermediate_Dir "GameServer___Win32_holylance_JPN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_holylance_JPN"
# PROP Intermediate_Dir "GameServer___Win32_holylance_JPN"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_kj.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"D:\00_Source_Safe\Korea\Server\GameServer\GameServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_HOLYLANCE_TEST"
# PROP BASE Intermediate_Dir "GameServer___Win32_HOLYLANCE_TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "HOLYLANCE_TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "WJKTEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /D "HOLYLANCE_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_HOLYLANCE_RUS"
# PROP BASE Intermediate_Dir "GameServer___Win32_HOLYLANCE_RUS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_HOLYLANCE_RUS"
# PROP Intermediate_Dir "GameServer___Win32_HOLYLANCE_RUS"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_imso.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_rus.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_PGMAN_MAL0"
# PROP BASE Intermediate_Dir "GameServer___Win32_PGMAN_MAL0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_PGMAN_MAL"
# PROP Intermediate_Dir "GameServer___Win32_PGMAN_MAL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_SPN=14 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MAL=7 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_imso.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_HOLYLANCE_USA"
# PROP BASE Intermediate_Dir "GameServer___Win32_HOLYLANCE_USA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_HOLYLANCE_USA"
# PROP Intermediate_Dir "GameServer___Win32_HOLYLANCE_USA"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_FINWELL_TEST"
# PROP BASE Intermediate_Dir "GameServer___Win32_FINWELL_TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "FINWELL_TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_swkwonTEST"
# PROP BASE Intermediate_Dir "GameServer___Win32_swkwonTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_swkwonTEST"
# PROP Intermediate_Dir "GameServer___Win32_swkwonTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "SWKWON_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_MEXICO"
# PROP BASE Intermediate_Dir "GameServer___Win32_MEXICO"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_MEXICO"
# PROP Intermediate_Dir "GameServer___Win32_MEXICO"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_rus.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_mex.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_WORLD_TOURNAMENT"
# PROP BASE Intermediate_Dir "GameServer___Win32_WORLD_TOURNAMENT"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_WORLD_TOURNAMENT"
# PROP Intermediate_Dir "GameServer___Win32_WORLD_TOURNAMENT"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_USA_ESP"
# PROP BASE Intermediate_Dir "GameServer___Win32_USA_ESP"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_USA_ESP"
# PROP Intermediate_Dir "GameServer___Win32_USA_ESP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /D "DEREK_DEBUG_DONT_SOAPHELPER" /D "DEREK_TEST_ACTION" /D "DEREK_INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_ESP_USA=21 /D "DEREK_DEBUG_DONT_SOAPHELPER" /D "DEREK_TEST_ACTION" /D "DEREK_INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_t.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_t.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_HK"
# PROP BASE Intermediate_Dir "GameServer___Win32_HK"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_HK"
# PROP Intermediate_Dir "GameServer___Win32_HK"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib ggsrvlib26_MT.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_KJRTEST0"
# PROP BASE Intermediate_Dir "GameServer___Win32_KJRTEST0"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_t.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_GER"
# PROP BASE Intermediate_Dir "GameServer___Win32_GER"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_GER"
# PROP Intermediate_Dir "GameServer___Win32_GER"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_t.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_ger.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_DEREK0"
# PROP BASE Intermediate_Dir "GameServer___Win32_DEREK0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_DEREK0"
# PROP Intermediate_Dir "GameServer___Win32_DEREK0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"GameServer___Win32_DEREK/GameServer.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_t.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_derek.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_SPN"
# PROP BASE Intermediate_Dir "GameServer___Win32_SPN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_SPN"
# PROP Intermediate_Dir "GameServer___Win32_SPN"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_t.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_spn.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_PLD"
# PROP BASE Intermediate_Dir "GameServer___Win32_PLD"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_PLD"
# PROP Intermediate_Dir "GameServer___Win32_PLD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_SPN=14 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_PLD=16 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_spn.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_pld.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_FRC"
# PROP BASE Intermediate_Dir "GameServer___Win32_FRC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_FRC"
# PROP Intermediate_Dir "GameServer___Win32_FRC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_SPN=14 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "zlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_FRC=15 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_spn.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"GameServer_frc.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GameServer - Win32 Release"
# Name "GameServer - Win32 Debug"
# Name "GameServer - Win32 TestServer_Debug"
# Name "GameServer - Win32 WJKTEST"
# Name "GameServer - Win32 KYDTEST"
# Name "GameServer - Win32 WJKTEST_BRZ"
# Name "GameServer - Win32 holylance_JPN"
# Name "GameServer - Win32 HOLYLANCE_TEST"
# Name "GameServer - Win32 HOLYLANCE_RUS"
# Name "GameServer - Win32 PGMAN_MAL"
# Name "GameServer - Win32 HOLYLANCE_USA"
# Name "GameServer - Win32 FINWELL_TEST"
# Name "GameServer - Win32 swkwonTEST"
# Name "GameServer - Win32 MEXICO"
# Name "GameServer - Win32 WORLD_TOURNAMENT"
# Name "GameServer - Win32 USA_ESP"
# Name "GameServer - Win32 HK"
# Name "GameServer - Win32 KJRTEST"
# Name "GameServer - Win32 GER"
# Name "GameServer - Win32 DEREK"
# Name "GameServer - Win32 SPN"
# Name "GameServer - Win32 PLD"
# Name "GameServer - Win32 FRC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Affinity.cpp
# End Source File
# Begin Source File

SOURCE=.\AlternateMerchant.cpp
# End Source File
# Begin Source File

SOURCE=.\APet.cpp
# End Source File
# Begin Source File

SOURCE=.\APetProto.cpp
# End Source File
# Begin Source File

SOURCE=.\Area.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Assist.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_DivisionExpSP.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_GetDamage.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_IsAvailable.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_NPCDropItem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_ProcAfterHit.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_ProcDead_APet.cpp
# End Source File
# Begin Source File

SOURCE=.\Battle_ProcDead_Elemental.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_ProcDead_NPC.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_ProcDead_PC.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle_ProcDead_Pet.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Cell.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Character.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Cmd.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CmdMsg.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compression.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigFile.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DBCmd.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DBThread.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Descriptor.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncAction.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncAdmin.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncAffinity.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncAlternateMerchant.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncAttack.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncCashItem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncChangeJob.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncChat.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncEtc.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncEvent.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncExAPet.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncExCastleMap.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncExCastllan.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncExchange.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncExPartyMatch.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncExpedition.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncExPet.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncExSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncExSubJob.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncExtremeCube.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncFriend.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncGuild.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncItem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncMemPos.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncMonsterCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncMove.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncNPCRegen.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncParty.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncPersonalShop.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncPK.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncProduce.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncQuest.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncQuickSlot.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncRaid.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncRebirth.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncSkill.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncSSkill.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncStash.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncTeach.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFuncTradeAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncTrigerEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\doFuncWarp.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DratanCastle.cpp
# End Source File
# Begin Source File

SOURCE=.\Elemental.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Env.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExchangeItems.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Exp.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Expedition.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtremeCube.cpp
# End Source File
# Begin Source File

SOURCE=.\Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Friend.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameServer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Guild.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hardcoding.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HexaDump4XTrap.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBuffer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventory.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Item.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LacaRette.cpp
# End Source File
# Begin Source File

SOURCE=.\localize.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LuckyDrawBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MapAttr.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MemPos.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MobActivity.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MonsterCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterMercenary.cpp
# End Source File
# Begin Source File

SOURCE=.\NetMsg.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NPC.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NPCProto.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NPCRegenInfo.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\nProtectFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Option.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OutputBuffer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Party.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PC.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PersonalShop.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pet.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProcConnMsg.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProcHelperMsg.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProcMsgrMsg.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProcSubHelperMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Promotion2Event.cpp
# End Source File
# Begin Source File

SOURCE=.\Quest.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RaidData.cpp
# End Source File
# Begin Source File

SOURCE=.\Random.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RareOptionProto.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Requital.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ServerRun.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SetItemProto.cpp
# End Source File
# Begin Source File

SOURCE=.\Skill.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SocketSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\SSkill.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stash.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TitleSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\TrigerSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WarCastle.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WarCastle_Merac.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WarCastleFunc.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XTrap4Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Zone.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Affinity.h
# End Source File
# Begin Source File

SOURCE=.\APetProto.h
# End Source File
# Begin Source File

SOURCE=.\Area.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Assist.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Battle.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Cell.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Character.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Cmd.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CmdMsg.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compression.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Conf.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Config.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigFile.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CPos.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CryptMem.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DBCmd.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DBThread.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Descriptor.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doFunc.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DratanCastle.h
# End Source File
# Begin Source File

SOURCE=.\Env.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExchangeItems.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Exp.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Expedition.h
# End Source File
# Begin Source File

SOURCE=.\ExtremeCube.h
# End Source File
# Begin Source File

SOURCE=.\Factory.h
# End Source File
# Begin Source File

SOURCE=.\Friend.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ggsrv26.h
# End Source File
# Begin Source File

SOURCE=.\Guild.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hardcoding.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HexaDump4XTrap.h
# End Source File
# Begin Source File

SOURCE=.\InputBuffer.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InputQueue.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inventory.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Item.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LacaRette.h
# End Source File
# Begin Source File

SOURCE=.\LCCRC32.h
# End Source File
# Begin Source File

SOURCE=.\LCList.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LCMD5.h
# End Source File
# Begin Source File

SOURCE=.\LCString.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LuckyDrawBox.h
# End Source File
# Begin Source File

SOURCE=.\MapAttr.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MemPool.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MemPos.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MessageType.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MonsterCombo.h
# End Source File
# Begin Source File

SOURCE=.\MonsterMercenary.h
# End Source File
# Begin Source File

SOURCE=.\Mutex.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NetMsg.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NPCProto.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NPCRegenInfo.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\nProtectFunc.h
# End Source File
# Begin Source File

SOURCE=.\Option.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OutputBuffer.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Party.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PersonalShop.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Promotion2Event.h
# End Source File
# Begin Source File

SOURCE=.\Quest.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RaidData.h
# End Source File
# Begin Source File

SOURCE=.\RareOptionProto.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\requital.h
# End Source File
# Begin Source File

SOURCE=.\Server.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SetItemProto.h
# End Source File
# Begin Source File

SOURCE=.\Shop.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Skill.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SocketSystem.h
# End Source File
# Begin Source File

SOURCE=.\SSkill.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stash.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stdhdrs.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sysdep.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TitleSystem.h
# End Source File
# Begin Source File

SOURCE=.\TrigerSet.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WarCastle.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Xtrap_S_Interface.h
# End Source File
# Begin Source File

SOURCE=.\Zone.h

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# End Group
# Begin Group "Settings"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\data\akantemple_1024_1024_0.sat
# End Source File
# Begin Source File

SOURCE=.\data\akantemple_1024_1024_0.sht
# End Source File
# Begin Source File

SOURCE=.\data\Ascadia_MondShine_2560_2560_0.sat
# End Source File
# Begin Source File

SOURCE=.\data\Ascadia_MondShine_2560_2560_0.sht
# End Source File
# Begin Source File

SOURCE=.\data\Ascadia_MondShine_Cappella_1_1024_1024_0.sat
# End Source File
# Begin Source File

SOURCE=.\data\Ascadia_MondShine_Cappella_1_1024_1024_0.sht
# End Source File
# Begin Source File

SOURCE=.\data\Ascadia_MondShine_Cappella_2_1024_1024_0.sat
# End Source File
# Begin Source File

SOURCE=.\data\Ascadia_MondShine_Cappella_2_1024_1024_0.sht
# End Source File
# Begin Source File

SOURCE=.\data\Ascadia_MondShine_Dungeon1_1536_1536_0.sat
# End Source File
# Begin Source File

SOURCE=.\data\Ascadia_MondShine_Dungeon1_1536_1536_0.sht
# End Source File
# Begin Source File

SOURCE=.\data\Cube_1536_1536_0.sat
# End Source File
# Begin Source File

SOURCE=.\data\Cube_1536_1536_0.sht
# End Source File
# Begin Source File

SOURCE=.\data\Dratan_3072_3072_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Dratan_3072_3072_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Dratan_3072_3072_1.sat
# End Source File
# Begin Source File

SOURCE=.\data\Dratan_3072_3072_1.sht
# End Source File
# Begin Source File

SOURCE=.\data\Dratan_siegeWarfare_dungeon_1536_1536_0.sat
# End Source File
# Begin Source File

SOURCE=.\data\Dratan_siegeWarfare_dungeon_1536_1536_0.sht
# End Source File
# Begin Source File

SOURCE=.\data\Dungeon1_480_480_0.sat
# End Source File
# Begin Source File

SOURCE=.\data\Dungeon1_480_480_0.sht
# End Source File
# Begin Source File

SOURCE=.\data\Dungeon1_480_480_1.sat
# End Source File
# Begin Source File

SOURCE=.\data\Dungeon1_480_480_1.sht
# End Source File
# Begin Source File

SOURCE=.\data\Dungeon2_504_432_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Dungeon2_504_432_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Dungeon2_504_432_1.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Dungeon2_504_432_1.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Dungeon3_504_504_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Dungeon3_504_504_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_1536_1536_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_1536_1536_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_dungeon_10F_516_516_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_dungeon_10F_516_516_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_dungeon_1F_7F_600_600_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_dungeon_1F_7F_600_600_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_dungeon_8F_600_600_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_dungeon_8F_600_600_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_dungeon_9F_516_516_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_dungeon_9F_516_516_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_PK_768_768_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Egeha_PK_768_768_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\GuildRoom_100_100_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\GuildRoom_100_100_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\M_combo_2400_2400_0_1F.sat
# End Source File
# Begin Source File

SOURCE=.\data\M_combo_2400_2400_0_1F.sht
# End Source File
# Begin Source File

SOURCE=.\data\M_combo_2400_2400_0_2F.sat
# End Source File
# Begin Source File

SOURCE=.\data\M_combo_2400_2400_0_2F.sht
# End Source File
# Begin Source File

SOURCE=.\data\Merac_1536_1536_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\Merac_1536_1536_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\newStobm.bin

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\newStobm.bin.new

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\OX_Room_120_120_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\OX_Room_120_120_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\P_Dungeon1_1152_1152_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\P_Dungeon1_1152_1152_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\PK_tournament_1F.sat
# End Source File
# Begin Source File

SOURCE=.\data\PK_tournament_1F.sht
# End Source File
# Begin Source File

SOURCE=.\data\PK_tournament_2F.sat
# End Source File
# Begin Source File

SOURCE=.\data\PK_tournament_2F.sht
# End Source File
# Begin Source File

SOURCE=.\data\S_Dungeon3.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\S_Dungeon3.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\S_Dungeon4_516_516_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\S_Dungeon4_516_516_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\SingleDungeon2_720_720_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\SingleDungeon2_720_720_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\SingleDungeon_768_768_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\SingleDungeon_768_768_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\StartZone_1536_1536_0.sat

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\StartZone_1536_1536_0.sht

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\data\STREIANA_1536_1536_1F.sat
# End Source File
# Begin Source File

SOURCE=.\data\STREIANA_1536_1536_1F.sht
# End Source File
# Begin Source File

SOURCE=.\data\STREIANA_1536_1536_2F.sat
# End Source File
# Begin Source File

SOURCE=.\data\STREIANA_1536_1536_2F.sht
# End Source File
# Begin Source File

SOURCE=.\data\STREIANA_1536_1536_3F.sat
# End Source File
# Begin Source File

SOURCE=.\data\STREIANA_1536_1536_3F.sht
# End Source File
# End Group
# Begin Group "Makefile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Makefile

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# End Group
# Begin Group "Documents"

# PROP Default_Filter "xls"
# Begin Source File

SOURCE=".\DB Scheme.xls"

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LogManual.xls

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\MSG정의.xls"

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\메시지 순서.xls"

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# End Group
# Begin Group "Localize"

# PROP Default_Filter "inc;inh"
# Begin Source File

SOURCE=.\Config_Developer.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize.inh

!IF  "$(CFG)" == "GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 TestServer_Debug"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KYDTEST"

# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WJKTEST_BRZ"

!ELSEIF  "$(CFG)" == "GameServer - Win32 holylance_JPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PGMAN_MAL"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FINWELL_TEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 swkwonTEST"

# PROP BASE Intermediate_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 MEXICO"

!ELSEIF  "$(CFG)" == "GameServer - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_ESP"

!ELSEIF  "$(CFG)" == "GameServer - Win32 HK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "GameServer - Win32 GER"

!ELSEIF  "$(CFG)" == "GameServer - Win32 DEREK"

!ELSEIF  "$(CFG)" == "GameServer - Win32 SPN"

!ELSEIF  "$(CFG)" == "GameServer - Win32 PLD"

!ELSEIF  "$(CFG)" == "GameServer - Win32 FRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Config_Localize_BACKUP.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_BILA.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_EUR.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_event.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_GER.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_HBK.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_ITA.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_JPN.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_KOR.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_MAL.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_NLD.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_QA.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_RUS.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_TEST.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_TLD.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_TUR.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_TWN.inh
# End Source File
# Begin Source File

SOURCE=.\Config_Localize_USA.inh
# End Source File
# Begin Source File

SOURCE=".\항목별 패치 방법.txt"
# End Source File
# End Group
# End Target
# End Project
