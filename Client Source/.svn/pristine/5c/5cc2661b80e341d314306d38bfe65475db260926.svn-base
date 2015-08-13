# Microsoft Developer Studio Project File - Name="Nksp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Nksp - Win32 KORTEST
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Nksp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Nksp.mak" CFG="Nksp - Win32 KORTEST"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Nksp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 USALIVE" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 GERLIVE" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 RUSLIVE" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 BRZLIVE" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 MAXLIVE" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 KORTEST" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 THAILIVE" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 WORLDEDITOR" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 CHNLIVE" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Nksp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "_Debug"
# PROP Intermediate_Dir "_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /I "..\include" /D "_USRDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "G_KOR" /D "VER_TEST" /Fr /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib" /nodefaultlib:"gamempd.lib" /nodefaultlib:"libcd.lib" /pdbtype:sept /libpath:"tinyxml" /libpath:"..\lib" -verbose:lib
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\_Debug\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _Debug\$(InputName).exe $(ENGINE_DIR)\Bin\Debug >nul 
	copy _Debug\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy _Debug\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build
# Begin Special Build Tool
OutDir=.\_Debug
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir                      $(OutDir)\..\..\DebugFilesForCopy                     	copy                      $(OutDir)\Nksp.exe                      $(OutDir)\..\..\DebugFilesForCopy\                     	copy                      $(OutDir)\Nksp.map                      $(OutDir)\..\..\DebugFilesForCopy\                     	copy                      $(OutDir)\Nksp.pdb                      $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Nksp - Win32 USALIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_USALIVE"
# PROP BASE Intermediate_Dir "Nksp___Win32_USALIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_USALIVE"
# PROP Intermediate_Dir "_USALIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# ADD LINK32 winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(USA_LIVE)\Bin
InputPath=.\_USALIVE\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(USA_LIVE)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _USALIVE\$(InputName).exe $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).map $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).pdb $(USA_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 GERLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_GERLIVE"
# PROP BASE Intermediate_Dir "Nksp___Win32_GERLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVE"
# PROP Intermediate_Dir "_GERLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# ADD LINK32 winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(GER_LIVE)\Bin
InputPath=.\_GERLIVE\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(GER_LIVE)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVE\$(InputName).exe $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).map $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).pdb $(GER_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 RUSLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_RUSLIVE"
# PROP BASE Intermediate_Dir "Nksp___Win32_RUSLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_RUSLIVE"
# PROP Intermediate_Dir "_RUSLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_RUSSIA" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# ADD LINK32 winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(RUS_LIVE)\Bin
InputPath=.\_RUSLIVE\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(RUS_LIVE)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _RUSLIVE\$(InputName).exe $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).map $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).pdb $(RUS_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 BRZLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_BRZLIVE"
# PROP BASE Intermediate_Dir "Nksp___Win32_BRZLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_BRZLIVE"
# PROP Intermediate_Dir "_BRZLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_BRAZIL" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(BRZ_LIVE)\Bin
InputPath=.\_BRZLIVE\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(BRZ_LIVE)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _BRZLIVE\$(InputName).exe $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).map $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).pdb $(BRZ_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 MAXLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_MAXLIVE"
# PROP BASE Intermediate_Dir "Nksp___Win32_MAXLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_MAXLIVE"
# PROP Intermediate_Dir "_MAXLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_MAXICO" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(MAX_LIVE)\Bin
InputPath=.\_MAXLIVE\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(MAX_LIVE)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _MAXLIVE\$(InputName).exe $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).map $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).pdb $(MAX_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 KORTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_KORTEST"
# PROP BASE Intermediate_Dir "Nksp___Win32_KORTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_KORTEST"
# PROP Intermediate_Dir "_KORTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\include" /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib version.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(KOR_TEST)\Bin
InputPath=.\_KORTEST\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(KOR_TEST)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _KORTEST\$(InputName).exe $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).map $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).pdb $(KOR_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 THAILIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_THAILIVE"
# PROP BASE Intermediate_Dir "Nksp___Win32_THAILIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_THAILIVE"
# PROP Intermediate_Dir "_THAILIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_THAI" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(THAI_LIVE)\Bin
InputPath=.\_THAILIVE\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(THAI_LIVE)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _THAILIVE\$(InputName).exe $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).map $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).pdb $(THAI_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_WORLDEDITOR"
# PROP BASE Intermediate_Dir "Nksp___Win32_WORLDEDITOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WORLDEDITOR"
# PROP Intermediate_Dir "WORLDEDITOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\WORLDEDITOR\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy WORLDEDITOR\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 CHNLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_CHNLIVE"
# PROP BASE Intermediate_Dir "Nksp___Win32_CHNLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_CHNLIVE"
# PROP Intermediate_Dir "_CHNLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /D "G_CHINA" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /fo"CHNLIVE/Nksp.res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"CHNLIVE/Nksp.bsc"
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# ADD LINK32 winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(CHN_LIVE)\Bin
InputPath=.\_CHNLIVE\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(CHN_LIVE)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _CHNLIVE\$(InputName).exe $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).map $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).pdb $(CHN_LIVE)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Nksp - Win32 Debug"
# Name "Nksp - Win32 USALIVE"
# Name "Nksp - Win32 GERLIVE"
# Name "Nksp - Win32 RUSLIVE"
# Name "Nksp - Win32 BRZLIVE"
# Name "Nksp - Win32 MAXLIVE"
# Name "Nksp - Win32 KORTEST"
# Name "Nksp - Win32 THAILIVE"
# Name "Nksp - Win32 WORLDEDITOR"
# Name "Nksp - Win32 CHNLIVE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CmdLine.cpp
# End Source File
# Begin Source File

SOURCE=.\GameGuard.cpp
# End Source File
# Begin Source File

SOURCE=.\GLSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\MainWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Nksp.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\StdH.cpp
# ADD CPP /Yc"StdH.h"
# End Source File
# Begin Source File

SOURCE=.\VarList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CDCheck.h
# End Source File
# Begin Source File

SOURCE=.\CmdLine.h
# End Source File
# Begin Source File

SOURCE=.\GameGuard.h
# End Source File
# Begin Source File

SOURCE=.\GLSettings.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Templates\HashTable_CEffectGroup.h
# End Source File
# Begin Source File

SOURCE=.\MainWindow.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Network\MessageDispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Nksp.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SplashScreen.h
# End Source File
# Begin Source File

SOURCE=.\StdH.h
# End Source File
# Begin Source File

SOURCE=.\VarList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cur_00.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_01.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_02.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_03.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_04.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_05.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_06.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_07.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_08.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_09.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_10.cur
# End Source File
# Begin Source File

SOURCE=.\res\lastwar.ico
# End Source File
# Begin Source File

SOURCE=.\res\Nksp.ico
# End Source File
# Begin Source File

SOURCE=.\Nksp.rc
# End Source File
# Begin Source File

SOURCE=.\nocursor.cur
# End Source File
# Begin Source File

SOURCE=.\res\shadow.ico
# End Source File
# Begin Source File

SOURCE=.\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\SplashMask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\test_exe_blue.ico
# End Source File
# End Group
# Begin Group "Scripts"

# PROP Default_Filter ""
# End Group
# Begin Group "Texts"

# PROP Default_Filter ""
# End Group
# Begin Group "XTrap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xTrapWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\xTrapWrapper.h
# End Source File
# End Group
# End Target
# End Project
