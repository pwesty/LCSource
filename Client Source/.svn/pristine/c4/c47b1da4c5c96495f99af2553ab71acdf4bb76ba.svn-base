# Microsoft Developer Studio Project File - Name="GameGUIMP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GameGUIMP - Win32 DebugKalydo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameGUIMP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameGUIMP.mak" CFG="GameGUIMP - Win32 DebugKalydo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameGUIMP - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 FinalRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 FinalDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 USALIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 USATEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 GERLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 GERTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 RUSLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 RUSTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 JPNLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 JPNTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 HKLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 HKTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 EU2LIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 EU2TEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 BRZLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 BRZTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 EU3TEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 EU3LIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 MAXLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 MAXTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 NETLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 NETTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 KORLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 KORTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 THAITEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 THAILIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 WORLDEDITOR" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 DebugKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 ReleaseKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameGUIMP - Win32 GERLIVEKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameGUIMP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\Release\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/GameGUIMPD.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\Debug\GameGUIMPD.dll
InputName=GameGUIMPD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).dll $(ENGINE_DIR)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy Debug\$(InputName).lib $(ENGINE_DIR)\Bin\Debug >nul 
	copy Debug\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build
# Begin Special Build Tool
OutDir=.\Debug
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir    $(OutDir)\..\..\DebugFilesForCopy   	copy    $(OutDir)\GameGUIMPD.dll    $(OutDir)\..\..\DebugFilesForCopy\   	copy    $(OutDir)\GameGUIMPD.lib    $(OutDir)\..\..\DebugFilesForCopy\   	copy    $(OutDir)\GameGUIMPD.map    $(OutDir)\..\..\DebugFilesForCopy\   	copy    $(OutDir)\GameGUIMPD.pdb    $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_FinalRelease"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_FinalRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FinalRelease"
# PROP Intermediate_Dir "FinalRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "FINALVERSION" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\FinalRelease\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalRelease\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy FinalRelease\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy FinalRelease\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy FinalRelease\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameGUIMP___Win32_FinalDebug"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_FinalDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FinalDebug"
# PROP Intermediate_Dir "FinalDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "FINALVERSION" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/GameGUIMPD.dll" /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"FinalDebug/GameGUIMPD.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\FinalDebug\GameGUIMPD.dll
InputName=GameGUIMPD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalDebug\$(InputName).dll $(ENGINE_DIR)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).lib $(ENGINE_DIR)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USALIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_USALIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_USALIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "USALIVE"
# PROP Intermediate_Dir "USALIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_USA" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(USA_LIVE)\Bin
InputPath=.\USALIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(USA_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy USALIVE\$(InputName).dll $(USA_LIVE)\Bin >nul 
	copy USALIVE\$(InputName).map $(USA_LIVE)\Bin >nul 
	copy USALIVE\$(InputName).lib $(USA_LIVE)\Bin >nul 
	copy USALIVE\$(InputName).pdb $(USA_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USATEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_USATEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_USATEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "USATEST"
# PROP Intermediate_Dir "USATEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_USA" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(USA_TEST)\Bin
InputPath=.\USATEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(USA_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy USATEST\$(InputName).dll $(USA_TEST)\Bin >nul 
	copy USATEST\$(InputName).map $(USA_TEST)\Bin >nul 
	copy USATEST\$(InputName).lib $(USA_TEST)\Bin >nul 
	copy USATEST\$(InputName).pdb $(USA_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_GERLIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_GERLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GERLIVE"
# PROP Intermediate_Dir "GERLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_GERMAN" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(GER_LIVE)\Bin
InputPath=.\GERLIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(GER_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy GERLIVE\$(InputName).dll $(GER_LIVE)\Bin >nul 
	copy GERLIVE\$(InputName).map $(GER_LIVE)\Bin >nul 
	copy GERLIVE\$(InputName).lib $(GER_LIVE)\Bin >nul 
	copy GERLIVE\$(InputName).pdb $(GER_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_GERTEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_GERTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GERTEST"
# PROP Intermediate_Dir "GERTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "G_GERMAN" /D "VER_TEST" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(GER_TEST)\Bin
InputPath=.\GERTEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(GER_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy GERTEST\$(InputName).dll $(GER_TEST)\Bin >nul 
	copy GERTEST\$(InputName).map $(GER_TEST)\Bin >nul 
	copy GERTEST\$(InputName).lib $(GER_TEST)\Bin >nul 
	copy GERTEST\$(InputName).pdb $(GER_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_RUSLIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_RUSLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "RUSLIVE"
# PROP Intermediate_Dir "RUSLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_RUSSIA" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(RUS_LIVE)\Bin
InputPath=.\RUSLIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(RUS_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy RUSLIVE\$(InputName).dll $(RUS_LIVE)\Bin >nul 
	copy RUSLIVE\$(InputName).map $(RUS_LIVE)\Bin >nul 
	copy RUSLIVE\$(InputName).lib $(RUS_LIVE)\Bin >nul 
	copy RUSLIVE\$(InputName).pdb $(RUS_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_RUSTEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_RUSTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "RUSTEST"
# PROP Intermediate_Dir "RUSTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_RUSSIA" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(RUS_TEST)\Bin
InputPath=.\RUSTEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(RUS_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy RUSTEST\$(InputName).dll $(RUS_TEST)\Bin >nul 
	copy RUSTEST\$(InputName).map $(RUS_TEST)\Bin >nul 
	copy RUSTEST\$(InputName).lib $(RUS_TEST)\Bin >nul 
	copy RUSTEST\$(InputName).pdb $(RUS_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_JPNLIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_JPNLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "JPNLIVE"
# PROP Intermediate_Dir "JPNLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_JAPAN" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(JPN_LIVE)\Bin
InputPath=.\JPNLIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(JPN_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy JPNLIVE\$(InputName).dll $(JPN_LIVE)\Bin >nul 
	copy JPNLIVE\$(InputName).map $(JPN_LIVE)\Bin >nul 
	copy JPNLIVE\$(InputName).lib $(JPN_LIVE)\Bin >nul 
	copy JPNLIVE\$(InputName).pdb $(JPN_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_JPNTEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_JPNTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "JPNTEST"
# PROP Intermediate_Dir "JPNTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_JAPAN" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(JPN_TEST)\Bin
InputPath=.\JPNTEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(JPN_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy JPNTEST\$(InputName).dll $(JPN_TEST)\Bin >nul 
	copy JPNTEST\$(InputName).map $(JPN_TEST)\Bin >nul 
	copy JPNTEST\$(InputName).lib $(JPN_TEST)\Bin >nul 
	copy JPNTEST\$(InputName).pdb $(JPN_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_HKLIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_HKLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "HKLIVE"
# PROP Intermediate_Dir "HKLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_HONGKONG" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(HK_LIVE)\Bin
InputPath=.\HKLIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(HK_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy HKLIVE\$(InputName).dll $(HK_LIVE)\Bin >nul 
	copy HKLIVE\$(InputName).map $(HK_LIVE)\Bin >nul 
	copy HKLIVE\$(InputName).lib $(HK_LIVE)\Bin >nul 
	copy HKLIVE\$(InputName).pdb $(HK_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_HKTEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_HKTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "HKTEST"
# PROP Intermediate_Dir "HKTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_HONGKONG" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(HK_TEST)\Bin
InputPath=.\HKTEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(HK_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy HKTEST\$(InputName).dll $(HK_TEST)\Bin >nul 
	copy HKTEST\$(InputName).map $(HK_TEST)\Bin >nul 
	copy HKTEST\$(InputName).lib $(HK_TEST)\Bin >nul 
	copy HKTEST\$(InputName).pdb $(HK_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2LIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_EU2LIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_EU2LIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EU2LIVE"
# PROP Intermediate_Dir "EU2LIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_EUROPE2" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(EU2_LIVE)\Bin
InputPath=.\EU2LIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(EU2_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy EU2LIVE\$(InputName).dll $(EU2_LIVE)\Bin >nul 
	copy EU2LIVE\$(InputName).map $(EU2_LIVE)\Bin >nul 
	copy EU2LIVE\$(InputName).lib $(EU2_LIVE)\Bin >nul 
	copy EU2LIVE\$(InputName).pdb $(EU2_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2TEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_EU2TEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_EU2TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EU2TEST"
# PROP Intermediate_Dir "EU2TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_EUROPE2" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(EU2_TEST)\Bin
InputPath=.\EU2TEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(EU2_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy EU2TEST\$(InputName).dll $(EU2_TEST)\Bin >nul 
	copy EU2TEST\$(InputName).map $(EU2_TEST)\Bin >nul 
	copy EU2TEST\$(InputName).lib $(EU2_TEST)\Bin >nul 
	copy EU2TEST\$(InputName).pdb $(EU2_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_BRZLIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_BRZLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BRZLIVE"
# PROP Intermediate_Dir "BRZLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_BRAZIL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(BRZ_LIVE)\Bin
InputPath=.\BRZLIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(BRZ_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy BRZLIVE\$(InputName).dll $(BRZ_LIVE)\Bin >nul 
	copy BRZLIVE\$(InputName).map $(BRZ_LIVE)\Bin >nul 
	copy BRZLIVE\$(InputName).lib $(BRZ_LIVE)\Bin >nul 
	copy BRZLIVE\$(InputName).pdb $(BRZ_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_BRZTEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_BRZTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BRZTEST"
# PROP Intermediate_Dir "BRZTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_BRAZIL" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(BRZ_TEST)\Bin
InputPath=.\BRZTEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(BRZ_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy BRZTEST\$(InputName).dll $(BRZ_TEST)\Bin >nul 
	copy BRZTEST\$(InputName).map $(BRZ_TEST)\Bin >nul 
	copy BRZTEST\$(InputName).lib $(BRZ_TEST)\Bin >nul 
	copy BRZTEST\$(InputName).pdb $(BRZ_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3TEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_EU3TEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_EU3TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EU3TEST"
# PROP Intermediate_Dir "EU3TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_EUROPE3" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(EU3_TEST)\Bin
InputPath=.\EU3TEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(EU3_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy EU3TEST\$(InputName).dll $(EU3_TEST)\Bin >nul 
	copy EU3TEST\$(InputName).map $(EU3_TEST)\Bin >nul 
	copy EU3TEST\$(InputName).lib $(EU3_TEST)\Bin >nul 
	copy EU3TEST\$(InputName).pdb $(EU3_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3LIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_EU3LIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_EU3LIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EU3LIVE"
# PROP Intermediate_Dir "EU3LIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_EUROPE3" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(EU3_LIVE)\Bin
InputPath=.\EU3LIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(EU3_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy EU3LIVE\$(InputName).dll $(EU3_LIVE)\Bin >nul 
	copy EU3LIVE\$(InputName).map $(EU3_LIVE)\Bin >nul 
	copy EU3LIVE\$(InputName).lib $(EU3_LIVE)\Bin >nul 
	copy EU3LIVE\$(InputName).pdb $(EU3_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_MAXLIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_MAXLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MAXLIVE"
# PROP Intermediate_Dir "MAXLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_MAXICO" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(MAX_LIVE)\Bin
InputPath=.\MAXLIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(MAX_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy MAXLIVE\$(InputName).dll $(MAX_LIVE)\Bin >nul 
	copy MAXLIVE\$(InputName).map $(MAX_LIVE)\Bin >nul 
	copy MAXLIVE\$(InputName).lib $(MAX_LIVE)\Bin >nul 
	copy MAXLIVE\$(InputName).pdb $(MAX_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_MAXTEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_MAXTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MAXTEST"
# PROP Intermediate_Dir "MAXTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_MAXICO" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(MAX_TEST)\Bin
InputPath=.\MAXTEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(MAX_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy MAXTEST\$(InputName).dll $(MAX_TEST)\Bin >nul 
	copy MAXTEST\$(InputName).map $(MAX_TEST)\Bin >nul 
	copy MAXTEST\$(InputName).lib $(MAX_TEST)\Bin >nul 
	copy MAXTEST\$(InputName).pdb $(MAX_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_NETLIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_NETLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "NETLIVE"
# PROP Intermediate_Dir "NETLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_NETHERLANDS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(NET_LIVE)\Bin
InputPath=.\NETLIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(NET_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy NETLIVE\$(InputName).dll $(NET_LIVE)\Bin >nul 
	copy NETLIVE\$(InputName).map $(NET_LIVE)\Bin >nul 
	copy NETLIVE\$(InputName).lib $(NET_LIVE)\Bin >nul 
	copy NETLIVE\$(InputName).pdb $(NET_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_NETTEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_NETTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "NETTEST"
# PROP Intermediate_Dir "NETTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_NETHERLANDS" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(NET_TEST)\Bin
InputPath=.\NETTEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(NET_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy NETTEST\$(InputName).dll $(NET_TEST)\Bin >nul 
	copy NETTEST\$(InputName).map $(NET_TEST)\Bin >nul 
	copy NETTEST\$(InputName).lib $(NET_TEST)\Bin >nul 
	copy NETTEST\$(InputName).pdb $(NET_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_KORLIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_KORLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "KORLIVE"
# PROP Intermediate_Dir "KORLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_KOR" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(KOR_LIVE)\Bin
InputPath=.\KORLIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(KOR_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy KORLIVE\$(InputName).dll $(KOR_LIVE)\Bin >nul 
	copy KORLIVE\$(InputName).map $(KOR_LIVE)\Bin >nul 
	copy KORLIVE\$(InputName).lib $(KOR_LIVE)\Bin >nul 
	copy KORLIVE\$(InputName).pdb $(KOR_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_KORTEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_KORTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "KORTEST"
# PROP Intermediate_Dir "KORTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_KOR" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(KOR_TEST)\Bin
InputPath=.\KORTEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(KOR_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy KORTEST\$(InputName).dll $(KOR_TEST)\Bin >nul 
	copy KORTEST\$(InputName).map $(KOR_TEST)\Bin >nul 
	copy KORTEST\$(InputName).lib $(KOR_TEST)\Bin >nul 
	copy KORTEST\$(InputName).pdb $(KOR_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAITEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_THAITEST"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_THAITEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "THAITEST"
# PROP Intermediate_Dir "THAITEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_KOR" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_THAI" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(THAI_TEST)\Bin
InputPath=.\THAITEST\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(THAI_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy THAITEST\$(InputName).dll $(THAI_TEST)\Bin >nul 
	copy THAITEST\$(InputName).map $(THAI_TEST)\Bin >nul 
	copy THAITEST\$(InputName).lib $(THAI_TEST)\Bin >nul 
	copy THAITEST\$(InputName).pdb $(THAI_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAILIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_THAILIVE"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_THAILIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "THAILIVE"
# PROP Intermediate_Dir "THAILIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_KOR" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_THAI" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(THAI_LIVE)\Bin
InputPath=.\THAILIVE\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(THAI_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy THAILIVE\$(InputName).dll $(THAI_LIVE)\Bin >nul 
	copy THAILIVE\$(InputName).map $(THAI_LIVE)\Bin >nul 
	copy THAILIVE\$(InputName).lib $(THAI_LIVE)\Bin >nul 
	copy THAILIVE\$(InputName).pdb $(THAI_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_WORLDEDITOR"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_WORLDEDITOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WORLDEDITOR"
# PROP Intermediate_Dir "WORLDEDITOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_KOR" /D "VER_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_KOR" /D "VER_TEST" /D "_CLIENT_" /D "WORLD_EDITOR" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /O<none>
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\WORLDEDITOR\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\WorldEditor\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).dll $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).lib $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).pdb $(ENGINE_DIR)\Bin\WorldEditor >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 DebugKalydo"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameGUIMP___Win32_DebugKalydo"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_DebugKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugKalydo"
# PROP Intermediate_Dir "DebugKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/GameGUIMPD.dll" /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"DebugKalydo/GameGUIMPD.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\DebugKalydo\GameGUIMPD.dll
InputName=GameGUIMPD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy DebugKalydo\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).lib $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 ReleaseKalydo"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_ReleaseKalydo"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_ReleaseKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseKalydo"
# PROP Intermediate_Dir "ReleaseKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_GERMAN" /D "KALYDO" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\ReleaseKalydo\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ReleaseKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVEKalydo"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameGUIMP___Win32_GERLIVEKalydo"
# PROP BASE Intermediate_Dir "GameGUIMP___Win32_GERLIVEKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GERLIVEKalydo"
# PROP Intermediate_Dir "GERLIVEKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_GERMAN" /D "KALYDO" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Ow
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GAMEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "G_GERMAN" /D "KALYDO" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ow
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\GERLIVEKalydo\GameGUIMP.dll
InputName=GameGUIMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy GERLIVEKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy GERLIVEKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy GERLIVEKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy GERLIVEKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "GameGUIMP - Win32 Release"
# Name "GameGUIMP - Win32 Debug"
# Name "GameGUIMP - Win32 FinalRelease"
# Name "GameGUIMP - Win32 FinalDebug"
# Name "GameGUIMP - Win32 USALIVE"
# Name "GameGUIMP - Win32 USATEST"
# Name "GameGUIMP - Win32 GERLIVE"
# Name "GameGUIMP - Win32 GERTEST"
# Name "GameGUIMP - Win32 RUSLIVE"
# Name "GameGUIMP - Win32 RUSTEST"
# Name "GameGUIMP - Win32 JPNLIVE"
# Name "GameGUIMP - Win32 JPNTEST"
# Name "GameGUIMP - Win32 HKLIVE"
# Name "GameGUIMP - Win32 HKTEST"
# Name "GameGUIMP - Win32 EU2LIVE"
# Name "GameGUIMP - Win32 EU2TEST"
# Name "GameGUIMP - Win32 BRZLIVE"
# Name "GameGUIMP - Win32 BRZTEST"
# Name "GameGUIMP - Win32 EU3TEST"
# Name "GameGUIMP - Win32 EU3LIVE"
# Name "GameGUIMP - Win32 MAXLIVE"
# Name "GameGUIMP - Win32 MAXTEST"
# Name "GameGUIMP - Win32 NETLIVE"
# Name "GameGUIMP - Win32 NETTEST"
# Name "GameGUIMP - Win32 KORLIVE"
# Name "GameGUIMP - Win32 KORTEST"
# Name "GameGUIMP - Win32 THAITEST"
# Name "GameGUIMP - Win32 THAILIVE"
# Name "GameGUIMP - Win32 WORLDEDITOR"
# Name "GameGUIMP - Win32 DebugKalydo"
# Name "GameGUIMP - Win32 ReleaseKalydo"
# Name "GameGUIMP - Win32 GERLIVEKalydo"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActionsListControl.cpp

!IF  "$(CFG)" == "GameGUIMP - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USATEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAITEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxisListCtrl.cpp

!IF  "$(CFG)" == "GameGUIMP - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USATEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAITEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConsoleSymbolsCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAudioQuality.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditButtonAction.cpp

!IF  "$(CFG)" == "GameGUIMP - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USATEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAITEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgPlayerAppearance.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayerControls.cpp

!IF  "$(CFG)" == "GameGUIMP - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USATEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAITEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgPlayerSettings.cpp

!IF  "$(CFG)" == "GameGUIMP - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USATEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAITEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgRenameControls.cpp

!IF  "$(CFG)" == "GameGUIMP - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USATEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAITEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgSelectPlayer.cpp

!IF  "$(CFG)" == "GameGUIMP - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USALIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 USATEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 RUSTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 JPNTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 HKTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU2TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 BRZTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3TEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 EU3LIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 MAXTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 NETTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORLIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 KORTEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAITEST"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 THAILIVE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 DebugKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 ReleaseKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameGUIMP - Win32 GERLIVEKalydo"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgVideoQuality.cpp
# End Source File
# Begin Source File

SOURCE=.\EditConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\GameGUI.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalPlayersList.cpp
# End Source File
# Begin Source File

SOURCE=.\PressKeyEditControl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActionsListControl.h
# End Source File
# Begin Source File

SOURCE=.\AxisListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ConsoleSymbolsCombo.h
# End Source File
# Begin Source File

SOURCE=.\DlgAudioQuality.h
# End Source File
# Begin Source File

SOURCE=.\DlgConsole.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditButtonAction.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayerAppearance.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayerControls.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayerSettings.h
# End Source File
# Begin Source File

SOURCE=.\DlgRenameControls.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectPlayer.h
# End Source File
# Begin Source File

SOURCE=.\DlgVideoQuality.h
# End Source File
# Begin Source File

SOURCE=.\EditConsole.h
# End Source File
# Begin Source File

SOURCE=.\GameGUI.h
# End Source File
# Begin Source File

SOURCE=.\LocalPlayersList.h
# End Source File
# Begin Source File

SOURCE=.\PressKeyEditControl.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\cursor_u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Game.rc2
# End Source File
# Begin Source File

SOURCE=.\GameGUI.rc
# End Source File
# End Group
# Begin Group "Wizard Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameGUIMP.clw
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
