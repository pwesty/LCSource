# Microsoft Developer Studio Project File - Name="SubHelper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SubHelper - Win32 PLD
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SubHelper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SubHelper.mak" CFG="SubHelper - Win32 PLD"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SubHelper - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 KYDTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 WORLD_TOURNAMENT" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 swkwonTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 HK" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 USA" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 Holylance_jpn" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 MEXICO" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 wjktest_brz" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 HOLYLANCE_RUS" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 HOLYLANCE_USA" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 KJRTEST" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 TestServer" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 MEXICO_TEST" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 TURKEY" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 ITALY" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 DEREK" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 GER" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 SPN" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 FRC" (based on "Win32 (x86) Console Application")
!MESSAGE "SubHelper - Win32 PLD" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SubHelper - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /D "DEREK_DEBUG_DONT_SOAPHELPER" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /machine:I386 /out:"SubHelper.exe"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 Debug"

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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 KYDTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Helper___Win32_KYDTEST"
# PROP BASE Intermediate_Dir "Helper___Win32_KYDTEST"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 WORLD_TOURNAMENT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_WORLD_TOURNAMENT0"
# PROP BASE Intermediate_Dir "SubHelper___Win32_WORLD_TOURNAMENT0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_WORLD_TOURNAMENT0"
# PROP Intermediate_Dir "SubHelper___Win32_WORLD_TOURNAMENT0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /D "TEST_SERVER" /D WORLD_TOURNAMENT=100 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 swkwonTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_swkwonTEST"
# PROP BASE Intermediate_Dir "SubHelper___Win32_swkwonTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_swkwonTEST"
# PROP Intermediate_Dir "SubHelper___Win32_swkwonTEST"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 HK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_HK"
# PROP BASE Intermediate_Dir "SubHelper___Win32_HK"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_HK"
# PROP Intermediate_Dir "SubHelper___Win32_HK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_HBK=11 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_HK.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 USA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_USA"
# PROP BASE Intermediate_Dir "SubHelper___Win32_USA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_USA"
# PROP Intermediate_Dir "SubHelper___Win32_USA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 Holylance_jpn"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_Holylance_jpn"
# PROP BASE Intermediate_Dir "SubHelper___Win32_Holylance_jpn"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_Holylance_jpn"
# PROP Intermediate_Dir "SubHelper___Win32_Holylance_jpn"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_JPN=6 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 MEXICO"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_MEXICO"
# PROP BASE Intermediate_Dir "SubHelper___Win32_MEXICO"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_MEXICO"
# PROP Intermediate_Dir "SubHelper___Win32_MEXICO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MEX=20 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 wjktest_brz"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_wjktest_brz"
# PROP BASE Intermediate_Dir "SubHelper___Win32_wjktest_brz"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_wjktest_brz"
# PROP Intermediate_Dir "SubHelper___Win32_wjktest_brz"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_BRZ=10 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 HOLYLANCE_RUS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_HOLYLANCE_RUS"
# PROP BASE Intermediate_Dir "SubHelper___Win32_HOLYLANCE_RUS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_HOLYLANCE_RUS"
# PROP Intermediate_Dir "SubHelper___Win32_HOLYLANCE_RUS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_HBK=11 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_RUS=17 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_HK.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_rus.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 HOLYLANCE_USA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_HOLYLANCE_USA"
# PROP BASE Intermediate_Dir "SubHelper___Win32_HOLYLANCE_USA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_HOLYLANCE_USA"
# PROP Intermediate_Dir "SubHelper___Win32_HOLYLANCE_USA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_USA=9 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 KJRTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_KJRTEST"
# PROP BASE Intermediate_Dir "SubHelper___Win32_KJRTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_KJRTEST"
# PROP Intermediate_Dir "SubHelper___Win32_KJRTEST"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 TestServer"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_TestServer"
# PROP BASE Intermediate_Dir "SubHelper___Win32_TestServer"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_TestServer"
# PROP Intermediate_Dir "SubHelper___Win32_TestServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MEX=20 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 MEXICO_TEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_MEXICO_TEST"
# PROP BASE Intermediate_Dir "SubHelper___Win32_MEXICO_TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_MEXICO_TEST"
# PROP Intermediate_Dir "SubHelper___Win32_MEXICO_TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "KJR_TEST" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_MEX=20 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 TURKEY"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_TURKEY"
# PROP BASE Intermediate_Dir "SubHelper___Win32_TURKEY"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_TURKEY"
# PROP Intermediate_Dir "SubHelper___Win32_TURKEY"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 ITALY"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_ITALY"
# PROP BASE Intermediate_Dir "SubHelper___Win32_ITALY"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_ITALY"
# PROP Intermediate_Dir "SubHelper___Win32_ITALY"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 DEREK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_DEREK"
# PROP BASE Intermediate_Dir "SubHelper___Win32_DEREK"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_DEREK"
# PROP Intermediate_Dir "SubHelper___Win32_DEREK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_derek.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 GER"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_GER"
# PROP BASE Intermediate_Dir "SubHelper___Win32_GER"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_GER"
# PROP Intermediate_Dir "SubHelper___Win32_GER"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_KOR=0 /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib   LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_ger.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 SPN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_SPN"
# PROP BASE Intermediate_Dir "SubHelper___Win32_SPN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_SPN"
# PROP Intermediate_Dir "SubHelper___Win32_SPN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_SPN=14 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib   LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_ger.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib   LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_spn.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 FRC"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_FRC"
# PROP BASE Intermediate_Dir "SubHelper___Win32_FRC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_FRC"
# PROP Intermediate_Dir "SubHelper___Win32_FRC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_FRC=15 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib   LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_ger.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib   LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_frc.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SubHelper - Win32 PLD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SubHelper___Win32_PLD"
# PROP BASE Intermediate_Dir "SubHelper___Win32_PLD"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SubHelper___Win32_PLD"
# PROP Intermediate_Dir "SubHelper___Win32_PLD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_GER=13 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D LC_PLD=16 /D "DEREK_DEBUG_AND_TEST" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib   LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_ger.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mysqlclient.lib zlib.lib LCCrypt.lib   LCSha256.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"SubHelper_pld.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SubHelper - Win32 Release"
# Name "SubHelper - Win32 Debug"
# Name "SubHelper - Win32 KYDTEST"
# Name "SubHelper - Win32 WORLD_TOURNAMENT"
# Name "SubHelper - Win32 swkwonTEST"
# Name "SubHelper - Win32 HK"
# Name "SubHelper - Win32 USA"
# Name "SubHelper - Win32 Holylance_jpn"
# Name "SubHelper - Win32 MEXICO"
# Name "SubHelper - Win32 wjktest_brz"
# Name "SubHelper - Win32 HOLYLANCE_RUS"
# Name "SubHelper - Win32 HOLYLANCE_USA"
# Name "SubHelper - Win32 KJRTEST"
# Name "SubHelper - Win32 TestServer"
# Name "SubHelper - Win32 MEXICO_TEST"
# Name "SubHelper - Win32 TURKEY"
# Name "SubHelper - Win32 ITALY"
# Name "SubHelper - Win32 DEREK"
# Name "SubHelper - Win32 GER"
# Name "SubHelper - Win32 SPN"
# Name "SubHelper - Win32 FRC"
# Name "SubHelper - Win32 PLD"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CmdInterpreter.cpp
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

SOURCE=.\doFuncCommand.cpp

!IF  "$(CFG)" == "SubHelper - Win32 Release"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 Debug"

# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 KYDTEST"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 WORLD_TOURNAMENT"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 swkwonTEST"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 HK"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 Holylance_jpn"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 MEXICO"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 wjktest_brz"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 HOLYLANCE_RUS"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 HOLYLANCE_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 KJRTEST"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 TestServer"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 MEXICO_TEST"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 TURKEY"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 ITALY"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 DEREK"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 GER"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 SPN"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 FRC"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ELSEIF  "$(CFG)" == "SubHelper - Win32 PLD"

# ADD BASE CPP /D "LC_USA"
# ADD CPP /D "LC_USA"

!ENDIF 

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

SOURCE=.\RankingSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerRun.cpp
# End Source File
# Begin Source File

SOURCE=.\SubHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TradeAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp
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

SOURCE=.\doFunc.h
# End Source File
# Begin Source File

SOURCE=.\Exp.h
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

SOURCE=.\RankingSystem.h
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

SOURCE=.\TradeAgent.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h
# End Source File
# End Group
# Begin Group "Settings"

# PROP Default_Filter "bin"
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
