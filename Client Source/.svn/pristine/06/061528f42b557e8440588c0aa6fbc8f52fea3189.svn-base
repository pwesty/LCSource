# Microsoft Developer Studio Project File - Name="Engine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Engine - Win32 DebugKalydo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Engine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Engine.mak" CFG="Engine - Win32 DebugKalydo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Engine - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 USALIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 GERLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 RUSLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 BRZLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 MAXLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 KORTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 THAILIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 WORLDEDITOR" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 DebugKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 ReleaseKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 GERLIVEKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 CHNLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 MAKEDEF" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Engine - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /I ".\\" /I "..\3rdparty\zlib-1.2.8" /D "ENGINE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "G_KOR" /D "VER_TEST" /Fr /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 tinyxmld.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /map /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"LIBCD.lib" /def:".\EngineD.def" /out:"_Debug/EngineD.dll" /pdbtype:sept /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\_Debug\EngineD.dll
InputName=EngineD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _Debug\$(InputName).dll $(ENGINE_DIR)\Bin\Debug >nul 
	copy _Debug\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy _Debug\$(InputName).lib $(ENGINE_DIR)\Bin\Debug >nul 
	copy _Debug\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build
# Begin Special Build Tool
OutDir=.\_Debug
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir         $(OutDir)\..\..\DebugFilesForCopy         copy         $(OutDir)\EngineD.dll         $(OutDir)\..\..\DebugFilesForCopy\         copy         $(OutDir)\EngineD.lib         $(OutDir)\..\..\DebugFilesForCopy\         copy         $(OutDir)\EngineD.map         $(OutDir)\..\..\DebugFilesForCopy\         copy         $(OutDir)\EngineD.pdb         $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_USALIVE"
# PROP BASE Intermediate_Dir "Engine___Win32_USALIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_USALIVE"
# PROP Intermediate_Dir "_USALIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(USA_LIVE)\Bin
InputPath=.\_USALIVE\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(USA_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _USALIVE\$(InputName).dll $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).map $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).lib $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).pdb $(USA_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_GERLIVE"
# PROP BASE Intermediate_Dir "Engine___Win32_GERLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVE"
# PROP Intermediate_Dir "_GERLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_USA" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib"
# ADD LINK32 wininet.lib tinyxml.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /def:".\Engine_XTrap.def" /libpath:"XTrap/lib tinyxml" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(GER_LIVE)\Bin
InputPath=.\_GERLIVE\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(GER_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVE\$(InputName).dll $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).map $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).lib $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).pdb $(GER_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_RUSLIVE"
# PROP BASE Intermediate_Dir "Engine___Win32_RUSLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_RUSLIVE"
# PROP Intermediate_Dir "_RUSLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_GERMAN" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_RUSSIA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(RUS_LIVE)\Bin
InputPath=.\_RUSLIVE\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(RUS_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _RUSLIVE\$(InputName).dll $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).map $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).lib $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).pdb $(RUS_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_BRZLIVE"
# PROP BASE Intermediate_Dir "Engine___Win32_BRZLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_BRZLIVE"
# PROP Intermediate_Dir "_BRZLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_GERMAN" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_BRAZIL" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(BRZ_LIVE)\Bin
InputPath=.\_BRZLIVE\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(BRZ_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _BRZLIVE\$(InputName).dll $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).map $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).lib $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).pdb $(BRZ_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_MAXLIVE"
# PROP BASE Intermediate_Dir "Engine___Win32_MAXLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_MAXLIVE"
# PROP Intermediate_Dir "_MAXLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_GERMAN" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_MAXICO" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(MAX_LIVE)\Bin
InputPath=.\_MAXLIVE\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(MAX_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _MAXLIVE\$(InputName).dll $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).map $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).lib $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).pdb $(MAX_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_KORTEST"
# PROP BASE Intermediate_Dir "Engine___Win32_KORTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_KORTEST"
# PROP Intermediate_Dir "_KORTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I ".\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(KOR_TEST)\Bin
InputPath=.\_KORTEST\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(KOR_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _KORTEST\$(InputName).dll $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).map $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).lib $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).pdb $(KOR_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_THAILIVE"
# PROP BASE Intermediate_Dir "Engine___Win32_THAILIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_THAILIVE"
# PROP Intermediate_Dir "_THAILIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_KOR" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_THAI" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(THAI_LIVE)\Bin
InputPath=.\_THAILIVE\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(THAI_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _THAILIVE\$(InputName).dll $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).map $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).lib $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).pdb $(THAI_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_WORLDEDITOR"
# PROP BASE Intermediate_Dir "Engine___Win32_WORLDEDITOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WORLDEDITOR"
# PROP Intermediate_Dir "WORLDEDITOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /I "..\\" /I ".\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_KOR" /D "VER_TEST" /D "WORLD_EDITOR" /D "_CLIENT_" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /O<none> /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib F_SocketR.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\WORLDEDITOR\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\WorldEditor\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).dll $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).lib $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).pdb $(ENGINE_DIR)\Bin\WorldEditor >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Engine___Win32_DebugKalydo"
# PROP BASE Intermediate_Dir "Engine___Win32_DebugKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugKalydo"
# PROP Intermediate_Dir "DebugKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "ENGINE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_KOR" /D "VER_TEST" /Fr /Yu"StdH.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /D "ENGINE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "G_KOR" /D "KALYDO" /Fr /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tinyxmld.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"Debug/EngineD.dll" /pdbtype:sept /libpath:"tinyxml"
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 tinyxmld.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib KRFReadLibDebug.lib F_SocketD.lib /nologo /dll /incremental:no /map /debug /machine:I386 /nodefaultlib:"libc.lib" /def:".\Engine_Kalydo.def" /out:"DebugKalydo/EngineD.dll" /pdbtype:sept /libpath:"tinyxml" /libpath:"..\Nksp\Compact" /libpath:"..\lib" /ignore:4197
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\DebugKalydo\EngineD.dll
InputName=EngineD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy DebugKalydo\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).lib $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_ReleaseKalydo"
# PROP BASE Intermediate_Dir "Engine___Win32_ReleaseKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseKalydo"
# PROP Intermediate_Dir "ReleaseKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_GERMAN" /D "KALYDO" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib KRFReadLib.lib F_SocketR.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /def:".\Engine_Kalydo.def" /libpath:"tinyxml" /libpath:"..\Nksp\Compact" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\ReleaseKalydo\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ReleaseKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_GERLIVEKalydo"
# PROP BASE Intermediate_Dir "Engine___Win32_GERLIVEKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVEKalydo"
# PROP Intermediate_Dir "_GERLIVEKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "G_GERMAN" /D "KALYDO" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /D "KALYDO" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib KRFReadLib.lib F_SocketR.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\Nksp\Compact" /libpath:"..\lib"
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib KRFReadLib.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /def:".\Engine_Kalydo.def" /libpath:"tinyxml" /libpath:"..\Kalydo\KRFReadLib\Bin" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\_GERLIVEKalydo\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVEKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_CHNLIVE"
# PROP BASE Intermediate_Dir "Engine___Win32_CHNLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_CHNLIVE"
# PROP Intermediate_Dir "_CHNLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /D "G_CHINA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# ADD LINK32 tinyxml.lib wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(CHN_LIVE)\Bin
InputPath=.\_CHNLIVE\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(CHN_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _CHNLIVE\$(InputName).dll $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).map $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).lib $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).pdb $(CHN_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_MAKEDEF"
# PROP BASE Intermediate_Dir "Engine___Win32_MAKEDEF"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_MAKEDEF"
# PROP Intermediate_Dir "_MAKEDEF"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "ENGINE_EXPORTS" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "_CLIENT_" /D "ENGINE_EXPORTS" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wininet.lib tinyxml.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /def:".\Engine_XTrap.def" /libpath:"XTrap/lib tinyxml" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# ADD LINK32 wininet.lib tinyxml.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"libc.lib" /def:".\Engine_XTrap.def" /libpath:"XTrap/lib tinyxml" /libpath:"tinyxml" /libpath:"..\lib" /ignore:4197
# Begin Custom Build - Copying $(InputName) binaries to $(MAKE_DEF)\Bin
InputPath=.\_MAKEDEF\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(MAKE_DEF)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _MAKEDEF\$(InputName).dll $(MAKE_DEF)\Bin >nul 
	copy _MAKEDEF\$(InputName).map $(MAKE_DEF)\Bin >nul 
	copy _MAKEDEF\$(InputName).lib $(MAKE_DEF)\Bin >nul 
	copy _MAKEDEF\$(InputName).pdb $(MAKE_DEF)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Engine - Win32 Debug"
# Name "Engine - Win32 USALIVE"
# Name "Engine - Win32 GERLIVE"
# Name "Engine - Win32 RUSLIVE"
# Name "Engine - Win32 BRZLIVE"
# Name "Engine - Win32 MAXLIVE"
# Name "Engine - Win32 KORTEST"
# Name "Engine - Win32 THAILIVE"
# Name "Engine - Win32 WORLDEDITOR"
# Name "Engine - Win32 DebugKalydo"
# Name "Engine - Win32 ReleaseKalydo"
# Name "Engine - Win32 GERLIVEKalydo"
# Name "Engine - Win32 CHNLIVE"
# Name "Engine - Win32 MAKEDEF"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Math\Float.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Functions.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Geometry.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Geometry_DOUBLE.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Object3D.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Object3D_CSG.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Object3D_IO.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\ObjectSector.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Placement.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Isometric.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Parallel.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Perspective.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Simple.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Simple_DOUBLE.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\TextureMapping.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Graphics\Adapter.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Benchmark.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Color.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DepthCheck.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DisplayMode.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DrawPort.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DrawPort_Particles.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DrawPort_RenderScene.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DXTC.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics\Fog.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Font.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_Direct3D.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_Direct3D_Colors.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_Direct3D_Textures.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_Direct3D_XBox.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_OpenGL.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_OpenGL_Textures.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_wrapper.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_wrapper_Direct3D.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_wrapper_OpenGL.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\GfxLibrary.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Graphics.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\ImageInfo.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\MultiMonitor.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Raster.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Shader.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\ShadowMap.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Stereo.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics\Texture.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\TextureEffects.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\TextureRender.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics\ViewPort.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sound\SoundData.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SoundDecoder.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SoundLibrary.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SoundMixer.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SoundObject.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# End Group
# Begin Group "Models"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Models\EditModel.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\MipMaker.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\Model.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\Normals.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel_Mask.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel_View.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\VertexGetting.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\World\World.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldCollision.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldCollisionGrid.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldCSG.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldEditingProfile.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldEntityHashing.cpp
# End Source File
# Begin Source File

SOURCE=.\World\WorldIO.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldRayCasting.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# End Group
# Begin Group "Templates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Templates\AllocationArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\BSP.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicContainer.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicStackArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\HashTableTemplate.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\LinearAllocator.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable_CTFileName.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable_CTranslationPair.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\ReusableContainer.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Selection.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\StaticArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\StaticStackArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CAnimData.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CAnimSet.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CEntityClass.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CFontData.cpp
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CMesh.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CModelData.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CModelInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CShader.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CSkeleton.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CSoundData.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CTextureData.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\ActionBuffer.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\CharacterTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ChatMsgBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ClientInterface.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\cmd.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\CNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\CommunicationInterface.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\Compression.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\CPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Diff.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\EMsgBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EntityHashing.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\InputBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ItemTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MessageDispatcher.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\MobTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MultiTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkMessage.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\NetworkProfile.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\PetTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\PlayerSource.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\PlayerTarget.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\Server.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\SessionState.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\SessionStateComm.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SessionStateDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SessionStateEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SessionStateExten.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SessionStateExtendNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SessionStateInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SessionStateItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SlaveTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\TcpIpConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ThreadWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\TxtQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Web.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\WebAddress.cpp
# End Source File
# End Group
# Begin Group "Rendering"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Rendering\RenCache.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RendASER.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RendClip.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\Render.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RenderAdding.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RenderBloom.cpp
# End Source File
# Begin Source File

SOURCE=.\Rendering\RenderBrushes.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RenderModels.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RendMisc.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\SelectOnRender.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# End Group
# Begin Group "Entities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Entities\Action.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\AffinityData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\ArmorPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\CashShopData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\Entity.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\EntityClass.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\EntityCollision.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\EntityCopying.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\EntityProperties.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\FieldBSPTesting.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\ItemData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\ItemEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\Items.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\Lacarette.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\LastPositions.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\LevelupGuide.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\MissionCase.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\MobData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\NearestPolygon.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\NoticeData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\NpcHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\OptionData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\PlayerCharacter.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\QuestSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\ShopData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\SkillTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\SmcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\SmcParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\SpecialSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\StatusEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\TargetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\TEventString.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\TradeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\WildPetData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\ZoneData.cpp
# End Source File
# End Group
# Begin Group "Light"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Light\LayerMaker.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Light\LayerMixer.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Light\LightSource.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# End Group
# Begin Group "Ska"

# PROP Default_Filter ""
# Begin Group "Terrain"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Terrain\Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainInt.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainTile.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TREditing.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRGenerate.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRLayerImp.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRRayCast.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRRender.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_1TRL.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_2TRL.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_Fog.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_Shadow.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShadows.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Ska\AnimSet.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\Mesh.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\ModelInstance.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\NmFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\Ska\RMMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\Ska\RMRender.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\RMRenderMask.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\Skeleton.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\StringTable.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# End Group
# Begin Group "XBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XBox\IFeel_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XBox\Input_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XBox\SoundDecoder_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XBox\SymbolLocator_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XBox\VideoDecoder_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Effect"

# PROP Default_Filter ""
# Begin Group "Effects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CCameraEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectGroupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEntityEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CLightEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CMdlEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\COrbitEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CShockWaveEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSkaEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSoundEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSplineBillboardEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSplinePathEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CTerrainEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CTraceEffect.cpp
# End Source File
# End Group
# Begin Group "Particle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CParticle.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleGroupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticles.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorption.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorptionDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorptionSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesCommonProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitter.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterCone.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessDynamicState.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessForce.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessPointGoal.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessVelocity.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CRandomTable.cpp
# End Source File
# End Group
# Begin Group "Tags"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CEntityTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CGroupTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSkaTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CTagManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CWorldTag.cpp
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\TimerItem.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Effect\CManager.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Effect\CNiceWater.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\EffectCommon.cpp
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Contents\Base\ChattingUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAD.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAffinity.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAffinityInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAutoHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBingoBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICashShopEX.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChatting_GMCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChildInvenSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChildQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICollectBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICompound.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIDrawFigure.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIEventPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFiltering.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFlowerTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFortune.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGamble.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildMark.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildStash.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildWarPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGWMix.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelpIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInitJob.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInvenCashBag.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInvenCashBagBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIItemProduct.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UILacarette.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessageBoxL.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessenger.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMinigame.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMix.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMixNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMonsterCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMonsterMercenary.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UINickName.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UINpcHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UINpcScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIOption.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIOXQuizEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPartyAuto.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPersonalShop.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetFree.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetItemMix.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetTraining.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPlayerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProcessNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProduct.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRadar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRanking.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRectString.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRefine.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIReformSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIResurrection.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISecurity.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectList.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectResource.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectWord.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIShop.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfare.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfareDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfareNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISignBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISimplePop.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISingleBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISkillLearn.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISkillToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISocketSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISummon.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISystemMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITalk.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITatoo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITeleport.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITrackPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWareHouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWebBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWildPetInfo.cpp
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Base\Anim.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Changeable.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Console.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\CRC.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\CRCTable.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\CTString.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\CTWString.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\DamageLogInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\Directory.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\ErrorReporting.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\FileName.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\IFeel.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Input.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Lists.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\Memory.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\MemoryTracking.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\Profiling.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\ProgressHook.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Protection.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Registry.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Relations.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\ReplaceFile.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Serial.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Shell.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\ShellTypes.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\SimpleHashTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\StackDump.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Statistics.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Stream.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Synchronization.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Timer.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\TLVar.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\Translation.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Unzip.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Updateable.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# End Group
# Begin Group "Brushes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Brushes\Brush.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushArchive.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushExport.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushImport.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushIO.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushMip.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushPolygon.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushSector.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushShadows.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushTriangularize.cpp
# End Source File
# End Group
# Begin Group "XTrapInterface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XTrapInterface\XTrapInterface.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Engine.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine.def

!IF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameState.cpp
# End Source File
# Begin Source File

SOURCE=.\Loading.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniDump.cpp
# End Source File
# Begin Source File

SOURCE=.\PetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SharedWebData.cpp
# End Source File
# Begin Source File

SOURCE=.\StdH.cpp

!IF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# ADD BASE CPP /Zi /Yc"StdH.h"
# ADD CPP /Zi /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Help\UISupport.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\Version.cpp
# End Source File
# Begin Source File

SOURCE=.\World\WorldData.cpp
# End Source File
# Begin Source File

SOURCE=.\World\WorldString.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;inl;hxx"
# Begin Group "Base Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Base\Anim.h
# End Source File
# Begin Source File

SOURCE=.\Base\Assert.h
# End Source File
# Begin Source File

SOURCE=.\Base\Base.h
# End Source File
# Begin Source File

SOURCE=.\Base\Changeable.h
# End Source File
# Begin Source File

SOURCE=.\Base\ChangeableRT.h
# End Source File
# Begin Source File

SOURCE=.\Base\Console.h
# End Source File
# Begin Source File

SOURCE=.\Base\Console_internal.h
# End Source File
# Begin Source File

SOURCE=.\Base\CRC.h
# End Source File
# Begin Source File

SOURCE=.\Base\CRCTable.h
# End Source File
# Begin Source File

SOURCE=.\Base\CTString.h
# End Source File
# Begin Source File

SOURCE=.\Base\CTString.inl
# End Source File
# Begin Source File

SOURCE=.\Base\CTWString.h
# End Source File
# Begin Source File

SOURCE=.\Base\DamageLogInfo.h
# End Source File
# Begin Source File

SOURCE=.\Base\ErrorReporting.h
# End Source File
# Begin Source File

SOURCE=.\Base\ErrorTable.h
# End Source File
# Begin Source File

SOURCE=.\Base\FileName.h
# End Source File
# Begin Source File

SOURCE=.\Base\GroupFile.h
# End Source File
# Begin Source File

SOURCE=.\Base\IFeel.h
# End Source File
# Begin Source File

SOURCE=.\Base\Input.h
# End Source File
# Begin Source File

SOURCE=.\Base\KeyNames.h
# End Source File
# Begin Source File

SOURCE=.\Base\ListIterator.inl
# End Source File
# Begin Source File

SOURCE=.\Base\Lists.h
# End Source File
# Begin Source File

SOURCE=.\Base\Lists.inl
# End Source File
# Begin Source File

SOURCE=.\Base\md5.h
# End Source File
# Begin Source File

SOURCE=.\Base\md5_loc.h
# End Source File
# Begin Source File

SOURCE=.\Base\Memory.h
# End Source File
# Begin Source File

SOURCE=.\Base\MemoryTracking.h
# End Source File
# Begin Source File

SOURCE=.\Base\ObjectRestore.h
# End Source File
# Begin Source File

SOURCE=.\Base\ParsingSymbols.h
# End Source File
# Begin Source File

SOURCE=.\Base\PersistentSymbolsEncode.h
# End Source File
# Begin Source File

SOURCE=.\Base\Priority.inl
# End Source File
# Begin Source File

SOURCE=.\Base\Profiling.h
# End Source File
# Begin Source File

SOURCE=.\Base\ProfilingEnabled.h
# End Source File
# Begin Source File

SOURCE=.\Base\ProgressHook.h
# End Source File
# Begin Source File

SOURCE=.\Base\Protection.h
# End Source File
# Begin Source File

SOURCE=.\Base\Rect.h
# End Source File
# Begin Source File

SOURCE=.\Base\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Base\Relations.h
# End Source File
# Begin Source File

SOURCE=.\Base\ReplaceFile.h
# End Source File
# Begin Source File

SOURCE=.\Base\Serial.h
# End Source File
# Begin Source File

SOURCE=.\Base\Shell.h
# End Source File
# Begin Source File

SOURCE=.\Base\Shell_internal.h
# End Source File
# Begin Source File

SOURCE=.\Base\SimpleHashTable.h
# End Source File
# Begin Source File

SOURCE=.\Base\StackDump.h
# End Source File
# Begin Source File

SOURCE=.\Base\Statistics.h
# End Source File
# Begin Source File

SOURCE=.\Base\Statistics_Internal.h
# End Source File
# Begin Source File

SOURCE=.\Base\Stream.h
# End Source File
# Begin Source File

SOURCE=.\Base\Synchronization.h
# End Source File
# Begin Source File

SOURCE=.\Base\Timer.h
# End Source File
# Begin Source File

SOURCE=.\Base\Timer.inl
# End Source File
# Begin Source File

SOURCE=.\Base\TLVar.h
# End Source File
# Begin Source File

SOURCE=.\Base\Translation.h
# End Source File
# Begin Source File

SOURCE=.\Base\TranslationPair.h
# End Source File
# Begin Source File

SOURCE=.\Base\Types.h
# End Source File
# Begin Source File

SOURCE=.\Base\Unzip.h
# End Source File
# Begin Source File

SOURCE=.\Base\Updateable.h
# End Source File
# Begin Source File

SOURCE=.\Base\UpdateableRT.h
# End Source File
# End Group
# Begin Group "Math Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Math\AABBox.h
# End Source File
# Begin Source File

SOURCE=.\Math\AdditionalFunction.h
# End Source File
# Begin Source File

SOURCE=.\Math\Clipping.inl
# End Source File
# Begin Source File

SOURCE=.\Math\ExtensionMath.h
# End Source File
# Begin Source File

SOURCE=.\Math\FixInt.h
# End Source File
# Begin Source File

SOURCE=.\Math\Float.h
# End Source File
# Begin Source File

SOURCE=.\Math\Functions.h
# End Source File
# Begin Source File

SOURCE=.\Math\Geometry.h
# End Source File
# Begin Source File

SOURCE=.\Math\Geometry.inl
# End Source File
# Begin Source File

SOURCE=.\Math\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Math\Matrix12.h
# End Source File
# Begin Source File

SOURCE=.\Math\OBBox.h
# End Source File
# Begin Source File

SOURCE=.\Math\Object3D.h
# End Source File
# Begin Source File

SOURCE=.\Math\Placement.h
# End Source File
# Begin Source File

SOURCE=.\Math\Plane.h
# End Source File
# Begin Source File

SOURCE=.\Math\Projection.h
# End Source File
# Begin Source File

SOURCE=.\Math\Projection_DOUBLE.h
# End Source File
# Begin Source File

SOURCE=.\Math\Quaternion.h
# End Source File
# Begin Source File

SOURCE=.\Math\TextureMapping.h
# End Source File
# Begin Source File

SOURCE=.\Math\Vector.h
# End Source File
# End Group
# Begin Group "Graphics Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Graphics\Adapter.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Color.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\DisplayMode.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\DrawPort.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\dxtlib.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Fog.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Fog_internal.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Font.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_wrapper.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\GfxLibrary.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\gl_functions.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\gl_types.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\ImageInfo.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\MultiMonitor.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\OpenGL.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Raster.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\RenderPoly.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\RenderScene.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Shader.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\ShaderCode.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\ShadowMap.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Stereo.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\TextureEffects.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Vertex.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\ViewPort.h
# End Source File
# End Group
# Begin Group "Sound Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sound\al_functions.h
# End Source File
# Begin Source File

SOURCE=.\Sound\eax.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundData.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundDecoder.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundLibrary.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundObject.h
# End Source File
# End Group
# Begin Group "Models Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Models\EditModel.h
# End Source File
# Begin Source File

SOURCE=.\Models\MipMaker.h
# End Source File
# Begin Source File

SOURCE=.\Models\Model.h
# End Source File
# Begin Source File

SOURCE=.\Models\Model_internal.h
# End Source File
# Begin Source File

SOURCE=.\Models\ModelData.h
# End Source File
# Begin Source File

SOURCE=.\Models\ModelObject.h
# End Source File
# Begin Source File

SOURCE=.\Models\Normals.h
# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel.h
# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel_internal.h
# End Source File
# End Group
# Begin Group "Templates Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Templates\AllocationArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\BSP.h
# End Source File
# Begin Source File

SOURCE=.\Templates\BSP_internal.h
# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicContainer.h
# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicStackArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\HashTableTemplate.h
# End Source File
# Begin Source File

SOURCE=.\Templates\LinearAllocator.h
# End Source File
# Begin Source File

SOURCE=.\Templates\MapOfData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable.h
# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable_CTFileName.h
# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable_CTranslationPair.h
# End Source File
# Begin Source File

SOURCE=.\Templates\ReusableContainer.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Selection.h
# End Source File
# Begin Source File

SOURCE=.\Templates\StaticArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\StaticStackArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CAnimData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CAnimSet.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CEntityClass.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CFontData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CMesh.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CModelData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CModelInstance.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CShader.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CSkeleton.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CSoundData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CTextureData.h
# End Source File
# End Group
# Begin Group "Rendering Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Rendering\Render.h
# End Source File
# Begin Source File

SOURCE=.\Rendering\Render_internal.h
# End Source File
# End Group
# Begin Group "Entities Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Entities\Action.h
# End Source File
# Begin Source File

SOURCE=.\Entities\AffinityData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ArmorPreview.h
# End Source File
# Begin Source File

SOURCE=.\Entities\CashShopData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\CharacterAnimation.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Entity.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityClass.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityCollision.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityEvent.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityPointer.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityProperties.h
# End Source File
# Begin Source File

SOURCE=.\Entities\FieldSettings.h
# End Source File
# Begin Source File

SOURCE=.\Entities\InternalClasses.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ItemData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ItemEffect.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Items.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Lacarette.h
# End Source File
# Begin Source File

SOURCE=.\Entities\LastPositions.h
# End Source File
# Begin Source File

SOURCE=.\Entities\LevelupGuide.h
# End Source File
# Begin Source File

SOURCE=.\Entities\MissionCase.h
# End Source File
# Begin Source File

SOURCE=.\Entities\MobData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\NoticeData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\NpcHelp.h
# End Source File
# Begin Source File

SOURCE=.\Entities\OptionData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\PlayerCharacter.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Precaching.h
# End Source File
# Begin Source File

SOURCE=.\Entities\QuestSystem.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ShadingInfo.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ShopData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Skill.h
# End Source File
# Begin Source File

SOURCE=.\Entities\SkillTree.h
# End Source File
# Begin Source File

SOURCE=.\Entities\SmcInfo.h
# End Source File
# Begin Source File

SOURCE=.\Entities\SmcParser.h
# End Source File
# Begin Source File

SOURCE=.\Entities\SpecialSkill.h
# End Source File
# Begin Source File

SOURCE=.\Entities\StatusEffect.h
# End Source File
# Begin Source File

SOURCE=.\Entities\TargetInfo.h
# End Source File
# Begin Source File

SOURCE=.\Entities\TEventString.h
# End Source File
# Begin Source File

SOURCE=.\Entities\TradeItem.h
# End Source File
# Begin Source File

SOURCE=.\Entities\WildPetData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ZoneData.h
# End Source File
# End Group
# Begin Group "Brushes Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Brushes\Brush.h
# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushArchive.h
# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushBase.h
# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushTransformed.h
# End Source File
# End Group
# Begin Group "Network Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\ActionBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\CharacterTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\ChatMsgBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\ClientInterface.h
# End Source File
# Begin Source File

SOURCE=.\Network\cmd.h
# End Source File
# Begin Source File

SOURCE=.\Network\CNetwork.h
# End Source File
# Begin Source File

SOURCE=.\Network\Common.h
# End Source File
# Begin Source File

SOURCE=.\Network\CommunicationInterface.h
# End Source File
# Begin Source File

SOURCE=.\Network\Compression.h
# End Source File
# Begin Source File

SOURCE=.\Network\CPacket.h
# End Source File
# Begin Source File

SOURCE=.\Network\CryptNetMsg.h
# End Source File
# Begin Source File

SOURCE=.\Network\CryptNetMsg_TransTable
# End Source File
# Begin Source File

SOURCE=.\Network\Diff.h
# End Source File
# Begin Source File

SOURCE=.\Network\EMsgBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\EntityHashing.h
# End Source File
# Begin Source File

SOURCE=.\Network\InputBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\ItemTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\LevelChange.h
# End Source File
# Begin Source File

SOURCE=.\Network\MessageDefine.h
# End Source File
# Begin Source File

SOURCE=.\Network\MessageDispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Network\MobTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\MultiTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkMessage.h
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkProfile.h
# End Source File
# Begin Source File

SOURCE=.\Network\PetTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\PlayerSource.h
# End Source File
# Begin Source File

SOURCE=.\Network\PlayerTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\Server.h
# End Source File
# Begin Source File

SOURCE=.\Network\SessionSocket.h
# End Source File
# Begin Source File

SOURCE=.\Network\SessionState.h
# End Source File
# Begin Source File

SOURCE=.\Light\Shadows_internal.h
# End Source File
# Begin Source File

SOURCE=.\Network\SharedWebData.h
# End Source File
# Begin Source File

SOURCE=.\Network\SlaveTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\TcpIpConnection.h
# End Source File
# Begin Source File

SOURCE=.\Network\ThreadWrapper.h
# End Source File
# Begin Source File

SOURCE=.\Network\TxtQueue.h
# End Source File
# Begin Source File

SOURCE=.\Network\Web.h
# End Source File
# Begin Source File

SOURCE=.\Network\WebAddress.h
# End Source File
# End Group
# Begin Group "Light Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Light\Gradient.h
# End Source File
# Begin Source File

SOURCE=.\Light\LensFlares.h
# End Source File
# Begin Source File

SOURCE=.\Light\LightSource.h
# End Source File
# End Group
# Begin Group "exploration3d"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\exploration3d\E3BASE.H
# End Source File
# Begin Source File

SOURCE=.\exploration3d\E3CALL.H
# End Source File
# Begin Source File

SOURCE=.\exploration3d\E3EXT.H
# End Source File
# Begin Source File

SOURCE=.\exploration3d\E3TYPES.H
# End Source File
# Begin Source File

SOURCE=.\exploration3d\EXPLOR3D.H
# End Source File
# End Group
# Begin Group "Ska Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ska\AnimSet.h
# End Source File
# Begin Source File

SOURCE=.\Ska\Mesh.h
# End Source File
# Begin Source File

SOURCE=.\Ska\ModelInstance.h
# End Source File
# Begin Source File

SOURCE=.\Ska\NmFileIO.h
# End Source File
# Begin Source File

SOURCE=.\Ska\ParsingSmbs.h
# End Source File
# Begin Source File

SOURCE=.\Ska\Render.h
# End Source File
# Begin Source File

SOURCE=.\Ska\SkaAssert.h
# End Source File
# Begin Source File

SOURCE=.\Ska\Skeleton.h
# End Source File
# Begin Source File

SOURCE=.\Ska\StringTable.h
# End Source File
# End Group
# Begin Group "World Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\World\World.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldCollision.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldEditingProfile.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldEntityHashing.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldRayCasting.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldSettings.h
# End Source File
# End Group
# Begin Group "Terrain Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Terrain\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Terrain\Terrain.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainArchive.h
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainTile.h
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainTile.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRDataArray.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TREditing.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRGenerate.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRLayer.h
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRLayer.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRMisc.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRRayCast.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShadows.hpp
# End Source File
# End Group
# Begin Group "XBox Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XBox\SymbolLocator_X.h
# End Source File
# Begin Source File

SOURCE=.\XBox\VideoDecoder_X.h
# End Source File
# Begin Source File

SOURCE=.\XBox\XKeyboard.h
# End Source File
# End Group
# Begin Group "Effect Headers"

# PROP Default_Filter ""
# Begin Group "Effects Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CCameraEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectControl.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectGroup.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectGroupManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEntityEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CLightEffect.h
# End Source File
# Begin Source File

SOURCE=..\Engine/Effect/CMdlEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\COrbitEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CShockWaveEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSkaEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSoundEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSplineBillboardEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSplinePathEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CTerrainEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CTraceEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\TimerItem.h
# End Source File
# End Group
# Begin Group "Particle Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CConeSpace.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CCylinderSpace.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CForce.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticle.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleGroup.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleGroupManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticles.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorption.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorptionDefault.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorptionSphere.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesCommonProcess.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitter.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterCone.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterCylinder.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterSphere.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessControl.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessDynamicState.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessForce.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessPointGoal.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessVelocity.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CRandomTable.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSphereSpace.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CVelocity.h
# End Source File
# End Group
# Begin Group "Tags Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CEntityTag.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CGroupTag.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CRefCountPtr.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSkaTag.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CTag.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CTagManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CWorldTag.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Effect\CManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CNiceWater.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSampleSpline.h
# End Source File
# Begin Source File

SOURCE=.\Effect\EffectCommon.h
# End Source File
# Begin Source File

SOURCE=.\Effect\FreeMemStack.h
# End Source File
# End Group
# Begin Group "Interface Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Contents\Base\ChattingCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\ChattingUI.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAD.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAffinity.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAffinityInfo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAutoHelp.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBingoBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBuff.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICashShopEX.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICheckButton.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChildInvenSlot.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChildQuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICollectBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICompound.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIDrawFigure.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIEventPopup.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIExchange.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFiltering.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFlowerTree.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFortune.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGamble.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGroup.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuild.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildBattle.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildMark.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildNotice.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildStash.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildWarPortal.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGWMix.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelp.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelper.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelpIcon.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageArray.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInitJob.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInternalClasses.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInvenCashBag.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInvenCashBagBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInventory.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIItemProduct.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UILacarette.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMap.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessageBoxL.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessenger.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMinigame.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMix.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMixNew.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMonsterCombo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMonsterMercenary.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMouseCursor.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UINickName.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UINpcHelp.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UINpcScroll.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIOption.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIOXQuizEvent.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPartyAuto.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPersonalShop.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetFree.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetInfo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetItemMix.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetTarget.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetTraining.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPlayerInfo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProcess.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProcessNPC.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProduct.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRadar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRanking.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRectString.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRefine.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIReformSystem.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIResurrection.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISecurity.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectList.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectResource.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectWord.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIShop.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfare.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfareDoc.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfareNew.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISignBoard.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISimplePop.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISingleBattle.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISkillLearn.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISkillToolTip.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISocketSystem.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISummon.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISystemMenu.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITalk.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITatoo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITeleport.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITrackPopup.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWareHouse.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWebBoard.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWildPetInfo.h
# End Source File
# End Group
# Begin Group "Design Classes Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DesignClasses\Singleton\SingletonBase.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Build.h
# End Source File
# Begin Source File

SOURCE=.\CurrentVersion.h
# End Source File
# Begin Source File

SOURCE=.\Engine.h
# End Source File
# Begin Source File

SOURCE=.\GameGuardInterface.h
# End Source File
# Begin Source File

SOURCE=.\GameShell.h
# End Source File
# Begin Source File

SOURCE=.\GameState.h
# End Source File
# Begin Source File

SOURCE=.\GlobalDefinition.h
# End Source File
# Begin Source File

SOURCE=.\Loading.h
# End Source File
# Begin Source File

SOURCE=.\LocalDefine.h
# End Source File
# Begin Source File

SOURCE=.\MiniDump.h
# End Source File
# Begin Source File

SOURCE=.\Sound\ov_functions.h
# End Source File
# Begin Source File

SOURCE=.\PetInfo.h
# End Source File
# Begin Source File

SOURCE=.\StdH.h
# End Source File
# Begin Source File

SOURCE=.\Help\UISupport.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Generated Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Classes\BaseEvents.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=.\Classes\MovableBrushEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\MovableEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\MovableModelEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\PlayerEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\SCANNER.CPP
# End Source File
# Begin Source File

SOURCE=.\Ska\smcPars.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Ska\SMCSCAN.CPP
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Entity Class Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Classes\BaseEvents.es

!IF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Classes\MovableBrushEntity.es

!IF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Classes\MovableEntity.es

!IF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Classes\MovableModelEntity.es

!IF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Classes\PlayerEntity.es

!IF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 USALIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 RUSLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 BRZLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAXLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 KORTEST"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 THAILIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 WORLDEDITOR"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 DebugKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 ReleaseKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 GERLIVEKalydo"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 CHNLIVE"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 MAKEDEF"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lua\lauxlib.h
# End Source File
# Begin Source File

SOURCE=.\lua\lua.h
# End Source File
# Begin Source File

SOURCE=.\lua\LuaInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\lua\LuaInterface.h
# End Source File
# Begin Source File

SOURCE=.\lua\lualib.h
# End Source File
# End Group
# Begin Group "Kalydo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Kalydo\PackageManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Kalydo\PackageManager.h
# End Source File
# End Group
# Begin Group "Secure"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Secure\FileSecure.cpp
# End Source File
# Begin Source File

SOURCE=.\Secure\FileSecure.h
# End Source File
# End Group
# Begin Group "Help"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Help\DefineHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Help\DefineHelp.h
# End Source File
# Begin Source File

SOURCE=.\Help\ItemHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Help\ItemHelp.h
# End Source File
# Begin Source File

SOURCE=.\Help\Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\Help\Loader.h
# End Source File
# Begin Source File

SOURCE=.\Help\LoadLod.h
# End Source File
# Begin Source File

SOURCE=.\Help\LoadString.cpp
# End Source File
# Begin Source File

SOURCE=.\Help\LoadString.h
# End Source File
# Begin Source File

SOURCE=.\Help\Util_Help.cpp
# End Source File
# Begin Source File

SOURCE=.\Help\Util_Help.h
# End Source File
# End Group
# Begin Group "SEED_256_KISA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SEED_256_KISA\SEED_256_KISA.cpp
# End Source File
# Begin Source File

SOURCE=.\SEED_256_KISA\SEED_256_KISA.h
# End Source File
# Begin Source File

SOURCE=.\SEED_256_KISA\SEED_256_KISA.tab
# End Source File
# End Group
# Begin Group "Contents"

# PROP Default_Filter ""
# Begin Group "Base No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Contents\Base\Auction.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Auction.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\AuctionCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Calendar.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Calendar.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\ChangeWeapon.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\ChangeWeapon.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\CharacterInfoCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Durability.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Durability.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\ExpressSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\ExpressSystem.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\InvenData.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\InvenData.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Notice.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Notice.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Party.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Party.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\PetStash.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\PetStash.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\PetStashCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\PetStashSelectEffectCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\RankingList.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\RankingList.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\RankingSystemCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Syndicate.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Syndicate.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIAuctionNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIAuctionNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIAuctionNew_favorite.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIAuctionNew_regist.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIAuctionNew_search.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UICalendar.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UICalendar.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UICalendarEventInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UICalendarEventInfo.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIChangeEquipment.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIChangeEquipment.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIChangeWeaponNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIChangeWeaponNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UICharacterInfoNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UICharacterInfoNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIDurability.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIDurability.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIExpedition.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIExpedition.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIExpressItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIExpressItemList.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIExpressSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIExpressSystem.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMapOption.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMapOption.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMysteriousBead.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMysteriousBead.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMySyndicateInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMySyndicateInfo.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UINoticeNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UINoticeNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIPartyNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIPartyNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIPetStash.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIPetStash.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIPetStashSelectEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIPetStashSelectEffect.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIRankingSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIRankingSystem.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UISkillNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UISkillNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UITrade.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UITrade.h
# End Source File
# End Group
# Begin Group "Quest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Contents\Base\Quest.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\Quest.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestAccept.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestAccept.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestBookNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestBookNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestComplete.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestComplete.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestDesign.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestDesign.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestRestore.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestRestore.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestView.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIQuestView.h
# End Source File
# End Group
# Begin Group "Login"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Contents\Login\BackImageManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\BackImageManager.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\CharacterCreateNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\CharacterCreateNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\CharacterCreateNewCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\CharacterSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\CharacterSelect.h
# End Source File
# Begin Source File

SOURCE=.\LoginJobInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginJobInfo.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\LoginNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\LoginNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\LoginNewCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\ServerSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\ServerSelect.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UICharacterCreateNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UICharacterCreateNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UICharacterSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UICharacterSelect.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UIHardCoreWarning.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UIHardCoreWarning.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UILoginNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UILoginNew.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UIServerSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Login\UIServerSelect.h
# End Source File
# End Group
# Begin Group "Function"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Contents\function\attendance.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\attendance.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\gps.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\gps.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\HelpWebUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\HelpWebUI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCollection.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCollectionCmd.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCollectionData.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCollectionData.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCollectionUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCollectionUI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCompose.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemCompose.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemComposeUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\ItemComposeUI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\News.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\News.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\News_Web_UI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\News_Web_UI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\NewsUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\NewsUI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\PremiumChar.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\PremiumChar.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\PremiumCharDesign.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\PremiumCharDesign.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\PremiumCharUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\PremiumCharUI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\SimplePlayerInfoUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\SimplePlayerInfoUI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\SimplePlayerMenuUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\SimplePlayerMenuUI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\TargetInfoNewUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\TargetInfoNewUI.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\UIGps.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\UIGps.h
# End Source File
# Begin Source File

SOURCE=.\Contents\function\UIPortalNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\function\UIPortalNew.h
# End Source File
# End Group
# End Group
# Begin Group "GameDataManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameDataManager\GameDataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDataManager\GameDataManager.h
# End Source File
# End Group
# Begin Group "UILib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Interface\UIArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIArray.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIArrayItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIArrayItem.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBase.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIButton.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIComboBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICommand.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICommon.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIDefine.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIEditBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFactory.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFocus.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFocus.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIIcon.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImage.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImage.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageFont.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageFont.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageSplit.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageText.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageText.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIIME.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIIME.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIList.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIList.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBoxEx.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBoxMultiList.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBoxMultiList.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListItem.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIManager.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultiEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultiEditBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultiText.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultiText.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultList.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultList.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISlideBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISlideBar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISpinButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISpinButton.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISpinControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISpinControl.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISpriteAni.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISpriteAni.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITab.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITab.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIText.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIText.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextBoxEx.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextEx.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextureManager.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITooltip.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITree.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITree.h
# End Source File
# End Group
# Begin Group "UIHelper"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBoxIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBoxInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBoxMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBoxMgr.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBoxNumeric_only.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBoxNumeric_only.h
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBoxTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Contents\Base\UIMsgBoxYesNo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITooltipMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITooltipMgr.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITooltipResource.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITooltipResource.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWindowDeclare.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWindowDeclare.h
# End Source File
# End Group
# Begin Group "Object"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Object\ActorMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Object\ActorMgr.h
# End Source File
# Begin Source File

SOURCE=.\Object\DefineObject.h
# End Source File
# Begin Source File

SOURCE=.\JobInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\JobInfo.h
# End Source File
# Begin Source File

SOURCE=.\Object\ObjectBase.h
# End Source File
# Begin Source File

SOURCE=.\SlaveInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SlaveInfo.h
# End Source File
# Begin Source File

SOURCE=.\TransformInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TransformInfo.h
# End Source File
# End Group
# Begin Group "Info"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Info\MyInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Info\MyInfo.h
# End Source File
# Begin Source File

SOURCE=.\Info\MyInfoSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Info\MyInfoSkill.h
# End Source File
# Begin Source File

SOURCE=.\Info\ServerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Info\ServerInfo.h
# End Source File
# End Group
# Begin Group "StageMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameStageManager\StageCreateChar.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageCreateChar.h
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageGameEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageGameEnd.h
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageGamePlay.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageGamePlay.h
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageIntro.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageIntro.h
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageLoading.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageLoading.h
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageLogin.h
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageMgr.h
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageSelChar.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageSelChar.h
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageSelServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStageManager\StageSelServer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChangeLog.txt
# End Source File
# End Target
# End Project
