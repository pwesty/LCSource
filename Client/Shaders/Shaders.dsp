# Microsoft Developer Studio Project File - Name="Shaders" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Shaders - Win32 DebugKalydo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Shaders.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Shaders.mak" CFG="Shaders - Win32 DebugKalydo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Shaders - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 USALIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 GERLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 RUSLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 BRZLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 MAXLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 KORTEST" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 THAILIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 WORLDEDITOR" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 DebugKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 ReleaseKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 GERLIVEKalydo" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 CHNLIVE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Shaders - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"_Debug/ShadersD.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\_Debug\ShadersD.dll
InputName=ShadersD
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
PostBuild_Cmds=mkdir              $(OutDir)\..\..\DebugFilesForCopy             	copy              $(OutDir)\ShadersD.dll              $(OutDir)\..\..\DebugFilesForCopy\             	copy              $(OutDir)\ShadersD.lib              $(OutDir)\..\..\DebugFilesForCopy\             	copy              $(OutDir)\ShadersD.map              $(OutDir)\..\..\DebugFilesForCopy\             	copy              $(OutDir)\ShadersD.pdb              $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Shaders - Win32 USALIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_USALIVE"
# PROP BASE Intermediate_Dir "Shaders___Win32_USALIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_USALIVE"
# PROP Intermediate_Dir "_USALIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_USA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(USA_LIVE)\Bin
InputPath=.\_USALIVE\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(USA_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _USALIVE\$(InputName).dll $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).map $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).lib $(USA_LIVE)\Bin >nul 
	copy _USALIVE\$(InputName).pdb $(USA_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 GERLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_GERLIVE"
# PROP BASE Intermediate_Dir "Shaders___Win32_GERLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVE"
# PROP Intermediate_Dir "_GERLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_GERMAN" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(GER_LIVE)\Bin
InputPath=.\_GERLIVE\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(GER_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVE\$(InputName).dll $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).map $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).lib $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).pdb $(GER_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 RUSLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_RUSLIVE"
# PROP BASE Intermediate_Dir "Shaders___Win32_RUSLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_RUSLIVE"
# PROP Intermediate_Dir "_RUSLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_RUSSIA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(RUS_LIVE)\Bin
InputPath=.\_RUSLIVE\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(RUS_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _RUSLIVE\$(InputName).dll $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).map $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).lib $(RUS_LIVE)\Bin >nul 
	copy _RUSLIVE\$(InputName).pdb $(RUS_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 BRZLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_BRZLIVE"
# PROP BASE Intermediate_Dir "Shaders___Win32_BRZLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_BRZLIVE"
# PROP Intermediate_Dir "_BRZLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_BRAZIL" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(BRZ_LIVE)\Bin
InputPath=.\_BRZLIVE\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(BRZ_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _BRZLIVE\$(InputName).dll $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).map $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).lib $(BRZ_LIVE)\Bin >nul 
	copy _BRZLIVE\$(InputName).pdb $(BRZ_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 MAXLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_MAXLIVE"
# PROP BASE Intermediate_Dir "Shaders___Win32_MAXLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_MAXLIVE"
# PROP Intermediate_Dir "_MAXLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_MAXICO" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(MAX_LIVE)\Bin
InputPath=.\_MAXLIVE\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(MAX_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _MAXLIVE\$(InputName).dll $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).map $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).lib $(MAX_LIVE)\Bin >nul 
	copy _MAXLIVE\$(InputName).pdb $(MAX_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 KORTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_KORTEST"
# PROP BASE Intermediate_Dir "Shaders___Win32_KORTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_KORTEST"
# PROP Intermediate_Dir "_KORTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(KOR_TEST)\Bin
InputPath=.\_KORTEST\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(KOR_TEST)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _KORTEST\$(InputName).dll $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).map $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).lib $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).pdb $(KOR_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 THAILIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_THAILIVE"
# PROP BASE Intermediate_Dir "Shaders___Win32_THAILIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_THAILIVE"
# PROP Intermediate_Dir "_THAILIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_KOR" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "SHADERS_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_THAI" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(THAI_LIVE)\Bin
InputPath=.\_THAILIVE\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(THAI_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _THAILIVE\$(InputName).dll $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).map $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).lib $(THAI_LIVE)\Bin >nul 
	copy _THAILIVE\$(InputName).pdb $(THAI_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_WORLDEDITOR"
# PROP BASE Intermediate_Dir "Shaders___Win32_WORLDEDITOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WORLDEDITOR"
# PROP Intermediate_Dir "WORLDEDITOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_KOR" /D "VER_TEST" /D "_CLIENT_" /D "WORLD_EDITOR" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /O<none> /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\WORLDEDITOR\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\WorldEditor\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).dll $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).lib $(ENGINE_DIR)\Bin\WorldEditor >nul 
	copy WORLDEDITOR\$(InputName).pdb $(ENGINE_DIR)\Bin\WorldEditor >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 DebugKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Shaders___Win32_DebugKalydo"
# PROP BASE Intermediate_Dir "Shaders___Win32_DebugKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugKalydo"
# PROP Intermediate_Dir "DebugKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_KOR" /D "VER_TEST" /Yu"StdH.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_GERMAN" /D "KALYDO" /D "DELAY_LOADING" /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/ShadersD.dll" /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"DebugKalydo/ShadersD.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\DebugKalydo\ShadersD.dll
InputName=ShadersD
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy DebugKalydo\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).lib $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 ReleaseKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_ReleaseKalydo"
# PROP BASE Intermediate_Dir "Shaders___Win32_ReleaseKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseKalydo"
# PROP Intermediate_Dir "ReleaseKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_GERMAN" /D "KALYDO" /D "DELAY_LOADING" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\ReleaseKalydo\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ReleaseKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 GERLIVEKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_GERLIVEKalydo"
# PROP BASE Intermediate_Dir "Shaders___Win32_GERLIVEKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVEKalydo"
# PROP Intermediate_Dir "_GERLIVEKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_GERMAN" /D "KALYDO" /D "DELAY_LOADING" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_GERMAN" /D "KALYDO" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\_GERLIVEKalydo\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVEKalydo\$(InputName).dll $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).lib $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 CHNLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_CHNLIVE"
# PROP BASE Intermediate_Dir "Shaders___Win32_CHNLIVE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_CHNLIVE"
# PROP Intermediate_Dir "_CHNLIVE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /D "G_USA" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "_CLIENT_" /D "SHADERS_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_USA" /D "G_CHINA" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(CHN_LIVE)\Bin
InputPath=.\_CHNLIVE\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(CHN_LIVE)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _CHNLIVE\$(InputName).dll $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).map $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).lib $(CHN_LIVE)\Bin >nul 
	copy _CHNLIVE\$(InputName).pdb $(CHN_LIVE)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Shaders - Win32 Debug"
# Name "Shaders - Win32 USALIVE"
# Name "Shaders - Win32 GERLIVE"
# Name "Shaders - Win32 RUSLIVE"
# Name "Shaders - Win32 BRZLIVE"
# Name "Shaders - Win32 MAXLIVE"
# Name "Shaders - Win32 KORTEST"
# Name "Shaders - Win32 THAILIVE"
# Name "Shaders - Win32 WORLDEDITOR"
# Name "Shaders - Win32 DebugKalydo"
# Name "Shaders - Win32 ReleaseKalydo"
# Name "Shaders - Win32 GERLIVEKalydo"
# Name "Shaders - Win32 CHNLIVE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddShader.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseShader.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseShaderPPL_Abs.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTransparent.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorShader.cpp
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultShader.cpp
# End Source File
# Begin Source File

SOURCE=.\Detail_SpecularShader.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailShader.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplaceShader.cpp
# End Source File
# Begin Source File

SOURCE=.\GlassShader.cpp
# End Source File
# Begin Source File

SOURCE=.\InvisibleShader.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiLayerShader.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalMapShader.cpp
# End Source File
# Begin Source File

SOURCE=.\NoShadowShader.cpp
# End Source File
# Begin Source File

SOURCE=.\Reflection.cpp
# End Source File
# Begin Source File

SOURCE=.\ReftectionAndSpecular.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadowShader.cpp
# End Source File
# Begin Source File

SOURCE=.\Specular.cpp
# End Source File
# Begin Source File

SOURCE=.\StdH.cpp
# ADD CPP /Yc"StdH.h"
# End Source File
# Begin Source File

SOURCE=.\Translucent.cpp
# End Source File
# Begin Source File

SOURCE=.\WaterShader.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\StdH.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
