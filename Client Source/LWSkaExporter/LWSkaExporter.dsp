# Microsoft Developer Studio Project File - Name="LWSkaExporter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=LWSkaExporter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LWSkaExporter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LWSkaExporter.mak" CFG="LWSkaExporter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LWSkaExporter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "LWSkaExporter - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LWSkaExporter - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LWSKAEXPORTER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I ".\SDK" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LWSKAEXPORTER_EXPORTS" /D "_X86_" /D "_WIN32" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /out:"Release/LWSkaExporter.p" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release\LWSkaExporter.p D:\Newtek\LightWave_3d_7.0\Plugins\3rdParty\Croteam\LWSkaExporter.p
# End Special Build Tool

!ELSEIF  "$(CFG)" == "LWSkaExporter - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LWSKAEXPORTER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I ".\SDK" /I "..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LWSKAEXPORTER_EXPORTS" /D "_X86_" /D "_WIN32" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug/LWSkaExporterD.p" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copying plugin
PostBuild_Cmds=copy Debug\LWSkaExporterD.p D:\Newtek\LightWave_3d_7.0\Plugins\3rdParty\Croteam\LWSkaExporter.p
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "LWSkaExporter - Win32 Release"
# Name "LWSkaExporter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnimExport.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyWeightMaps.cpp
# End Source File
# Begin Source File

SOURCE=.\Master.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshExport.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelerMeshExporter.cpp
# End Source File
# Begin Source File

SOURCE=.\plugin.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionsExport.cpp
# End Source File
# Begin Source File

SOURCE=.\serv.def
# End Source File
# Begin Source File

SOURCE=.\vecmat.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Base.h
# End Source File
# Begin Source File

SOURCE=.\vecmat.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "SDK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SDK\lwanimlod.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwanimsav.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwchannel.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwcmdseq.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwcustobj.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwdialog.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwdisplay.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwdisplce.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwdyna.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwenvel.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwenviron.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwfilter.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwframbuf.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwgeneric.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwglobsrv.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwhandler.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwhost.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwimage.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwimageio.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwio.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmaster.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmath.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmeshedt.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmeshes.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmodeler.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmodtool.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmodule.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmonitor.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmotion.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwmtutil.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwobjimp.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwobjrep.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwpanel.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwpreview.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwprtcl.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwrender.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwscenecv.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwserver.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwshader.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwshelf.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwsurf.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwsurfed.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwtexture.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwtool.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwtxtr.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwtxtred.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwtypes.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwvolume.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwvparm.h
# End Source File
# Begin Source File

SOURCE=.\SDK\lwxpanel.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\COPYING
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
