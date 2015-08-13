# Microsoft Developer Studio Project File - Name="Ecc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Ecc - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ecc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ecc.mak" CFG="Ecc - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ecc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Ecc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ecc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin
InputPath=.\Release\Ecc.exe
InputName=Ecc
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).exe $(ENGINE_DIR)\Bin >nul 
	copy Release\$(InputName).map $(ENGINE_DIR)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Ecc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(ENGINE_DIR)\Bin\Debug
InputPath=.\Debug\Ecc.exe
InputName=Ecc
SOURCE="$(InputPath)"

"$(ENGINE_DIR)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).exe $(ENGINE_DIR)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(ENGINE_DIR)\Bin\Debug >nul 
	
# End Custom Build
# Begin Special Build Tool
OutDir=.\Debug
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir  $(OutDir)\..\..\DebugFilesForCopy 	copy  $(OutDir)\Ecc.exe  $(OutDir)\..\..\DebugFilesForCopy\ 	copy  $(OutDir)\Ecc.map  $(OutDir)\..\..\DebugFilesForCopy\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Ecc - Win32 Release"
# Name "Ecc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Group "Grammar files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Parser.y

!IF  "$(CFG)" == "Ecc - Win32 Release"

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

!ELSEIF  "$(CFG)" == "Ecc - Win32 Debug"

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

!IF  "$(CFG)" == "Ecc - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=.\Scanner.l

".\Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp  $(InputDir)\Scanner.l

# End Custom Build

!ELSEIF  "$(CFG)" == "Ecc - Win32 Debug"

# Begin Custom Build - Creating scanner source
InputDir=.
InputPath=.\Scanner.l

".\Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp  $(InputDir)\Scanner.l

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Dummy"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\Parser.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\SCANNER.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\StdH.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
