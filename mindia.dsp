# Microsoft Developer Studio Project File - Name="mindia" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=mindia - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "mindia.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "mindia.mak" CFG="mindia - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "mindia - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "mindia - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mindia - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "$(QTDIR)\include" /D "NDEBUG" /D "NO_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib imm32.lib wsock32.lib winmm.lib $(QTDIR)\lib\qt-mt230nc.lib $(QTDIR)\lib\qtmain.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

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
# ADD BASE CPP /nologo /MD /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib imm32.lib wsock32.lib winmm.lib $(QTDIR)\lib\qt-mt230nc.lib $(QTDIR)\lib\qtmain.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mindia - Win32 Release"
# Name "mindia - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutExtDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\appconfig.cpp
# End Source File
# Begin Source File

SOURCE=.\applscriptenv.cpp
# End Source File
# Begin Source File

SOURCE=.\ComLoggingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\comlogimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\CommentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\commentdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\configdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigPlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\configplayerdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigurationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\diainfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DiaInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\diainfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\diapresentation.cpp
# End Source File
# Begin Source File

SOURCE=.\diaprojector.cpp
# End Source File
# Begin Source File

SOURCE=.\doccontroler.cpp
# End Source File
# Begin Source File

SOURCE=.\dyngraphop.cpp
# End Source File
# Begin Source File

SOURCE=.\DynTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dyntextdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\EnterValueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EventMapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\eventmapdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hitem.cpp
# End Source File
# Begin Source File

SOURCE=.\hitemview.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\mincmdproc.cpp
# End Source File
# Begin Source File

SOURCE=.\mindia.rc
# End Source File
# Begin Source File

SOURCE=.\mindiawindow.cpp
# End Source File
# Begin Source File

SOURCE=.\minisound.cpp
# End Source File
# Begin Source File

SOURCE=.\minutils.cpp
# End Source File
# Begin Source File

SOURCE=.\misctools.cpp
# End Source File
# Begin Source File

SOURCE=.\misctools.h
# End Source File
# Begin Source File

SOURCE=.\moc\moc_AboutExtDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_ComLoggingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_comlogimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_CommentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_commentdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_configdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_ConfigPlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_configplayerdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_ConfigurationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_DiaInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_diainfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_doccontroler.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_dyngraphop.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_DynTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_dyntextdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_EnterValueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_EventMapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_eventmapdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_HelpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_hitemview.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_LicenseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_mindiawindow.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_pcdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_pddlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_PlayInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_playinfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_PresentationDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_ProjectorControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_sndinfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_SoundInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_timelineview.cpp
# End Source File
# Begin Source File

SOURCE=.\moc\moc_timerjobproc.cpp
# End Source File
# Begin Source File

SOURCE=.\pcdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\pddlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\playinfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\PresentationDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectorControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\qtmtlock.cpp
# End Source File
# Begin Source File

SOURCE=.\rolleicom.cpp
# End Source File
# Begin Source File

SOURCE=.\sndinfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\soundinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\timelineaxis.cpp
# End Source File
# Begin Source File

SOURCE=.\timelineitem.cpp
# End Source File
# Begin Source File

SOURCE=.\timelineview.cpp
# End Source File
# Begin Source File

SOURCE=.\timerjobproc.cpp
# End Source File
# Begin Source File

SOURCE=.\writexml.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutExtDlg.h
# End Source File
# Begin Source File

SOURCE=.\appconfig.h
# End Source File
# Begin Source File

SOURCE=.\applscriptenv.h
# End Source File
# Begin Source File

SOURCE=.\ComLoggingDlg.h
# End Source File
# Begin Source File

SOURCE=.\comlogimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing comlogimpl.h...
InputPath=.\comlogimpl.h

"moc\moc_comlogimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe comlogimpl.h -o moc\moc_comlogimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing comlogimpl.h...
InputPath=.\comlogimpl.h

"moc\moc_comlogimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe comlogimpl.h -o moc\moc_comlogimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CommentDlg.h
# End Source File
# Begin Source File

SOURCE=.\commentdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing commentdlgimpl.h...
InputPath=.\commentdlgimpl.h

"moc\moc_commentdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe commentdlgimpl.h -o moc\moc_commentdlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing commentdlgimpl.h...
InputPath=.\commentdlgimpl.h

"moc\moc_commentdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe commentdlgimpl.h -o moc\moc_commentdlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\configdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing configdlgimpl.h...
InputPath=.\configdlgimpl.h

"moc\moc_configdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe configdlgimpl.h -o moc\moc_configdlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing configdlgimpl.h...
InputPath=.\configdlgimpl.h

"moc\moc_configdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe configdlgimpl.h -o moc\moc_configdlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigPlayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\configplayerdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputPath=.\configplayerdlgimpl.h

"moc\moc_configplayerdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe configplayerdlgimpl.h -o moc\moc_configplayerdlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputPath=.\configplayerdlgimpl.h

"moc\moc_configplayerdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe configplayerdlgimpl.h -o moc\moc_configplayerdlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigurationDlg.h
# End Source File
# Begin Source File

SOURCE=.\diacallback.h
# End Source File
# Begin Source File

SOURCE=.\diainfo.h
# End Source File
# Begin Source File

SOURCE=.\DiaInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\diainfodlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing diainfodlgimpl.h...
InputPath=.\diainfodlgimpl.h

"moc\moc_diainfodlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe diainfodlgimpl.h -o moc\moc_diainfodlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing diainfodlgimpl.h...
InputPath=.\diainfodlgimpl.h

"moc\moc_diainfodlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe diainfodlgimpl.h -o moc\moc_diainfodlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\diapresentation.h
# End Source File
# Begin Source File

SOURCE=.\diaprojector.h
# End Source File
# Begin Source File

SOURCE=.\doccontroler.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing doccontroler.h...
InputPath=.\doccontroler.h

"moc\moc_doccontroler.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe doccontroler.h -o moc\moc_doccontroler.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing doccontroler.h...
InputPath=.\doccontroler.h

"moc\moc_doccontroler.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe doccontroler.h -o moc\moc_doccontroler.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dyngraphop.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputPath=.\dyngraphop.h

"moc\moc_dyngraphop.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe dyngraphop.h -o moc\moc_dyngraphop.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputPath=.\dyngraphop.h

"moc\moc_dyngraphop.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe dyngraphop.h -o moc\moc_dyngraphop.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynTextDlg.h
# End Source File
# Begin Source File

SOURCE=.\dyntextdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\dyntextdlgimpl.h
InputName=dyntextdlgimpl

"moc\moc_dyntextdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe dyntextdlgimpl.h -o moc\moc_dyntextdlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\dyntextdlgimpl.h
InputName=dyntextdlgimpl

"moc\moc_dyntextdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe dyntextdlgimpl.h -o moc\moc_dyntextdlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnterValueDlg.h
# End Source File
# Begin Source File

SOURCE=.\EventMapDlg.h
# End Source File
# Begin Source File

SOURCE=.\eventmapdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing eventmapdlgimpl.h...
InputPath=.\eventmapdlgimpl.h

"moc\moc_eventmapdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe eventmapdlgimpl.h -o moc\moc_eventmapdlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing eventmapdlgimpl.h...
InputPath=.\eventmapdlgimpl.h

"moc\moc_eventmapdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe eventmapdlgimpl.h -o moc\moc_eventmapdlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HelpDlg.h
# End Source File
# Begin Source File

SOURCE=.\hitem.h
# End Source File
# Begin Source File

SOURCE=.\hitemview.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing hitemview.h...
InputPath=.\hitemview.h

"moc\moc_hitemview.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe hitemview.h -o moc\moc_hitemview.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing hitemview.h...
InputPath=.\hitemview.h

"moc\moc_hitemview.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe hitemview.h -o moc\moc_hitemview.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iocontainer.h
# End Source File
# Begin Source File

SOURCE=.\LicenseDlg.h
# End Source File
# Begin Source File

SOURCE=.\mincmdproc.h
# End Source File
# Begin Source File

SOURCE=.\mindiawindow.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing mindiawindow.h...
InputPath=.\mindiawindow.h

"moc\moc_mindiawindow.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe mindiawindow.h -o moc\moc_mindiawindow.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing mindiawindow.h...
InputPath=.\mindiawindow.h

"moc\moc_mindiawindow.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe mindiawindow.h -o moc\moc_mindiawindow.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\minexception.h
# End Source File
# Begin Source File

SOURCE=.\minisound.h
# End Source File
# Begin Source File

SOURCE=.\minlog.h
# End Source File
# Begin Source File

SOURCE=.\minutils.h
# End Source File
# Begin Source File

SOURCE=.\mp3file.h
# End Source File
# Begin Source File

SOURCE=.\osdep2.h
# End Source File
# Begin Source File

SOURCE=.\pcdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing pcdlgimpl.h...
InputPath=.\pcdlgimpl.h

"moc\moc_pcdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe pcdlgimpl.h -o moc\moc_pcdlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing pcdlgimpl.h...
InputPath=.\pcdlgimpl.h

"moc\moc_pcdlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe pcdlgimpl.h -o moc\moc_pcdlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pddlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing pddlgimpl.h...
InputPath=.\pddlgimpl.h

"moc\moc_pddlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe pddlgimpl.h -o moc\moc_pddlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing pddlgimpl.h...
InputPath=.\pddlgimpl.h

"moc\moc_pddlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe pddlgimpl.h -o moc\moc_pddlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlayInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\playinfodlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing playinfodlgimpl.h...
InputPath=.\playinfodlgimpl.h

"moc\moc_playinfodlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe playinfodlgimpl.h -o moc\moc_playinfodlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing playinfodlgimpl.h...
InputPath=.\playinfodlgimpl.h

"moc\moc_playinfodlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe playinfodlgimpl.h -o moc\moc_playinfodlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PresentationDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProjectorControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\qtmtlock.h
# End Source File
# Begin Source File

SOURCE=.\rolleicom.h
# End Source File
# Begin Source File

SOURCE=.\sndinfodlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing sndinfodlgimpl.h...
InputPath=.\sndinfodlgimpl.h

"moc\moc_sndinfodlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe sndinfodlgimpl.h -o moc\moc_sndinfodlgimpl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing sndinfodlgimpl.h...
InputPath=.\sndinfodlgimpl.h

"moc\moc_sndinfodlgimpl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe sndinfodlgimpl.h -o moc\moc_sndinfodlgimpl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\soundinfo.h
# End Source File
# Begin Source File

SOURCE=.\SoundInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\timelineaxis.h
# End Source File
# Begin Source File

SOURCE=.\timelineitem.h
# End Source File
# Begin Source File

SOURCE=.\timelineview.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Moc'ing timelineview.h...
InputPath=.\timelineview.h

"moc\moc_timelineview.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe timelineview.h -o moc\moc_timelineview.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Moc'ing timelineview.h...
InputPath=.\timelineview.h

"moc\moc_timelineview.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe timelineview.h -o moc\moc_timelineview.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\timerjobproc.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputPath=.\timerjobproc.h

"moc\moc_timerjobproc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe timerjobproc.h -o moc\moc_timerjobproc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputPath=.\timerjobproc.h

"moc\moc_timerjobproc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QTDIR%\bin\moc.exe timerjobproc.h -o moc\moc_timerjobproc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\writexml.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\mindia.ico
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AboutExtDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\AboutExtDlg.ui
InputName=AboutExtDlg

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o moc\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\AboutExtDlg.ui
InputName=AboutExtDlg

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o moc\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ComLoggingDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing ComLoggingDlg.ui...
InputPath=.\ComLoggingDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic ComLoggingDlg.ui -o ComLoggingDlg.h \
	$(QTDIR)\bin\uic ComLoggingDlg.ui -i ComLoggingDlg.h -o ComLoggingDlg.cpp \
	%QTDIR%\bin\moc ComLoggingDlg.h -o moc\moc_ComLoggingDlg.cpp \
	

"ComLoggingDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ComLoggingDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_ComLoggingDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing ComLoggingDlg.ui...
InputPath=.\ComLoggingDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic ComLoggingDlg.ui -o ComLoggingDlg.h \
	$(QTDIR)\bin\uic ComLoggingDlg.ui -i ComLoggingDlg.h -o ComLoggingDlg.cpp \
	%QTDIR%\bin\moc ComLoggingDlg.h -o moc\moc_ComLoggingDlg.cpp \
	

"ComLoggingDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ComLoggingDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_ComLoggingDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CommentDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing CommentDlg.ui...
InputPath=.\CommentDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic CommentDlg.ui -o CommentDlg.h \
	$(QTDIR)\bin\uic CommentDlg.ui -i CommentDlg.h -o CommentDlg.cpp \
	%QTDIR%\bin\moc CommentDlg.h -o moc\moc_CommentDlg.cpp \
	

"CommentDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"CommentDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_CommentDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing CommentDlg.ui...
InputPath=.\CommentDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic CommentDlg.ui -o CommentDlg.h \
	$(QTDIR)\bin\uic CommentDlg.ui -i CommentDlg.h -o CommentDlg.cpp \
	%QTDIR%\bin\moc CommentDlg.h -o moc\moc_CommentDlg.cpp \
	

"CommentDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"CommentDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_CommentDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigPlayerDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing ConfigPlayerDlg.ui ...
InputPath=.\ConfigPlayerDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic ConfigPlayerDlg.ui -o ConfigPlayerDlg.h \
	$(QTDIR)\bin\uic ConfigPlayerDlg.ui -i ConfigPlayerDlg.h -o ConfigPlayerDlg.cpp \
	%QTDIR%\bin\moc ConfigPlayerDlg.h -o moc\moc_ConfigPlayerDlg.cpp \
	

"ConfigPlayerDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ConfigPlayerDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_ConfigPlayerDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing ConfigPlayerDlg.ui ...
InputPath=.\ConfigPlayerDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic ConfigPlayerDlg.ui -o ConfigPlayerDlg.h \
	$(QTDIR)\bin\uic ConfigPlayerDlg.ui -i ConfigPlayerDlg.h -o ConfigPlayerDlg.cpp \
	%QTDIR%\bin\moc ConfigPlayerDlg.h -o moc\moc_ConfigPlayerDlg.cpp \
	

"ConfigPlayerDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ConfigPlayerDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_ConfigPlayerDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigurationDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing ConfigurationDlg.ui...
InputPath=.\ConfigurationDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic ConfigurationDlg.ui -o ConfigurationDlg.h \
	$(QTDIR)\bin\uic ConfigurationDlg.ui -i ConfigurationDlg.h -o ConfigurationDlg.cpp \
	%QTDIR%\bin\moc ConfigurationDlg.h -o moc\moc_ConfigurationDlg.cpp \
	

"ConfigurationDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ConfigurationDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_ConfigurationDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing ConfigurationDlg.ui...
InputPath=.\ConfigurationDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic ConfigurationDlg.ui -o ConfigurationDlg.h \
	$(QTDIR)\bin\uic ConfigurationDlg.ui -i ConfigurationDlg.h -o ConfigurationDlg.cpp \
	%QTDIR%\bin\moc ConfigurationDlg.h -o moc\moc_ConfigurationDlg.cpp \
	

"ConfigurationDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ConfigurationDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_ConfigurationDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DiaInfoDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing DiaInfoDlg.ui...
InputPath=.\DiaInfoDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic DiaInfoDlg.ui -o DiaInfoDlg.h \
	$(QTDIR)\bin\uic DiaInfoDlg.ui -i DiaInfoDlg.h -o DiaInfoDlg.cpp \
	%QTDIR%\bin\moc DiaInfoDlg.h -o moc\moc_DiaInfoDlg.cpp \
	

"DiaInfoDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"DiaInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_DiaInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing DiaInfoDlg.ui...
InputPath=.\DiaInfoDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic DiaInfoDlg.ui -o DiaInfoDlg.h \
	$(QTDIR)\bin\uic DiaInfoDlg.ui -i DiaInfoDlg.h -o DiaInfoDlg.cpp \
	%QTDIR%\bin\moc DiaInfoDlg.h -o moc\moc_DiaInfoDlg.cpp \
	

"DiaInfoDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"DiaInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_DiaInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynTextDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DynTextDlg.ui
InputName=DynTextDlg

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\DynTextDlg.ui
InputName=DynTextDlg

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnterValueDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing EnterValueDlg.ui...
InputPath=.\EnterValueDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic EnterValueDlg.ui -o EnterValueDlg.h \
	$(QTDIR)\bin\uic EnterValueDlg.ui -i EnterValueDlg.h -o EnterValueDlg.cpp \
	%QTDIR%\bin\moc EnterValueDlg.h -o moc\moc_EnterValueDlg.cpp \
	

"EnterValueDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"EnterValueDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_EnterValueDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing EnterValueDlg.ui...
InputPath=.\EnterValueDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic EnterValueDlg.ui -o EnterValueDlg.h \
	$(QTDIR)\bin\uic EnterValueDlg.ui -i EnterValueDlg.h -o EnterValueDlg.cpp \
	%QTDIR%\bin\moc EnterValueDlg.h -o moc\moc_EnterValueDlg.cpp \
	

"EnterValueDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"EnterValueDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_EnterValueDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EventMapDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing EventMapDlg.ui...
InputPath=.\EventMapDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic EventMapDlg.ui -o EventMapDlg.h \
	$(QTDIR)\bin\uic EventMapDlg.ui -i EventMapDlg.h -o EventMapDlg.cpp \
	%QTDIR%\bin\moc EventMapDlg.h -o moc\moc_EventMapDlg.cpp \
	

"EventMapDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"EventMapDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_EventMapDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing EventMapDlg.ui...
InputPath=.\EventMapDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic EventMapDlg.ui -o EventMapDlg.h \
	$(QTDIR)\bin\uic EventMapDlg.ui -i EventMapDlg.h -o EventMapDlg.cpp \
	%QTDIR%\bin\moc EventMapDlg.h -o moc\moc_EventMapDlg.cpp \
	

"EventMapDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"EventMapDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_EventMapDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HelpDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing HelpDlg.ui...
InputPath=.\HelpDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic HelpDlg.ui -o HelpDlg.h \
	$(QTDIR)\bin\uic HelpDlg.ui -i HelpDlg.h -o HelpDlg.cpp \
	%QTDIR%\bin\moc HelpDlg.h -o moc\moc_HelpDlg.cpp \
	

"HelpDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"HelpDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_HelpDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing HelpDlg.ui...
InputPath=.\HelpDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic HelpDlg.ui -o HelpDlg.h \
	$(QTDIR)\bin\uic HelpDlg.ui -i HelpDlg.h -o HelpDlg.cpp \
	%QTDIR%\bin\moc HelpDlg.h -o moc\moc_HelpDlg.cpp \
	

"HelpDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"HelpDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_HelpDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LicenseDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\LicenseDlg.ui
InputName=LicenseDlg

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o moc\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.
InputPath=.\LicenseDlg.ui
InputName=LicenseDlg

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o $(InputDir)\$(InputName).h \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o moc\moc_$(InputName).cpp \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlayInfoDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing PlayInfoDlg.ui...
InputPath=.\PlayInfoDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic PlayInfoDlg.ui -o PlayInfoDlg.h \
	$(QTDIR)\bin\uic PlayInfoDlg.ui -i PlayInfoDlg.h -o PlayInfoDlg.cpp \
	%QTDIR%\bin\moc PlayInfoDlg.h -o moc\moc_PlayInfoDlg.cpp \
	

"PlayInfoDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"PlayInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_PlayInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing PlayInfoDlg.ui...
InputPath=.\PlayInfoDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic PlayInfoDlg.ui -o PlayInfoDlg.h \
	$(QTDIR)\bin\uic PlayInfoDlg.ui -i PlayInfoDlg.h -o PlayInfoDlg.cpp \
	%QTDIR%\bin\moc PlayInfoDlg.h -o moc\moc_PlayInfoDlg.cpp \
	

"PlayInfoDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"PlayInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_PlayInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PresentationDataDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing PresentationDataDlg.ui...
InputPath=.\PresentationDataDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic PresentationDataDlg.ui -o PresentationDataDlg.h \
	$(QTDIR)\bin\uic PresentationDataDlg.ui -i PresentationDataDlg.h -o PresentationDataDlg.cpp \
	%QTDIR%\bin\moc PresentationDataDlg.h -o moc\moc_PresentationDataDlg.cpp \
	

"PresentationDataDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"PresentationDataDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_PresentationDataDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing PresentationDataDlg.ui...
InputPath=.\PresentationDataDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic PresentationDataDlg.ui -o PresentationDataDlg.h \
	$(QTDIR)\bin\uic PresentationDataDlg.ui -i PresentationDataDlg.h -o PresentationDataDlg.cpp \
	%QTDIR%\bin\moc PresentationDataDlg.h -o moc\moc_PresentationDataDlg.cpp \
	

"PresentationDataDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"PresentationDataDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_PresentationDataDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProjectorControlDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing ProjectorControlDlg.ui...
InputPath=.\ProjectorControlDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic ProjectorControlDlg.ui -o ProjectorControlDlg.h \
	$(QTDIR)\bin\uic ProjectorControlDlg.ui -i ProjectorControlDlg.h -o ProjectorControlDlg.cpp \
	%QTDIR%\bin\moc ProjectorControlDlg.h -o moc\moc_ProjectorControlDlg.cpp \
	

"ProjectorControlDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ProjectorControlDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_ProjectorControlDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing ProjectorControlDlg.ui...
InputPath=.\ProjectorControlDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic ProjectorControlDlg.ui -o ProjectorControlDlg.h \
	$(QTDIR)\bin\uic ProjectorControlDlg.ui -i ProjectorControlDlg.h -o ProjectorControlDlg.cpp \
	%QTDIR%\bin\moc ProjectorControlDlg.h -o moc\moc_ProjectorControlDlg.cpp \
	

"ProjectorControlDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ProjectorControlDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_ProjectorControlDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SoundInfoDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# Begin Custom Build - Uic'ing SoundInfoDlg.ui...
InputPath=.\SoundInfoDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic SoundInfoDlg.ui -o SoundInfoDlg.h \
	$(QTDIR)\bin\uic SoundInfoDlg.ui -i SoundInfoDlg.h -o SoundInfoDlg.cpp \
	%QTDIR%\bin\moc SoundInfoDlg.h -o moc\moc_SoundInfoDlg.cpp \
	

"SoundInfoDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"SoundInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_SoundInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# Begin Custom Build - Uic'ing SoundInfoDlg.ui...
InputPath=.\SoundInfoDlg.ui

BuildCmds= \
	$(QTDIR)\bin\uic SoundInfoDlg.ui -o SoundInfoDlg.h \
	$(QTDIR)\bin\uic SoundInfoDlg.ui -i SoundInfoDlg.h -o SoundInfoDlg.cpp \
	%QTDIR%\bin\moc SoundInfoDlg.h -o moc\moc_SoundInfoDlg.cpp \
	

"SoundInfoDlg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"SoundInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"moc\moc_SoundInfoDlg.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
