# Microsoft Developer Studio Project File - Name="EntitiesMP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EntitiesMP - Win32 DebugKalydo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EntitiesMP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EntitiesMP.mak" CFG="EntitiesMP - Win32 DebugKalydo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EntitiesMP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 USALIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 GERLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 RUSLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 BRZLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 MAXLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 KORTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 THAILIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 WORLDEDITOR" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 DebugKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 ReleaseKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 GERLIVEKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EntitiesMP - Win32 CHNLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "_Debug"
# PROP Intermediate_Dir "_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /I ".\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /c
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
# ADD LINK32 user32.lib winmm.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"_Debug/EntitiesMPD.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\_Debug\EntitiesMPD.dll
InputName=EntitiesMPD
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
PostBuild_Cmds=mkdir                     $(OutDir)\..\..\DebugFilesForCopy                    	copy                     $(OutDir)\EntitiesMPD.dll                     $(OutDir)\..\..\DebugFilesForCopy\                    	copy                     $(OutDir)\EntitiesMPD.lib                     $(OutDir)\..\..\DebugFilesForCopy\                    	copy                     $(OutDir)\EntitiesMPD.map                     $(OutDir)\..\..\DebugFilesForCopy\                    	copy                     $(OutDir)\EntitiesMPD.pdb                     $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_USALIVE"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_USALIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_USALIVE"
# PROP Intermediate_Dir "_USALIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(USA_LIVE)\Bin
InputPath=.\_USALIVE\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(USA_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _USALIVE\$(InputName).dll $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).map $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).lib $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).pdb $(USA_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_GERLIVE"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_GERLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVE"
# PROP Intermediate_Dir "_GERLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(GER_LIVE)\Bin
InputPath=.\_GERLIVE\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(GER_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVE\$(InputName).dll $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).map $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).lib $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).pdb $(GER_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_RUSLIVE"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_RUSLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_RUSLIVE"
# PROP Intermediate_Dir "_RUSLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_RUSSIA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(RUS_LIVE)\Bin
InputPath=.\_RUSLIVE\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(RUS_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _RUSLIVE\$(InputName).dll $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).map $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).lib $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).pdb $(RUS_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_BRZLIVE"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_BRZLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_BRZLIVE"
# PROP Intermediate_Dir "_BRZLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_BRAZIL" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(BRZ_LIVE)\Bin
InputPath=.\_BRZLIVE\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(BRZ_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _BRZLIVE\$(InputName).dll $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).map $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).lib $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).pdb $(BRZ_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_MAXLIVE"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_MAXLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_MAXLIVE"
# PROP Intermediate_Dir "_MAXLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_MAXICO" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(MAX_LIVE)\Bin
InputPath=.\_MAXLIVE\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(MAX_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _MAXLIVE\$(InputName).dll $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).map $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).lib $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).pdb $(MAX_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_KORTEST"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_KORTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_KORTEST"
# PROP Intermediate_Dir "_KORTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I ".\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(KOR_TEST)\Bin
InputPath=.\_KORTEST\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(KOR_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _KORTEST\$(InputName).dll $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).map $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).lib $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).pdb $(KOR_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_THAILIVE"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_THAILIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_THAILIVE"
# PROP Intermediate_Dir "_THAILIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_THAI" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(THAI_LIVE)\Bin
InputPath=.\_THAILIVE\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(THAI_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _THAILIVE\$(InputName).dll $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).map $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).lib $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).pdb $(THAI_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_WORLDEDITOR"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_WORLDEDITOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WORLDEDITOR"
# PROP Intermediate_Dir "WORLDEDITOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /D "_CLIENT_" /D "WORLD_EDITOR" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /O<none> /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\WORLDEDITOR\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\WorldEditor\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).dll $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).lib $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).pdb $(ENGINE_DIR)\Bin\WorldEditor >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "EntitiesMP___Win32_DebugKalydo"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_DebugKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugKalydo"
# PROP Intermediate_Dir "DebugKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "G_KOR" /D "KALYDO" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib winmm.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"Debug/EntitiesMPD.dll" /pdbtype:sept
# ADD LINK32 user32.lib winmm.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"DebugKalydo/EntitiesMPD.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\DebugKalydo\EntitiesMPD.dll
InputName=EntitiesMPD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy DebugKalydo\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).lib $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_ReleaseKalydo"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_ReleaseKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseKalydo"
# PROP Intermediate_Dir "ReleaseKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /D "KALYDO" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\ReleaseKalydo\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ReleaseKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_GERLIVEKalydo"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_GERLIVEKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVEKalydo"
# PROP Intermediate_Dir "_GERLIVEKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /D "KALYDO" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\\" /I "..\\" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /D "KALYDO" /D "_WINDLL" /D "_AFXDLL" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\_GERLIVEKalydo\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVEKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EntitiesMP___Win32_CHNLIVE"
# PROP BASE Intermediate_Dir "EntitiesMP___Win32_CHNLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_CHNLIVE"
# PROP Intermediate_Dir "_CHNLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /D "G_CHINA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# ADD LINK32 user32.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(CHN_LIVE)\Bin
InputPath=.\_CHNLIVE\EntitiesMP.dll
InputName=EntitiesMP
SOURCE="$(InputPath)"

"$(CHN_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _CHNLIVE\$(InputName).dll $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).map $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).lib $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).pdb $(CHN_LIVE)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "EntitiesMP - Win32 Debug"
# Name "EntitiesMP - Win32 USALIVE"
# Name "EntitiesMP - Win32 GERLIVE"
# Name "EntitiesMP - Win32 RUSLIVE"
# Name "EntitiesMP - Win32 BRZLIVE"
# Name "EntitiesMP - Win32 MAXLIVE"
# Name "EntitiesMP - Win32 KORTEST"
# Name "EntitiesMP - Win32 THAILIVE"
# Name "EntitiesMP - Win32 WORLDEDITOR"
# Name "EntitiesMP - Win32 DebugKalydo"
# Name "EntitiesMP - Win32 ReleaseKalydo"
# Name "EntitiesMP - Win32 GERLIVEKalydo"
# Name "EntitiesMP - Win32 CHNLIVE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Group "Common sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Common\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\EmanatingParticles.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\FixedCameraContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\HUD.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\Particles.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\PathFinding.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\RestrictedFieldContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\StdH\StdH.cpp
# ADD CPP /Yc"StdH.h"
# End Source File
# Begin Source File

SOURCE=.\Common\Version.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AnimationChanger.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimationHub.cpp
# End Source File
# Begin Source File

SOURCE=.\AreaMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\BackgroundViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicEffects.cpp
# End Source File
# Begin Source File

SOURCE=.\BlendController.cpp
# End Source File
# Begin Source File

SOURCE=.\BloodSpray.cpp
# End Source File
# Begin Source File

SOURCE=.\Bullet.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\CameraMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\Character.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorContoller.cpp
# End Source File
# Begin Source File

SOURCE=.\Copier.cpp
# End Source File
# Begin Source File

SOURCE=.\Counter.cpp
# End Source File
# Begin Source File

SOURCE=.\Damager.cpp
# End Source File
# Begin Source File

SOURCE=.\Debris.cpp
# End Source File
# Begin Source File

SOURCE=.\DebrisSka.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugEntityStatesDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DestroyableArchitecture.cpp
# End Source File
# Begin Source File

SOURCE=.\DoorController.cpp
# End Source File
# Begin Source File

SOURCE=.\Dummy.cpp
# End Source File
# Begin Source File

SOURCE=.\EffectMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\Effector.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\EnemyBase.cpp
# End Source File
# Begin Source File

SOURCE=.\EnemyMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\EnemySpawner.cpp
# End Source File
# Begin Source File

SOURCE=.\EnvironmentBase.cpp
# End Source File
# Begin Source File

SOURCE=.\EnvironmentMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\EnvironmentParticlesHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\Eruptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FixedCameraField.cpp
# End Source File
# Begin Source File

SOURCE=.\Flame.cpp
# End Source File
# Begin Source File

SOURCE=.\FogMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\GravityMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\GravityRouter.cpp
# End Source File
# Begin Source File

SOURCE=.\HazeMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\HudPicHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\itemObjects.cpp
# End Source File
# Begin Source File

SOURCE=.\items.cpp
# End Source File
# Begin Source File

SOURCE=.\Light.cpp
# End Source File
# Begin Source File

SOURCE=.\Lightning.cpp
# End Source File
# Begin Source File

SOURCE=.\Marker.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageView.cpp
# End Source File
# Begin Source File

SOURCE=.\MirrorMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\Missile.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelDestruction.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelHolder2.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelHolder3.cpp
# End Source File
# Begin Source File

SOURCE=.\MovingBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\MovingBrushMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicChanger.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\NavigationMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\ParticleCloudsHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\ParticleCloudsMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\ParticlesHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\Pendulum.cpp
# End Source File
# Begin Source File

SOURCE=.\Pet.cpp
# End Source File
# Begin Source File

SOURCE=.\PetBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerActionMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerAnimator.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerView.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerWeapons.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerWeaponsEffects.cpp
# End Source File
# Begin Source File

SOURCE=.\PointMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\Projectile.cpp
# End Source File
# Begin Source File

SOURCE=.\Reminder.cpp
# End Source File
# Begin Source File

SOURCE=.\RestrictedField.cpp
# End Source File
# Begin Source File

SOURCE=.\RollingStone.cpp
# End Source File
# Begin Source File

SOURCE=.\Slave.cpp
# End Source File
# Begin Source File

SOURCE=.\SlaveBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\StormController.cpp
# End Source File
# Begin Source File

SOURCE=.\SunMoon.cpp
# End Source File
# Begin Source File

SOURCE=.\Switch.cpp
# End Source File
# Begin Source File

SOURCE=.\TacticsChanger.cpp
# End Source File
# Begin Source File

SOURCE=.\TacticsHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\Teleport.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\TextFXHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeController.cpp
# End Source File
# Begin Source File

SOURCE=.\TouchField.cpp
# End Source File
# Begin Source File

SOURCE=.\Trigger.cpp
# End Source File
# Begin Source File

SOURCE=.\Unit.cpp
# End Source File
# Begin Source File

SOURCE=.\VoiceHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\Watcher.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchPlayers.cpp
# End Source File
# Begin Source File

SOURCE=.\WaterHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\WildPet.cpp
# End Source File
# Begin Source File

SOURCE=.\WildPetBase.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldBase.cpp
# ADD CPP /Yu"StdH.h"
# End Source File
# Begin Source File

SOURCE=.\WorldLink.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldSettingsController.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Group "Common headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Common\CharacterDefinition.h
# End Source File
# Begin Source File

SOURCE=.\Common\Common.h
# End Source File
# Begin Source File

SOURCE=.\Common\EmanatingParticles.h
# End Source File
# Begin Source File

SOURCE=.\Common\ExtensionMath.h
# End Source File
# Begin Source File

SOURCE=.\Common\FixedCameraContainer.h
# End Source File
# Begin Source File

SOURCE=.\Common\Flags.h
# End Source File
# Begin Source File

SOURCE=.\Common\GameInterface.h
# End Source File
# Begin Source File

SOURCE=.\Common\Particles.h
# End Source File
# Begin Source File

SOURCE=.\Common\PathFinding.h
# End Source File
# Begin Source File

SOURCE=.\Common\RestrictedFieldContainer.h
# End Source File
# Begin Source File

SOURCE=.\StdH\StdH.h
# End Source File
# Begin Source File

SOURCE=.\Common\WeaponPositions.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AnimationChanger.h
# End Source File
# Begin Source File

SOURCE=.\AnimationChanger_tables.h
# End Source File
# Begin Source File

SOURCE=.\AnimationHub.h
# End Source File
# Begin Source File

SOURCE=.\AnimationHub_tables.h
# End Source File
# Begin Source File

SOURCE=.\AreaMarker.h
# End Source File
# Begin Source File

SOURCE=.\AreaMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\BackgroundViewer.h
# End Source File
# Begin Source File

SOURCE=.\BackgroundViewer_tables.h
# End Source File
# Begin Source File

SOURCE=.\BasicEffects.h
# End Source File
# Begin Source File

SOURCE=.\BasicEffects_tables.h
# End Source File
# Begin Source File

SOURCE=.\BlendController.h
# End Source File
# Begin Source File

SOURCE=.\BlendController_tables.h
# End Source File
# Begin Source File

SOURCE=.\BloodSpray.h
# End Source File
# Begin Source File

SOURCE=.\Bullet.h
# End Source File
# Begin Source File

SOURCE=.\Bullet_tables.h
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\Camera_tables.h
# End Source File
# Begin Source File

SOURCE=.\CameraMarker.h
# End Source File
# Begin Source File

SOURCE=.\CameraMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\Character.h
# End Source File
# Begin Source File

SOURCE=.\Character_tables.h
# End Source File
# Begin Source File

SOURCE=.\CharacterBase.h
# End Source File
# Begin Source File

SOURCE=.\CharacterBase_tables.h
# End Source File
# Begin Source File

SOURCE=.\ColorContoller.h
# End Source File
# Begin Source File

SOURCE=.\ColorContoller_tables.h
# End Source File
# Begin Source File

SOURCE=.\Copier.h
# End Source File
# Begin Source File

SOURCE=.\Copier_tables.h
# End Source File
# Begin Source File

SOURCE=.\Counter.h
# End Source File
# Begin Source File

SOURCE=.\Counter_tables.h
# End Source File
# Begin Source File

SOURCE=.\Damager.h
# End Source File
# Begin Source File

SOURCE=.\Damager_tables.h
# End Source File
# Begin Source File

SOURCE=.\Debris.h
# End Source File
# Begin Source File

SOURCE=.\Debris_tables.h
# End Source File
# Begin Source File

SOURCE=.\DebrisSka.h
# End Source File
# Begin Source File

SOURCE=.\DebrisSka_tables.h
# End Source File
# Begin Source File

SOURCE=.\DebugEntityStatesDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DebugEntityStatesDisplay_tables.h
# End Source File
# Begin Source File

SOURCE=.\DestroyableArchitecture.h
# End Source File
# Begin Source File

SOURCE=.\DestroyableArchitecture_tables.h
# End Source File
# Begin Source File

SOURCE=.\DoorController.h
# End Source File
# Begin Source File

SOURCE=.\DoorController_tables.h
# End Source File
# Begin Source File

SOURCE=.\Dummy.h
# End Source File
# Begin Source File

SOURCE=.\EffectMarker.h
# End Source File
# Begin Source File

SOURCE=.\EffectMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\Effector.h
# End Source File
# Begin Source File

SOURCE=.\Effector_tables.h
# End Source File
# Begin Source File

SOURCE=.\Enemy.h
# End Source File
# Begin Source File

SOURCE=.\Enemy_tables.h
# End Source File
# Begin Source File

SOURCE=.\EnemyBase.h
# End Source File
# Begin Source File

SOURCE=.\EnemyBase_tables.h
# End Source File
# Begin Source File

SOURCE=.\EnemyHolder.h
# End Source File
# Begin Source File

SOURCE=.\EnemyHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\EnemyMarker.h
# End Source File
# Begin Source File

SOURCE=.\EnemyMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\EnemySpawner.h
# End Source File
# Begin Source File

SOURCE=.\EnemySpawner_tables.h
# End Source File
# Begin Source File

SOURCE=.\EnvironmentBase.h
# End Source File
# Begin Source File

SOURCE=.\EnvironmentBase_tables.h
# End Source File
# Begin Source File

SOURCE=.\EnvironmentMarker.h
# End Source File
# Begin Source File

SOURCE=.\EnvironmentMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\EnvironmentParticlesHolder.h
# End Source File
# Begin Source File

SOURCE=.\EnvironmentParticlesHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\Eruptor.h
# End Source File
# Begin Source File

SOURCE=.\Eruptor_tables.h
# End Source File
# Begin Source File

SOURCE=.\FixedCameraField.h
# End Source File
# Begin Source File

SOURCE=.\FixedCameraField_tables.h
# End Source File
# Begin Source File

SOURCE=.\Flame.h
# End Source File
# Begin Source File

SOURCE=.\Flame_tables.h
# End Source File
# Begin Source File

SOURCE=.\FogMarker.h
# End Source File
# Begin Source File

SOURCE=.\FogMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\Global_tables.h
# End Source File
# Begin Source File

SOURCE=.\GradientMarker.h
# End Source File
# Begin Source File

SOURCE=.\GradientMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\GravityMarker.h
# End Source File
# Begin Source File

SOURCE=.\GravityMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\GravityRouter.h
# End Source File
# Begin Source File

SOURCE=.\GravityRouter_tables.h
# End Source File
# Begin Source File

SOURCE=.\HazeMarker.h
# End Source File
# Begin Source File

SOURCE=.\HazeMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\HudPicHolder.h
# End Source File
# Begin Source File

SOURCE=.\HudPicHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\Item_tables.h
# End Source File
# Begin Source File

SOURCE=.\itemObjects.h
# End Source File
# Begin Source File

SOURCE=.\itemObjects_tables.h
# End Source File
# Begin Source File

SOURCE=.\items.h
# End Source File
# Begin Source File

SOURCE=.\items_tables.h
# End Source File
# Begin Source File

SOURCE=.\Light.h
# End Source File
# Begin Source File

SOURCE=.\Light_tables.h
# End Source File
# Begin Source File

SOURCE=.\Lightning.h
# End Source File
# Begin Source File

SOURCE=.\Lightning_tables.h
# End Source File
# Begin Source File

SOURCE=.\Marker.h
# End Source File
# Begin Source File

SOURCE=.\Marker_tables.h
# End Source File
# Begin Source File

SOURCE=.\MessageHolder.h
# End Source File
# Begin Source File

SOURCE=.\MessageHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\MessageView.h
# End Source File
# Begin Source File

SOURCE=.\MessageView_tables.h
# End Source File
# Begin Source File

SOURCE=.\MirrorMarker.h
# End Source File
# Begin Source File

SOURCE=.\MirrorMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\Missile.h
# End Source File
# Begin Source File

SOURCE=.\Missile_tables.h
# End Source File
# Begin Source File

SOURCE=.\ModelDestruction.h
# End Source File
# Begin Source File

SOURCE=.\ModelDestruction_tables.h
# End Source File
# Begin Source File

SOURCE=.\ModelHolder.h
# End Source File
# Begin Source File

SOURCE=.\ModelHolder2.h
# End Source File
# Begin Source File

SOURCE=.\ModelHolder2_tables.h
# End Source File
# Begin Source File

SOURCE=.\ModelHolder3.h
# End Source File
# Begin Source File

SOURCE=.\ModelHolder3_tables.h
# End Source File
# Begin Source File

SOURCE=.\ModelHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\MovingBrush.h
# End Source File
# Begin Source File

SOURCE=.\MovingBrush_tables.h
# End Source File
# Begin Source File

SOURCE=.\MovingBrushMarker.h
# End Source File
# Begin Source File

SOURCE=.\MovingBrushMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\MusicChanger.h
# End Source File
# Begin Source File

SOURCE=.\MusicChanger_tables.h
# End Source File
# Begin Source File

SOURCE=.\MusicHolder.h
# End Source File
# Begin Source File

SOURCE=.\MusicHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\NavigationMarker.h
# End Source File
# Begin Source File

SOURCE=.\NavigationMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\ParticleCloudsHolder.h
# End Source File
# Begin Source File

SOURCE=.\ParticleCloudsHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\ParticleCloudsMarker.h
# End Source File
# Begin Source File

SOURCE=.\ParticleCloudsMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\ParticlesHolder.h
# End Source File
# Begin Source File

SOURCE=.\ParticlesHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\Pendulum.h
# End Source File
# Begin Source File

SOURCE=.\Pendulum_tables.h
# End Source File
# Begin Source File

SOURCE=.\Pet.h
# End Source File
# Begin Source File

SOURCE=.\Pet_tables.h
# End Source File
# Begin Source File

SOURCE=.\PetBase.h
# End Source File
# Begin Source File

SOURCE=.\PetBase_tables.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\Player_tables.h
# End Source File
# Begin Source File

SOURCE=.\PlayerActionMarker.h
# End Source File
# Begin Source File

SOURCE=.\PlayerActionMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\PlayerAnimator.h
# End Source File
# Begin Source File

SOURCE=.\PlayerAnimator_tables.h
# End Source File
# Begin Source File

SOURCE=.\PlayerMarker.h
# End Source File
# Begin Source File

SOURCE=.\PlayerMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\PlayerView.h
# End Source File
# Begin Source File

SOURCE=.\PlayerView_tables.h
# End Source File
# Begin Source File

SOURCE=.\PlayerWeapons.h
# End Source File
# Begin Source File

SOURCE=.\PlayerWeapons_tables.h
# End Source File
# Begin Source File

SOURCE=.\PlayerWeaponsEffects.h
# End Source File
# Begin Source File

SOURCE=.\PlayerWeaponsEffects_tables.h
# End Source File
# Begin Source File

SOURCE=.\PointMarker.h
# End Source File
# Begin Source File

SOURCE=.\PointMarker_tables.h
# End Source File
# Begin Source File

SOURCE=.\Projectile.h
# End Source File
# Begin Source File

SOURCE=.\Projectile_tables.h
# End Source File
# Begin Source File

SOURCE=.\Reminder.h
# End Source File
# Begin Source File

SOURCE=.\Reminder_tables.h
# End Source File
# Begin Source File

SOURCE=.\RestrictedField.h
# End Source File
# Begin Source File

SOURCE=.\RestrictedField_tables.h
# End Source File
# Begin Source File

SOURCE=.\RollingStone.h
# End Source File
# Begin Source File

SOURCE=.\RollingStone_tables.h
# End Source File
# Begin Source File

SOURCE=.\Slave.h
# End Source File
# Begin Source File

SOURCE=.\Slave_tables.h
# End Source File
# Begin Source File

SOURCE=.\SlaveBase.h
# End Source File
# Begin Source File

SOURCE=.\SlaveBase_tables.h
# End Source File
# Begin Source File

SOURCE=.\SoundHolder.h
# End Source File
# Begin Source File

SOURCE=.\SoundHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\StormController.h
# End Source File
# Begin Source File

SOURCE=.\StormController_tables.h
# End Source File
# Begin Source File

SOURCE=.\SunMoon.h
# End Source File
# Begin Source File

SOURCE=.\SunMoon_tables.h
# End Source File
# Begin Source File

SOURCE=.\Switch.h
# End Source File
# Begin Source File

SOURCE=.\Switch_tables.h
# End Source File
# Begin Source File

SOURCE=.\TacticsChanger.h
# End Source File
# Begin Source File

SOURCE=.\TacticsChanger_tables.h
# End Source File
# Begin Source File

SOURCE=.\TacticsHolder.h
# End Source File
# Begin Source File

SOURCE=.\TacticsHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\Teleport.h
# End Source File
# Begin Source File

SOURCE=.\Teleport_tables.h
# End Source File
# Begin Source File

SOURCE=.\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Terrain_tables.h
# End Source File
# Begin Source File

SOURCE=.\TextFXHolder.h
# End Source File
# Begin Source File

SOURCE=.\TextFXHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\TimeController.h
# End Source File
# Begin Source File

SOURCE=.\TimeController_tables.h
# End Source File
# Begin Source File

SOURCE=.\TouchField.h
# End Source File
# Begin Source File

SOURCE=.\TouchField_tables.h
# End Source File
# Begin Source File

SOURCE=.\Trigger.h
# End Source File
# Begin Source File

SOURCE=.\Trigger_tables.h
# End Source File
# Begin Source File

SOURCE=.\Unit.h
# End Source File
# Begin Source File

SOURCE=.\Unit_tables.h
# End Source File
# Begin Source File

SOURCE=.\VoiceHolder.h
# End Source File
# Begin Source File

SOURCE=.\VoiceHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\Watcher.h
# End Source File
# Begin Source File

SOURCE=.\Watcher_tables.h
# End Source File
# Begin Source File

SOURCE=.\WatchPlayers.h
# End Source File
# Begin Source File

SOURCE=.\WatchPlayers_tables.h
# End Source File
# Begin Source File

SOURCE=.\WaterHolder.h
# End Source File
# Begin Source File

SOURCE=.\WaterHolder_tables.h
# End Source File
# Begin Source File

SOURCE=.\WildPet.h
# End Source File
# Begin Source File

SOURCE=.\WildPet_tables.h
# End Source File
# Begin Source File

SOURCE=.\WildPetBase.h
# End Source File
# Begin Source File

SOURCE=.\WildPetBase_tables.h
# End Source File
# Begin Source File

SOURCE=.\WorldBase.h
# End Source File
# Begin Source File

SOURCE=.\WorldBase_tables.h
# End Source File
# Begin Source File

SOURCE=.\WorldLink.h
# End Source File
# Begin Source File

SOURCE=.\WorldLink_tables.h
# End Source File
# Begin Source File

SOURCE=.\WorldSettingsController.h
# End Source File
# Begin Source File

SOURCE=.\WorldSettingsController_tables.h
# End Source File
# End Group
# Begin Group "Class Files"

# PROP Default_Filter "es"
# Begin Group "AI"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\NavigationMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\NavigationMarker.es
InputName=NavigationMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Reminder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Reminder.es
InputName=Reminder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TacticsChanger.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsChanger.es
InputName=TacticsChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TacticsHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TacticsHolder.es
InputName=TacticsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Watcher.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Watcher.es
InputName=Watcher

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WatchPlayers.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WatchPlayers.es
InputName=WatchPlayers

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Brushes"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\DestroyableArchitecture.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DestroyableArchitecture.es
InputName=DestroyableArchitecture

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MovingBrush.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrush.es
InputName=MovingBrush

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MovingBrushMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MovingBrushMarker.es
InputName=MovingBrushMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pendulum.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Pendulum.es
InputName=Pendulum

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Terrain.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Terrain.es
InputName=Terrain

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WorldBase.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldBase.es
InputName=WorldBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Effects"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\BasicEffects.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BasicEffects.es
InputName=BasicEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BlendController.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BlendController.es
InputName=BlendController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BloodSpray.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BloodSpray.es
InputName=BloodSpray

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Debris.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Debris.es
InputName=Debris

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebrisSka.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebrisSka.es
InputName=DebrisSka

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EffectMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EffectMarker.es
InputName=EffectMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Effector.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Effector.es
InputName=Effector

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Lightning.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Lightning.es
InputName=Lightning

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RollingStone.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\RollingStone.es
InputName=RollingStone

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StormController.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\StormController.es
InputName=StormController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeController.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TimeController.es
InputName=TimeController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WorldSettingsController.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldSettingsController.es
InputName=WorldSettingsController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Enemies"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\Character.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Character.es
InputName=Character

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CharacterBase.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CharacterBase.es
InputName=CharacterBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Enemy.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Enemy.es
InputName=Enemy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnemyBase.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyBase.es
InputName=EnemyBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnemyMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemyMarker.es
InputName=EnemyMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnemySpawner.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnemySpawner.es
InputName=EnemySpawner

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pet.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\Pet.es
InputName=Pet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PetBase.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\PetBase.es
InputName=PetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PointMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PointMarker.es
InputName=PointMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Slave.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\Slave.es
InputName=Slave

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SlaveBase.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\SlaveBase.es
InputName=SlaveBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Unit.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\Unit.es
InputName=Unit

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WildPet.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\WildPet.es
InputName=WildPet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WildPetBase.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\WildPetBase.es
InputName=WildPetBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Environment"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\EnvironmentBase.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentBase.es
InputName=EnvironmentBase

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnvironmentMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentMarker.es
InputName=EnvironmentMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnvironmentParticlesHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\EnvironmentParticlesHolder.es
InputName=EnvironmentParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Items"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\Item.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\Item.es
InputName=Item

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\itemObjects.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\itemObjects.es
InputName=itemObjects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\items.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\items.es
InputName=items

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Players"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\Player.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Player.es
InputName=Player

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlayerActionMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerActionMarker.es
InputName=PlayerActionMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlayerAnimator.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerAnimator.es
InputName=PlayerAnimator

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlayerMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerMarker.es
InputName=PlayerMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlayerView.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerView.es
InputName=PlayerView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlayerWeapons.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeapons.es
InputName=PlayerWeapons

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlayerWeaponsEffects.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\PlayerWeaponsEffects.es
InputName=PlayerWeaponsEffects

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Tools"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\AnimationChanger.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationChanger.es
InputName=AnimationChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AnimationHub.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AnimationHub.es
InputName=AnimationHub

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AreaMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\AreaMarker.es
InputName=AreaMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BackgroundViewer.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\BackgroundViewer.es
InputName=BackgroundViewer

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Camera.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Camera.es
InputName=Camera

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CameraMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\CameraMarker.es
InputName=CameraMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ColorContoller.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ColorContoller.es
InputName=ColorContoller

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Copier.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Copier.es
InputName=Copier

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Counter.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Counter.es
InputName=Counter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Damager.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Damager.es
InputName=Damager

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugEntityStatesDisplay.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DebugEntityStatesDisplay.es
InputName=DebugEntityStatesDisplay

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DoorController.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\DoorController.es
InputName=DoorController

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dummy.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Dummy.es
InputName=Dummy

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Eruptor.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Eruptor.es
InputName=Eruptor

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FixedCameraField.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FixedCameraField.es
InputName=FixedCameraField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FogMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\FogMarker.es
InputName=FogMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GradientMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GradientMarker.es
InputName=GradientMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GravityMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityMarker.es
InputName=GravityMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GravityRouter.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\GravityRouter.es
InputName=GravityRouter

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HazeMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HazeMarker.es
InputName=HazeMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HudPicHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\HudPicHolder.es
InputName=HudPicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Light.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Light.es
InputName=Light

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Marker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Marker.es
InputName=Marker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MessageHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageHolder.es
InputName=MessageHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MessageView.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MessageView.es
InputName=MessageView

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MirrorMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MirrorMarker.es
InputName=MirrorMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Missile.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Missile.es
InputName=Missile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ModelDestruction.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelDestruction.es
InputName=ModelDestruction

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ModelHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder.es
InputName=ModelHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ModelHolder2.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder2.es
InputName=ModelHolder2

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ModelHolder3.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ModelHolder3.es
InputName=ModelHolder3

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MusicChanger.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicChanger.es
InputName=MusicChanger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MusicHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\MusicHolder.es
InputName=MusicHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ParticleCloudsHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsHolder.es
InputName=ParticleCloudsHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ParticleCloudsMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticleCloudsMarker.es
InputName=ParticleCloudsMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ParticlesHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\ParticlesHolder.es
InputName=ParticlesHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RestrictedField.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\RestrictedField.es
InputName=RestrictedField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SoundHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SoundHolder.es
InputName=SoundHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SunMoon.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\SunMoon.es
InputName=SunMoon

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Switch.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Switch.es
InputName=Switch

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Teleport.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Teleport.es
InputName=Teleport

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TextFXHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TextFXHolder.es
InputName=TextFXHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TouchField.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\TouchField.es
InputName=TouchField

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trigger.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Trigger.es
InputName=Trigger

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VoiceHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\VoiceHolder.es
InputName=VoiceHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WaterHolder.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolder.es
InputName=WaterHolder

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WaterHolderMarker.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build
InputPath=.\WaterHolderMarker.es
InputName=WaterHolderMarker

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WorldLink.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\WorldLink.es
InputName=WorldLink

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Weapons"

# PROP Default_Filter "es"
# Begin Source File

SOURCE=.\Bullet.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Bullet.es
InputName=Bullet

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Flame.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Flame.es
InputName=Flame

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Projectile.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Projectile.es
InputName=Projectile

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\Global.es

!IF  "$(CFG)" == "EntitiesMP - Win32 Debug"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 USALIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 RUSLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 BRZLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 MAXLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 KORTEST"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 THAILIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 WORLDEDITOR"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 DebugKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 ReleaseKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 GERLIVEKalydo"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EntitiesMP - Win32 CHNLIVE"

# Begin Custom Build - EntitiesMP/$(InputName).es
InputPath=.\Global.es
InputName=Global

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd .. 
	ecc EntitiesMP/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LastFileID.txt
# End Source File
# End Group
# Begin Group "Links To Package"

# PROP Default_Filter "ecl"
# Begin Source File

SOURCE=..\..\Flesh\Classes\Acid.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\AirWave.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\AmmoItem.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\AmmoPack.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\AnimationChanger.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\AnimationHub.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\ArmorItem.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\BackgroundViewer.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\BasicEffect.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Beast.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\BigHead.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\BlendController.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\BloodSpray.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Boneman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Bouncer.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Bullet.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Camera.ecl
# End Source File
# Begin Source File

SOURCE=..\..\FLESH\Classes\CameraMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\CannonBall.ecl
# End Source File
# Begin Source File

SOURCE=..\..\flesh\Classes\CannonStatic.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Catman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\flesh\Classes\ChainSawFreak.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Copier.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Counter.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\CrateRider.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Cyborg.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\CyborgBike.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Damager.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Debris.ecl
# End Source File
# Begin Source File

SOURCE=..\..\flesh\Classes\Demon.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\DestroyableArchitecture.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Devil.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\DevilMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\DevilProjectile.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\DoorController.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Dragonman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EffectMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Effector.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Elemental.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnemyBase.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnemyCounter.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnemyDive.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnemyFly.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnemyMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnemyRunInto.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnemySpawner.ecl
# End Source File
# Begin Source File

SOURCE=..\..\flesh\Classes\EntityStateDisplay.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnvironmentBase.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\EnvironmentMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Eruptor.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Eyeman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Fish.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Fishman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Flame.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\FogMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\GhostBusterRay.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Gizmo.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\GradientMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\GravityMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\GravityRouter.ecl
# End Source File
# Begin Source File

SOURCE=..\..\flesh\Classes\Grunt.ecl
# End Source File
# Begin Source File

SOURCE=..\..\flesh\Classes\Guffy.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\HazeMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Headman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\HealthItem.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Huanman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Item.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\KeyItem.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Light.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Lightning.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Mamut.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Mamutman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Mantaman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Marker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\MessageHolder.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\MessageItem.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\MirrorMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\ModelDestruction.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\ModelHolder.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\ModelHolder2.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\MovingBrush.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\MovingBrushMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\MusicChanger.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\MusicHolder.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\NavigationMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\ParticlesHolder.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Pendulum.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Pipebomb.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Player.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\PlayerActionMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\PlayerAnimator.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\PlayerMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\PlayerView.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\PlayerWeapons.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\PlayerWeaponsEffects.ecl
# End Source File
# Begin Source File

SOURCE=..\..\FLESH\Classes\PowerUpItem.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Projectile.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\PyramidSpaceShip.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\PyramidSpaceShipMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Reminder.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\RobotDriving.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\RobotFixed.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\RobotFlying.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\RollingStone.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Santa.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Scorpman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Ship.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\ShipMarker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\flesh\Classes\Shooter.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\SoundHolder.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Switch.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Teleport.ecl
# End Source File
# Begin Source File

SOURCE="..\..\..\..\games\ss-se\classes\TestPlayer.ecl"
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\TouchField.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Trigger.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Twister.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\VoiceHolder.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Walker.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Watcher.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\WatchPlayers.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Water.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\WeaponItem.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Werebull.ecl
# End Source File
# Begin Source File

SOURCE=..\..\LCplay\LC_Kor\Classes\WildPet.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\Woman.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\WorldBase.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\WorldLink.ecl
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Classes\WorldSettingsController.ecl
# End Source File
# End Group
# Begin Group "Messages"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\BeastBig.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\BeastNormal.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Bomberman.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Boneman.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Bull.txt
# End Source File
# Begin Source File

SOURCE=..\..\flesh\DataMP\Messages\Enemies\ChainsawFreak.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Devil.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\ElementalLava.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\EyemanGreen.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\EyemanPurple.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Firecracker.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Fish.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Gizmo.txt
# End Source File
# Begin Source File

SOURCE=..\..\flesh\DataMP\Messages\Enemies\GruntCommander.txt
# End Source File
# Begin Source File

SOURCE=..\..\flesh\DataMP\Messages\Enemies\GruntSoldier.txt
# End Source File
# Begin Source File

SOURCE=..\..\flesh\DataMP\Messages\Enemies\Guffy.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Kamikaze.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Rocketman.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\ScorpmanGeneral.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\ScorpmanSoldier.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\WalkerBig.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\WalkerSmall.txt
# End Source File
# Begin Source File

SOURCE=..\..\Flesh\Data\Messages\Enemies\Woman.txt
# End Source File
# End Group
# End Target
# End Project
