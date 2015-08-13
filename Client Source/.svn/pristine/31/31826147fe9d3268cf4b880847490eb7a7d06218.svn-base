# Microsoft Developer Studio Project File - Name="SeriousSkaStudio" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SeriousSkaStudio - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SeriousSkaStudio.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SeriousSkaStudio.mak" CFG="SeriousSkaStudio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SeriousSkaStudio - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SeriousSkaStudio - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SeriousSkaStudio - Win32 WORLDEDITOR" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SeriousSkaStudio - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "G_KOR" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\Release\SeriousSkaStudio.exe
InputName=SeriousSkaStudio
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\Debug\SeriousSkaStudio.exe
InputName=SeriousSkaStudio
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).exe $(ENGINE_DIR)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	copy Debug\$(InputName).pdb $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 WORLDEDITOR"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SeriousSkaStudio___Win32_WORLDEDITOR"
# PROP BASE Intermediate_Dir "SeriousSkaStudio___Win32_WORLDEDITOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WORLDEDITOR"
# PROP Intermediate_Dir "WORLDEDITOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "G_KOR" /Fr /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "G_KOR" /Fr /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /O<none>
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /profile /map /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\WORLDEDITOR\SeriousSkaStudio.exe
InputName=SeriousSkaStudio
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy WORLDEDITOR\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy WORLDEDITOR\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	copy WORLDEDITOR\$(InputName).pdb $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "SeriousSkaStudio - Win32 Release"
# Name "SeriousSkaStudio - Win32 Debug"
# Name "SeriousSkaStudio - Win32 WORLDEDITOR"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Effect GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgAutosettingFeatherFall.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCCameraEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCCone.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCEffectControlProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCEffectGroupProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCEntityEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCForce.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCLightEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCMdlEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCOrbitEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPADefaultProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPAProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCParticleEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCParticleGroupProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPASphereProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPEConeProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPECylinderProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPEProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPESphereProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPPControlProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPPDSOffsetPosAutoSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPPDynamicStateProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPPForceProcessProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPPPointGoalProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPPProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPPVelocityProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCShockWaveEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSkaEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSoundEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSplineBillboardEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSplinePathEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCTerrainEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCTraceEffectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEffectEditType.cpp
# End Source File
# End Group
# Begin Group "Original Ska"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ColoredButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgClient.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\DropDown.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MDIClientWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SeriousSkaStudio.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\SeriousSkaStudio.hpj

!IF  "$(CFG)" == "SeriousSkaStudio - Win32 Release"

# PROP Exclude_From_Build 1
# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 Debug"

# PROP Exclude_From_Build 1
# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 WORLDEDITOR"

# PROP BASE Exclude_From_Build 1
# PROP BASE Ignore_Default_Tool 1
# PROP Exclude_From_Build 1
# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SeriousSkaStudioDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SeriousSkaStudioView.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Stripper.cpp
# End Source File
# Begin Source File

SOURCE=.\TextBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TexView.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgExtensionEdit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Effect GUI Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgAutosettingFeatherFall.h
# End Source File
# Begin Source File

SOURCE=.\DlgCCameraEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCCone.h
# End Source File
# Begin Source File

SOURCE=.\DlgCCylinder.h
# End Source File
# Begin Source File

SOURCE=.\DlgCEffectControlProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCEffectGroupProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCEntityEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCForce.h
# End Source File
# Begin Source File

SOURCE=.\DlgCLightEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCMdlEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCOrbitEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPADefaultProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPAProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCParticleEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCParticleGroupProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPASphereProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPEConeProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPECylinderProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPEProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPESphereProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPPControlProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPPDSOffsetPosAutoSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPPDynamicStateProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPPForceProcessProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPPPointGoalProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPPProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPPVelocityProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCShockWaveEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSkaEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSoundEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSphere.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSplineBillboardEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSplinePathEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCTerrainEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgCTraceEffectProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgEffectEditType.h
# End Source File
# End Group
# Begin Group "Original Ska Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CheckBox.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ColoredButton.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarTreeView.h
# End Source File
# Begin Source File

SOURCE=.\DlgClient.h
# End Source File
# Begin Source File

SOURCE=.\DlgProperties.h
# End Source File
# Begin Source File

SOURCE=.\DlgTemplate.h
# End Source File
# Begin Source File

SOURCE=.\DropDown.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MDIClientWnd.h
# End Source File
# Begin Source File

SOURCE=.\ModelTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ParsingSymbols.h
# End Source File
# Begin Source File

SOURCE=.\SeriousSkaStudio.h
# End Source File
# Begin Source File

SOURCE=.\SeriousSkaStudioDoc.h
# End Source File
# Begin Source File

SOURCE=.\SeriousSkaStudioView.h
# End Source File
# Begin Source File

SOURCE=.\SplitterFrame.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextBox.h
# End Source File
# Begin Source File

SOURCE=.\TexView.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgExtensionEdit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Icons.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "SeriousSkaStudio - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=SeriousSkaStudio
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=SeriousSkaStudio
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 WORLDEDITOR"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=SeriousSkaStudio
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\SeriousSkaStudio.ico
# End Source File
# Begin Source File

SOURCE=.\SeriousSkaStudio.rc
# End Source File
# Begin Source File

SOURCE=.\res\SeriousSkaStudio.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SeriousSkaStudioDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
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
# Begin Source File

SOURCE=.\hlp\SeriousSkaStudio.cnt

!IF  "$(CFG)" == "SeriousSkaStudio - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Release
InputPath=.\hlp\SeriousSkaStudio.cnt
InputName=SeriousSkaStudio

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Debug
InputPath=.\hlp\SeriousSkaStudio.cnt
InputName=SeriousSkaStudio

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 WORLDEDITOR"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\WORLDEDITOR
InputPath=.\hlp\SeriousSkaStudio.cnt
InputName=SeriousSkaStudio

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Grammar Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Parser.y

!IF  "$(CFG)" == "SeriousSkaStudio - Win32 Release"

# Begin Custom Build - Creating parser source
InputDir=.
InputPath=.\Parser.y

BuildCmds= \
	bison -o$(InputDir)/Parser.c $(InputDir)/Parser.y -d \
	copy $(InputDir)\Parser.c $(InputDir)\Parser.cpp \
	del $(InputDir)\Parser.c /q \
	

"$(InputDir)/Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/Parser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 Debug"

# Begin Custom Build - Creating parser source
InputDir=.
InputPath=.\Parser.y

BuildCmds= \
	bison -o$(InputDir)/Parser.c $(InputDir)/Parser.y -d \
	copy $(InputDir)\Parser.c $(InputDir)\Parser.cpp \
	del $(InputDir)\Parser.c /q \
	

"$(InputDir)/Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/Parser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 WORLDEDITOR"

# Begin Custom Build - Creating parser source
InputDir=.
InputPath=.\Parser.y

BuildCmds= \
	bison -o$(InputDir)/Parser.c $(InputDir)/Parser.y -d \
	copy $(InputDir)\Parser.c $(InputDir)\Parser.cpp \
	del $(InputDir)\Parser.c /q \
	

"$(InputDir)/Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/Parser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scanner.l

!IF  "$(CFG)" == "SeriousSkaStudio - Win32 Release"

# Begin Custom Build - Creating scanner source
InputDir=.
InputPath=.\Scanner.l

".\Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp  $(InputDir)\Scanner.l

# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 Debug"

# Begin Custom Build - Creating scanner source
InputDir=.
InputPath=.\Scanner.l

".\Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp  $(InputDir)\Scanner.l

# End Custom Build

!ELSEIF  "$(CFG)" == "SeriousSkaStudio - Win32 WORLDEDITOR"

# Begin Custom Build - Creating scanner source
InputDir=.
InputPath=.\Scanner.l

".\Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp  $(InputDir)\Scanner.l

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Generated Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\scanner.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Wizard Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SeriousSkaStudio.clw
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section SeriousSkaStudio : {F9043C85-F6F2-101A-A3C9-08002B2F49FB}
# 	2:21:DefaultSinkHeaderFile:commondialog.h
# 	2:16:DefaultSinkClass:CCmnDlgColor
# End Section
# Section SeriousSkaStudio : {72ADFD66-2C39-11D0-9903-00A0C91BC942}
# 	2:23:CDlgLightEffectProperty:Palette support added
# End Section
# Section SeriousSkaStudio : {083039C2-13F4-11D1-8B7E-0000F8754DA1}
# 	2:5:Class:CCmnDlgColor
# 	2:10:HeaderFile:commondialog.h
# 	2:8:ImplFile:commondialog.cpp
# End Section
