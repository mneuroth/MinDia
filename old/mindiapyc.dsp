# Microsoft Developer Studio Project File - Name="mindiapyc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mindiapyc - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "mindiapyc.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "mindiapyc.mak" CFG="mindiapyc - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "mindiapyc - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mindiapyc - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mindiapyc - Win32 Release"

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
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "$(PYTHONDIR)\include" /I "$(PYTHONDIR)\pc" /I "$(QTDIR)\include" /I ".\minsrv" /I ".\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "NO_DEBUG" /D "QT_THREAD_SUPPORT" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib wsock32.lib winmm.lib $(QTDIR)\lib\$(QTLIB) /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "mindiapyc - Win32 Debug"

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
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(PYTHONDIR)\include" /I "$(PYTHONDIR)\pc" /I "$(QTDIR)\include" /I ".\minsrv" /I ".\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib wsock32.lib winmm.lib $(QTDIR)\lib\$(QTLIB) /nologo /dll /debug /machine:I386 /out:"Debug/mindiapyc_d.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mindiapyc - Win32 Release"
# Name "mindiapyc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\mindiapyc\_minutils.cpp
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\mindia_wrap.cpp
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\mindiapy.cpp
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\moc_ScriptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\moc_scriptdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\moc_scriptfcn.cpp
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\ScriptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\scriptdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\scriptfcn.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\mindiapyc\ScriptDlg.h
# End Source File
# Begin Source File

SOURCE=.\mindiapyc\scriptdlgimpl.h

!IF  "$(CFG)" == "mindiapyc - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing scriptdlgimpl.h...
InputDir=.\mindiapyc
InputPath=.\mindiapyc\scriptdlgimpl.h
InputName=scriptdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindiapyc - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing scriptdlgimpl.h...
InputDir=.\mindiapyc
InputPath=.\mindiapyc\scriptdlgimpl.h
InputName=scriptdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mindiapyc\scriptfcn.h

!IF  "$(CFG)" == "mindiapyc - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing scriptfcn.h...
InputDir=.\mindiapyc
InputPath=.\mindiapyc\scriptfcn.h
InputName=scriptfcn

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindiapyc - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing scriptfcn.h...
InputDir=.\mindiapyc
InputPath=.\mindiapyc\scriptfcn.h
InputName=scriptfcn

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\mindiapyc\ScriptDlg.ui

!IF  "$(CFG)" == "mindiapyc - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ScriptDlg.ui...
InputDir=.\mindiapyc
InputPath=.\mindiapyc\ScriptDlg.ui
InputName=ScriptDlg

BuildCmds= \
	"%qtdir%\bin\uic.exe" $(InputPath) -o $(InputDir)\$(InputName).h \
	"%qtdir%\bin\uic.exe" $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	"%qtdir%\bin\moc.exe" $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindiapyc - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ScriptDlg.ui...
InputDir=.\mindiapyc
InputPath=.\mindiapyc\ScriptDlg.ui
InputName=ScriptDlg

BuildCmds= \
	"%qtdir%\bin\uic.exe" $(InputPath) -o $(InputDir)\$(InputName).h \
	"%qtdir%\bin\uic.exe" $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	"%qtdir%\bin\moc.exe" $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
