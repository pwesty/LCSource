# Microsoft Developer Studio Project File - Name="GameMP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GameMP - Win32 DebugKalydo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameMP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameMP.mak" CFG="GameMP - Win32 DebugKalydo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameMP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 USALIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 GERLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 RUSLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 BRZLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 MAXLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 KORTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 THAILIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 WORLDEDITOR" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 DebugKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 ReleaseKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 GERLIVEKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 CHNLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameMP - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "_Debug"
# PROP Intermediate_Dir "_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "G_KOR" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"_Debug/GameMPD.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\_Debug\GameMPD.dll
InputName=GameMPD
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
PostBuild_Cmds=mkdir                $(OutDir)\..\..\DebugFilesForCopy               	copy                $(OutDir)\GameMPD.dll                $(OutDir)\..\..\DebugFilesForCopy\               	copy                $(OutDir)\GameMPD.lib                $(OutDir)\..\..\DebugFilesForCopy\               	copy                $(OutDir)\GameMPD.map                $(OutDir)\..\..\DebugFilesForCopy\               	copy                $(OutDir)\GameMPD.pdb                $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "GameMP - Win32 USALIVE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_USALIVE"
# PROP BASE Intermediate_Dir "GameMP___Win32_USALIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_USALIVE"
# PROP Intermediate_Dir "_USALIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_USA" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(USA_LIVE)\Bin
InputPath=.\_USALIVE\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(USA_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _USALIVE\$(InputName).dll $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).map $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).lib $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).pdb $(USA_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 GERLIVE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_GERLIVE"
# PROP BASE Intermediate_Dir "GameMP___Win32_GERLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVE"
# PROP Intermediate_Dir "_GERLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_GERMAN" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(GER_LIVE)\Bin
InputPath=.\_GERLIVE\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(GER_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVE\$(InputName).dll $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).map $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).lib $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).pdb $(GER_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 RUSLIVE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_RUSLIVE"
# PROP BASE Intermediate_Dir "GameMP___Win32_RUSLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_RUSLIVE"
# PROP Intermediate_Dir "_RUSLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_RUSSIA" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(RUS_LIVE)\Bin
InputPath=.\_RUSLIVE\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(RUS_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _RUSLIVE\$(InputName).dll $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).map $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).lib $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).pdb $(RUS_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 BRZLIVE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_BRZLIVE"
# PROP BASE Intermediate_Dir "GameMP___Win32_BRZLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_BRZLIVE"
# PROP Intermediate_Dir "_BRZLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_BRAZIL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(BRZ_LIVE)\Bin
InputPath=.\_BRZLIVE\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(BRZ_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _BRZLIVE\$(InputName).dll $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).map $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).lib $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).pdb $(BRZ_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 MAXLIVE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_MAXLIVE"
# PROP BASE Intermediate_Dir "GameMP___Win32_MAXLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_MAXLIVE"
# PROP Intermediate_Dir "_MAXLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_MAXICO" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(MAX_LIVE)\Bin
InputPath=.\_MAXLIVE\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(MAX_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _MAXLIVE\$(InputName).dll $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).map $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).lib $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).pdb $(MAX_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 KORTEST"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_KORTEST"
# PROP BASE Intermediate_Dir "GameMP___Win32_KORTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_KORTEST"
# PROP Intermediate_Dir "_KORTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "_WINDLL" /D "_AFXDLL" /D "GAME_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(KOR_TEST)\Bin
InputPath=.\_KORTEST\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(KOR_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _KORTEST\$(InputName).dll $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).map $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).lib $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).pdb $(KOR_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 THAILIVE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_THAILIVE"
# PROP BASE Intermediate_Dir "GameMP___Win32_THAILIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_THAILIVE"
# PROP Intermediate_Dir "_THAILIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_KOR" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "_WINDLL" /D "_AFXDLL" /D "GAME_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_THAI" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(THAI_LIVE)\Bin
InputPath=.\_THAILIVE\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(THAI_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _THAILIVE\$(InputName).dll $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).map $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).lib $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).pdb $(THAI_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_WORLDEDITOR"
# PROP BASE Intermediate_Dir "GameMP___Win32_WORLDEDITOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WORLDEDITOR"
# PROP Intermediate_Dir "WORLDEDITOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_KOR" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_KOR" /D "VER_TEST" /D "_CLIENT_" /D "WORLD_EDITOR" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /O<none> /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\WORLDEDITOR\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\WorldEditor\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).dll $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).lib $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).pdb $(ENGINE_DIR)\Bin\WorldEditor >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 DebugKalydo"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameMP___Win32_DebugKalydo"
# PROP BASE Intermediate_Dir "GameMP___Win32_DebugKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugKalydo"
# PROP Intermediate_Dir "DebugKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_KOR" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_KOR" /D "KALYDO" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"Debug/GameMPD.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 user32.lib KRFReadLibDebug.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"DebugKalydo/GameMPD.dll" /pdbtype:sept /libpath:"..\Nksp\Compact"
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\DebugKalydo\GameMPD.dll
InputName=GameMPD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy DebugKalydo\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).lib $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 ReleaseKalydo"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_ReleaseKalydo"
# PROP BASE Intermediate_Dir "GameMP___Win32_ReleaseKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseKalydo"
# PROP Intermediate_Dir "ReleaseKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_GERMAN" /D "KALYDO" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib KRFReadLib.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386 /libpath:"..\Nksp\Compact"
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\ReleaseKalydo\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ReleaseKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 GERLIVEKalydo"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_GERLIVEKalydo"
# PROP BASE Intermediate_Dir "GameMP___Win32_GERLIVEKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVEKalydo"
# PROP Intermediate_Dir "_GERLIVEKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_GERMAN" /D "KALYDO" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_GERMAN" /D "KALYDO" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib KRFReadLib.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386 /libpath:"..\Nksp\Compact"
# ADD LINK32 user32.lib KRFReadLib.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386 /libpath:"..\Kalydo\KRFReadLib\Bin"
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\_GERLIVEKalydo\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVEKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 CHNLIVE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_CHNLIVE"
# PROP BASE Intermediate_Dir "GameMP___Win32_CHNLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_CHNLIVE"
# PROP Intermediate_Dir "_CHNLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /D "G_USA" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "_WINDLL" /D "_AFXDLL" /D "GAME_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /D "G_CHINA" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"CHNLIVE/GameMP.bsc"
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(CHN_LIVE)\Bin
InputPath=.\_CHNLIVE\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(CHN_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _CHNLIVE\$(InputName).dll $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).map $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).lib $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).pdb $(CHN_LIVE)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "GameMP - Win32 Debug"
# Name "GameMP - Win32 USALIVE"
# Name "GameMP - Win32 GERLIVE"
# Name "GameMP - Win32 RUSLIVE"
# Name "GameMP - Win32 BRZLIVE"
# Name "GameMP - Win32 MAXLIVE"
# Name "GameMP - Win32 KORTEST"
# Name "GameMP - Win32 THAILIVE"
# Name "GameMP - Win32 WORLDEDITOR"
# Name "GameMP - Win32 DebugKalydo"
# Name "GameMP - Win32 ReleaseKalydo"
# Name "GameMP - Win32 GERLIVEKalydo"
# Name "GameMP - Win32 CHNLIVE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Console.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\LCDDrawing.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WEDInterface.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\LCDDrawing.h
# End Source File
# Begin Source File

SOURCE=.\PlayerSettings.h
# End Source File
# Begin Source File

SOURCE=.\SEColors.h
# End Source File
# Begin Source File

SOURCE=.\SessionProperties.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
