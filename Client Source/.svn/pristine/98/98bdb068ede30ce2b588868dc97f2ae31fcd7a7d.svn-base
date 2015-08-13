# Microsoft Developer Studio Project File - Name="Modeler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Modeler - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Modeler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Modeler.mak" CFG="Modeler - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Modeler - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Modeler - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Modeler - Win32 WORLDEDITOR" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Modeler - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 htmlhelp.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /out:"Release/SeriousModeler.exe"
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\Release\SeriousModeler.exe
InputName=SeriousModeler
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Modeler - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 htmlhelp.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/SeriousModeler.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\Debug\SeriousModeler.exe
InputName=SeriousModeler
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).exe $(ENGINE_DIR)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Modeler - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WORLDEDITOR"
# PROP BASE Intermediate_Dir "WORLDEDITOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WORLDEDITOR"
# PROP Intermediate_Dir "WORLDEDITOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 htmlhelp.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /out:"Release/SeriousModeler.exe"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 htmlhelp.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /out:"WORLDEDITOR/SeriousModeler.exe"
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\WORLDEDITOR\SeriousModeler.exe
InputName=SeriousModeler
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Modeler - Win32 Release"
# Name "Modeler - Win32 Debug"
# Name "Modeler - Win32 WORLDEDITOR"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\AnimComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChoosedColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ColoredButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlEditBoolean.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoMipModeling.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChooseAnim.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateReflectionTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateSpecularTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExportForSkinning.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgAnim.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgMip.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgNone.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgPos.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgRendering.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMarkLinkedSurfaces.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMultiplyMapping.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNumericAlpha.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPgCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPgInfoAttachingPlacement.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPgInfoAttachingSound.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPleaseWait.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPreferences.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkedSurfaceList.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Modeler.cpp
# End Source File
# Begin Source File

SOURCE=.\Modeler.rc
# End Source File
# Begin Source File

SOURCE=.\ModelerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelerView.cpp
# End Source File
# Begin Source File

SOURCE=.\PaletteButton.cpp
# End Source File
# Begin Source File

SOURCE=.\PaletteDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchPalette.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchPaletteButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ScriptDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptView.cpp
# End Source File
# Begin Source File

SOURCE=.\SkyFloat.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=.\StainsComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextureComboBox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\AnimComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChoosedColorButton.h
# End Source File
# Begin Source File

SOURCE=.\ColoredButton.h
# End Source File
# Begin Source File

SOURCE=.\CtrlEditBoolean.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoMipModeling.h
# End Source File
# Begin Source File

SOURCE=.\DlgChooseAnim.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateReflectionTexture.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateSpecularTexture.h
# End Source File
# Begin Source File

SOURCE=.\DlgExportForSkinning.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoFrame.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgAnim.h
# End Source File
# Begin Source File

SOURCE=.\dlginfopgglobal.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgMip.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgNone.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgPos.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoPgRendering.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoSheet.h
# End Source File
# Begin Source File

SOURCE=.\DlgMarkLinkedSurfaces.h
# End Source File
# Begin Source File

SOURCE=.\DlgMultiplyMapping.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewProgress.h
# End Source File
# Begin Source File

SOURCE=.\DlgNumericAlpha.h
# End Source File
# Begin Source File

SOURCE=.\DlgPgCollision.h
# End Source File
# Begin Source File

SOURCE=.\DlgPgInfoAttachingPlacement.h
# End Source File
# Begin Source File

SOURCE=.\DlgPgInfoAttachingSound.h
# End Source File
# Begin Source File

SOURCE=.\DlgPleaseWait.h
# End Source File
# Begin Source File

SOURCE=.\DlgPreferences.h
# End Source File
# Begin Source File

SOURCE=.\LinkedSurfaceList.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Modeler.h
# End Source File
# Begin Source File

SOURCE=.\ModelerDoc.h
# End Source File
# Begin Source File

SOURCE=.\ModelerView.h
# End Source File
# Begin Source File

SOURCE=.\PaletteButton.h
# End Source File
# Begin Source File

SOURCE=.\PaletteDialog.h
# End Source File
# Begin Source File

SOURCE=.\PatchPalette.h
# End Source File
# Begin Source File

SOURCE=.\PatchPaletteButton.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScriptDoc.h
# End Source File
# Begin Source File

SOURCE=.\ScriptView.h
# End Source File
# Begin Source File

SOURCE=.\SkyFloat.h
# End Source File
# Begin Source File

SOURCE=.\StainsComboBox.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextureComboBox.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\animcont.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_scri.ico
# End Source File
# Begin Source File

SOURCE=.\res\mipandli.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Modeler.ico
# End Source File
# Begin Source File

SOURCE=.\res\Modeler.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ModelerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\nullcurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\pick.ico
# End Source File
# Begin Source File

SOURCE=.\res\renderco.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stain_ac.ico
# End Source File
# Begin Source File

SOURCE=.\res\stain_in.ico
# End Source File
# Begin Source File

SOURCE=.\res\stainsco.bmp
# End Source File
# Begin Source File

SOURCE=.\res\texturec.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AppExit.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw4.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCopy.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCut.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditUndo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FilePrnt.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileSave.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\MakeHelp.bat
# End Source File
# Begin Source File

SOURCE=.\hlp\Modeler.cnt
# End Source File
# Begin Source File

SOURCE=.\hlp\Modeler.hm
# End Source File
# Begin Source File

SOURCE=.\hlp\Modeler.hpj
# End Source File
# Begin Source File

SOURCE=.\hlp\Modeler.ph
# End Source File
# Begin Source File

SOURCE=.\hlp\RecFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecLast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecNext.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmax.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# End Group
# Begin Group "Wizard Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Modeler.clw
# End Source File
# End Group
# End Target
# End Project
