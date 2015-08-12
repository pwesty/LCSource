# Microsoft Developer Studio Project File - Name="Nksp_Kalydo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Nksp_Kalydo - Win32 DebugKorKalydo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Nksp_Kalydo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Nksp_Kalydo.mak" CFG="Nksp_Kalydo - Win32 DebugKorKalydo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Nksp_Kalydo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Nksp_Kalydo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Nksp_Kalydo - Win32 GERLIVE" (based on "Win32 (x86) Application")
!MESSAGE "Nksp_Kalydo - Win32 KORTEST" (based on "Win32 (x86) Application")
!MESSAGE "Nksp_Kalydo - Win32 WORLDEDITOR" (based on "Win32 (x86) Application")
!MESSAGE "Nksp_Kalydo - Win32 DebugKalydo" (based on "Win32 (x86) Application")
!MESSAGE "Nksp_Kalydo - Win32 ReleaseKalydo" (based on "Win32 (x86) Application")
!MESSAGE "Nksp_Kalydo - Win32 GERLIVEKalydo" (based on "Win32 (x86) Application")
!MESSAGE "Nksp_Kalydo - Win32 DebugKorKalydo" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Nksp_Kalydo - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib F_SocketR.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\Release\Nksp_Kalydo.exe
InputName=Nksp_Kalydo
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp_Kalydo - Win32 Debug"

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
# ADD LINK32 Winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib" /nodefaultlib:"gamempd.lib" /pdbtype:sept /libpath:"tinyxml" /libpath:"..\lib" -verbose:lib
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\_Debug\Nksp_Kalydo.exe
InputName=Nksp_Kalydo
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _Debug\$(InputName).exe $(ENGINE_DIR)\Bin\Debug >nul 
	copy _Debug\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy _Debug\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build
# Begin Special Build Tool
OutDir=.\_Debug
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir                                 $(OutDir)\..\..\DebugFilesForCopy                                	copy                                 $(OutDir)\Nksp.exe                                 $(OutDir)\..\..\DebugFilesForCopy\                                	copy                                 $(OutDir)\Nksp.map                                 $(OutDir)\..\..\DebugFilesForCopy\                                	copy                                 $(OutDir)\Nksp.pdb                                 $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Nksp_Kalydo - Win32 GERLIVE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp_Kalydo___Win32_GERLIVE"
# PROP BASE Intermediate_Dir "Nksp_Kalydo___Win32_GERLIVE"
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
InputPath=.\_GERLIVE\Nksp_Kalydo.exe
InputName=Nksp_Kalydo
SOURCE="$(InputPath)"

"$(GER_LIVE)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVE\$(InputName).exe $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).map $(GER_LIVE)\Bin >nul 
	copy _GERLIVE\$(InputName).pdb $(GER_LIVE)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp_Kalydo - Win32 KORTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp_Kalydo___Win32_KORTEST"
# PROP BASE Intermediate_Dir "Nksp_Kalydo___Win32_KORTEST"
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# Begin Custom Build - Copying $(InputName) binaries to $(KOR_TEST)\Bin
InputPath=.\_KORTEST\Nksp_Kalydo.exe
InputName=Nksp_Kalydo
SOURCE="$(InputPath)"

"$(KOR_TEST)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _KORTEST\$(InputName).exe $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).map $(KOR_TEST)\Bin >nul 
	copy _KORTEST\$(InputName).pdb $(KOR_TEST)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp_Kalydo - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp_Kalydo___Win32_WORLDEDITOR"
# PROP BASE Intermediate_Dir "Nksp_Kalydo___Win32_WORLDEDITOR"
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
InputPath=.\WORLDEDITOR\Nksp_Kalydo.exe
InputName=Nksp_Kalydo
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy WORLDEDITOR\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp_Kalydo - Win32 DebugKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Nksp_Kalydo___Win32_DebugKalydo"
# PROP BASE Intermediate_Dir "Nksp_Kalydo___Win32_DebugKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugKalydo"
# PROP Intermediate_Dir "DebugKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /Fr /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /I "..\include" /D "_USRDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "G_KOR" /D "KALYDO" /Fr /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Winmm.lib ws2_32.lib F_SocketD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib" /nodefaultlib:"gamempd.lib" /pdbtype:sept /libpath:"tinyxml" /libpath:"..\lib" -verbose:lib
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Winmm.lib ws2_32.lib F_SocketD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib KRFReadLibDebug.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib" /nodefaultlib:"gamempd.lib" /out:"DebugKalydo/Nksp.exe" /pdbtype:sept /libpath:"tinyxml" /libpath:"..\lib" /libpath:"..\Kalydo\KRFReadLib\Bin" -verbose:lib
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\DebugKalydo\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy DebugKalydo\$(InputName).exe $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy DebugKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build
# Begin Special Build Tool
OutDir=.\DebugKalydo
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir                                 $(OutDir)\..\..\DebugFilesForCopy                                	copy                                 $(OutDir)\Nksp.exe                                 $(OutDir)\..\..\DebugFilesForCopy\                                	copy                                 $(OutDir)\Nksp.map                                 $(OutDir)\..\..\DebugFilesForCopy\                                	copy                                 $(OutDir)\Nksp.pdb                                 $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Nksp_Kalydo - Win32 ReleaseKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp_Kalydo___Win32_ReleaseKalydo"
# PROP BASE Intermediate_Dir "Nksp_Kalydo___Win32_ReleaseKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseKalydo"
# PROP Intermediate_Dir "ReleaseKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "KALYDO" /D "G_KOR" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib F_SocketR.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /libpath:"tinyxml" /libpath:"..\lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib F_SocketR.lib KRFReadLib.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /out:"ReleaseKalydo/Nksp.exe" /libpath:"tinyxml" /libpath:"..\lib" /libpath:"..\Kalydo\KRFReadLib\Bin"
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\ReleaseKalydo\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ReleaseKalydo\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy ReleaseKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp_Kalydo - Win32 GERLIVEKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp_Kalydo___Win32_GERLIVEKalydo"
# PROP BASE Intermediate_Dir "Nksp_Kalydo___Win32_GERLIVEKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "_GERLIVEKalydo"
# PROP Intermediate_Dir "_GERLIVEKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /D "KALYDO" /FR /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /I "..\include" /D "_WINDLL" /D "_AFXDLL" /D "_CLIENT_" /D "_USE_32BIT_TIME_T" /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "G_GERMAN" /D "KALYDO" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib F_SocketR.lib KRFReadLib.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /out:"ReleaseKalydo/Nksp.exe" /libpath:"tinyxml" /libpath:"..\lib" /libpath:".\Compact"
# ADD LINK32 winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib KRFReadLib.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib" /out:"_GERLIVEKalydo/Nksp.exe" /libpath:"tinyxml" /libpath:"..\lib" /libpath:"..\Kalydo\KRFReadLib\Bin"
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\_GERLIVEKalydo\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(GER_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy _GERLIVEKalydo\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy _GERLIVEKalydo\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp_Kalydo - Win32 DebugKorKalydo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "_DebugKorKalydo"
# PROP BASE Intermediate_Dir "_DebugKorKalydo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "_DebugKorKalydo"
# PROP Intermediate_Dir "_DebugKorKalydo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "G_KOR" /D "KALYDO" /Fr /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\\" /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "G_KOR" /D "VER_TEST" /D "KALYDO" /Fr /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Engined.dll Winmm.lib ws2_32.lib F_SocketD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib KRFReadLibDebug.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib" /nodefaultlib:"gamempd.lib" /out:"DebugKalydo/Nksp.exe" /pdbtype:sept /libpath:"tinyxml" /libpath:"..\lib" /libpath:"..\Kalydo\KRFReadLib\Bin" -verbose:lib
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Engined.lib Winmm.lib ws2_32.lib F_SocketD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib KRFReadLibDebug.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib" /nodefaultlib:"gamempd.lib" /out:"_DebugKORKalydo/Nksp.exe" /pdbtype:sept /libpath:"tinyxml" /libpath:"..\lib" /libpath:"..\Kalydo\KRFReadLib\Bin" /libpath:"..\DebugFilesForCopy" -verbose:lib
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
OutDir=.\_DebugKorKalydo
InputPath=.\_DebugKORKalydo\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(OutDir)\$(InputName).exe $(ENGINE_DIR)\Bin\Debug >nul 
	copy $(OutDir)\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy $(OutDir)\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build
# Begin Special Build Tool
OutDir=.\_DebugKorKalydo
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir                                 $(OutDir)\..\..\DebugFilesForCopy                                	copy                                 $(OutDir)\Nksp.exe                                 $(OutDir)\..\..\DebugFilesForCopy\                                	copy                                 $(OutDir)\Nksp.map                                 $(OutDir)\..\..\DebugFilesForCopy\                                	copy                                 $(OutDir)\Nksp.pdb                                 $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Nksp_Kalydo - Win32 Release"
# Name "Nksp_Kalydo - Win32 Debug"
# Name "Nksp_Kalydo - Win32 GERLIVE"
# Name "Nksp_Kalydo - Win32 KORTEST"
# Name "Nksp_Kalydo - Win32 WORLDEDITOR"
# Name "Nksp_Kalydo - Win32 DebugKalydo"
# Name "Nksp_Kalydo - Win32 ReleaseKalydo"
# Name "Nksp_Kalydo - Win32 GERLIVEKalydo"
# Name "Nksp_Kalydo - Win32 DebugKorKalydo"
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
# Begin Group "Kalydo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Social\Defines.h
# End Source File
# Begin Source File

SOURCE=.\Social\Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\Social\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Social\KalydoExports.cpp
# End Source File
# Begin Source File

SOURCE=.\Social\KalydoExports.h
# End Source File
# Begin Source File

SOURCE=..\Kalydo\KRFReadLib\Include\kifstream.h
# End Source File
# Begin Source File

SOURCE=..\Kalydo\KRFReadLib\Include\kifstream.inl
# End Source File
# Begin Source File

SOURCE=..\Kalydo\KRFReadLib\Include\KRFDefines.h
# End Source File
# Begin Source File

SOURCE=..\Kalydo\KRFReadLib\Include\KRFReadLib.h
# End Source File
# Begin Source File

SOURCE=..\Kalydo\KRFReadLib\Include\kseek.h
# End Source File
# Begin Source File

SOURCE=..\Kalydo\KRFReadLib\Include\kseek.inl
# End Source File
# Begin Source File

SOURCE=.\Social\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Social\PlayerInterfaceDef.h
# End Source File
# Begin Source File

SOURCE=.\Social\resource.h
# End Source File
# Begin Source File

SOURCE=.\Social\TApplication.cpp
# End Source File
# Begin Source File

SOURCE=.\Social\TApplication.h
# End Source File
# Begin Source File

SOURCE=.\Social\TException.cpp
# End Source File
# Begin Source File

SOURCE=.\Social\TException.h
# End Source File
# Begin Source File

SOURCE=.\Social\THandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Social\THandler.h
# End Source File
# Begin Source File

SOURCE=.\Social\TLastChaosApplication.cpp
# End Source File
# Begin Source File

SOURCE=.\Social\TLastChaosApplication.h
# End Source File
# Begin Source File

SOURCE=.\Social\TPlayerInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Social\TPlayerInterface.h
# End Source File
# Begin Source File

SOURCE=.\Social\TRequestId.h
# End Source File
# End Group
# End Target
# End Project
