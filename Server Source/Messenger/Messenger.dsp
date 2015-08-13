# Microsoft Developer Studio Project File - Name="Messenger" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Messenger - Win32 ITALY
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Messenger.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Messenger.mak" CFG="Messenger - Win32 ITALY"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Messenger - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 TESTSERVER" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 KYDTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 WJKTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 WJKTEST_BRZ" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 holylance_JPN" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 HOLYLANCE_TEST" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 HOLYLANCE_RUS" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 PGMAN_MAL" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 HOLYLANCE_USA" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 swkwonTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 MEXICO" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 WORLD_TOURNAMENT" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 ESP_USA" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 JPNEP" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 HK" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 KJRTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 TURKEY" (based on "Win32 (x86) Console Application")
!MESSAGE "Messenger - Win32 ITALY" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Messenger - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Messenger - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 TESTSERVER"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_TESTSERVER"
# PROP BASE Intermediate_Dir "Messenger___Win32_TESTSERVER"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "TESTSERVER"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_td.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 KYDTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_KYDTEST"
# PROP BASE Intermediate_Dir "Messenger___Win32_KYDTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "KYDTEST"
# PROP Intermediate_Dir "KYDTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 WJKTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_WJKTEST"
# PROP BASE Intermediate_Dir "Messenger___Win32_WJKTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "WJKTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "WJKTEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 WJKTEST_BRZ"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_WJKTEST_BRZ"
# PROP BASE Intermediate_Dir "Messenger___Win32_WJKTEST_BRZ"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "WJKTEST_BRZ"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "WJKTEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_BRZ=10 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_brz.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 holylance_JPN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_holylance_JPN"
# PROP BASE Intermediate_Dir "Messenger___Win32_holylance_JPN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_holylance_JPN"
# PROP Intermediate_Dir "Messenger___Win32_holylance_JPN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_kj.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_jpn.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 HOLYLANCE_TEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_HOLYLANCE_TEST"
# PROP BASE Intermediate_Dir "Messenger___Win32_HOLYLANCE_TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "HOLYLANCE_TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "WJKTEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "HOLYLANCE_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 HOLYLANCE_RUS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_HOLYLANCE_RUS"
# PROP BASE Intermediate_Dir "Messenger___Win32_HOLYLANCE_RUS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_HOLYLANCE_RUS"
# PROP Intermediate_Dir "Messenger___Win32_HOLYLANCE_RUS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_PLD=16 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_RUS=17 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_imso.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_rus.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 PGMAN_MAL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_PGMAN_MAL"
# PROP BASE Intermediate_Dir "Messenger___Win32_PGMAN_MAL"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_PGMAN_MAL"
# PROP Intermediate_Dir "Messenger___Win32_PGMAN_MAL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MAL=7 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 HOLYLANCE_USA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_HOLYLANCE_USA"
# PROP BASE Intermediate_Dir "Messenger___Win32_HOLYLANCE_USA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_HOLYLANCE_USA"
# PROP Intermediate_Dir "Messenger___Win32_HOLYLANCE_USA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "HOLYLANCE_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_usa.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 swkwonTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_swkwonTEST"
# PROP BASE Intermediate_Dir "Messenger___Win32_swkwonTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_swkwonTEST"
# PROP Intermediate_Dir "Messenger___Win32_swkwonTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /D "SWKWON_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 MEXICO"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_MEXICO"
# PROP BASE Intermediate_Dir "Messenger___Win32_MEXICO"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_MEXICO"
# PROP Intermediate_Dir "Messenger___Win32_MEXICO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_RUS=17 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MEX=20 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_rus.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_mex.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 WORLD_TOURNAMENT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_WORLD_TOURNAMENT"
# PROP BASE Intermediate_Dir "Messenger___Win32_WORLD_TOURNAMENT"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_WORLD_TOURNAMENT"
# PROP Intermediate_Dir "Messenger___Win32_WORLD_TOURNAMENT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /D WORLD_TOURNAMENT=100 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_usa.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_usa.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 ESP_USA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_ESP_USA"
# PROP BASE Intermediate_Dir "Messenger___Win32_ESP_USA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_ESP_USA"
# PROP Intermediate_Dir "Messenger___Win32_ESP_USA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MEX=20 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MEX=20 /D "DEREK_TEST_ACTION" /D "DEREK_INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_mex.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_espusa.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 JPNEP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_JPNEP"
# PROP BASE Intermediate_Dir "Messenger___Win32_JPNEP"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_JPNEP"
# PROP Intermediate_Dir "Messenger___Win32_JPNEP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /D "DEREK_DEBUG_DONT_SOAPHELPER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN_EP=6 /D "DEREK_DEBUG_DONT_SOAPHELPER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_jpn.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_jpnep.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 HK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_HK"
# PROP BASE Intermediate_Dir "Messenger___Win32_HK"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_HK"
# PROP Intermediate_Dir "Messenger___Win32_HK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_HBK=11 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_usa.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_usa.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 KJRTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_KJRTEST"
# PROP BASE Intermediate_Dir "Messenger___Win32_KJRTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "KJRTEST"
# PROP Intermediate_Dir "KJRTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "KJR_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger_td.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 TURKEY"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_TURKEY"
# PROP BASE Intermediate_Dir "Messenger___Win32_TURKEY"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_TURKEY"
# PROP Intermediate_Dir "Messenger___Win32_TURKEY"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "KJR_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_TUR=18 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Messenger - Win32 ITALY"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Messenger___Win32_ITALY"
# PROP BASE Intermediate_Dir "Messenger___Win32_ITALY"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Messenger___Win32_ITALY"
# PROP Intermediate_Dir "Messenger___Win32_ITALY"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "KJR_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_ITA=19 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib ws2_32.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Messenger.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Messenger - Win32 Release"
# Name "Messenger - Win32 Debug"
# Name "Messenger - Win32 TESTSERVER"
# Name "Messenger - Win32 KYDTEST"
# Name "Messenger - Win32 WJKTEST"
# Name "Messenger - Win32 WJKTEST_BRZ"
# Name "Messenger - Win32 holylance_JPN"
# Name "Messenger - Win32 HOLYLANCE_TEST"
# Name "Messenger - Win32 HOLYLANCE_RUS"
# Name "Messenger - Win32 PGMAN_MAL"
# Name "Messenger - Win32 HOLYLANCE_USA"
# Name "Messenger - Win32 swkwonTEST"
# Name "Messenger - Win32 MEXICO"
# Name "Messenger - Win32 WORLD_TOURNAMENT"
# Name "Messenger - Win32 ESP_USA"
# Name "Messenger - Win32 JPNEP"
# Name "Messenger - Win32 HK"
# Name "Messenger - Win32 KJRTEST"
# Name "Messenger - Win32 TURKEY"
# Name "Messenger - Win32 ITALY"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

SOURCE=.\Messenger.cpp
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

SOURCE=.\MsgList.h
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
# End Target
# End Project
