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
# ADD CPP /nologo /MD /W3 /GX /O1 /I "$(QTDIR)\include" /I ".\minsrv" /I ".\gendev" /D "NDEBUG" /D "NO_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib $(QTDIR)\lib\$(QTLIB) $(QTDIR)\lib\qtmain.lib /nologo /subsystem:windows /machine:I386

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
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /I ".\minsrv" /I ".\gendev" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib $(QTDIR)\lib\$(QTLIB) $(QTDIR)\lib\qtmain.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mindia - Win32 Release"
# Name "mindia - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\AboutExtDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\appconfig.cpp
# End Source File
# Begin Source File

SOURCE=.\src\applscriptenv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ComLoggingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\comlogimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CommentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\commentdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\configdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ConfigPlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\configplayerdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ConfigurationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CreateMovieDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\createmoviedlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\diainfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DiaInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\diainfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\diapresentation.cpp
# End Source File
# Begin Source File

SOURCE=.\src\diaprojector.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dllbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\doccontroler.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dyngraphop.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DynTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dyntextdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EnterValueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EventMapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eventmapdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HelpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\helpdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hitem.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hitemview.cpp
# End Source File
# Begin Source File

SOURCE=.\src\LicenseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\minbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mincmdproc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mindia.rc
# End Source File
# Begin Source File

SOURCE=.\src\mindiawindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\miniini.cpp
# End Source File
# Begin Source File

SOURCE=.\src\minisound.cpp
# End Source File
# Begin Source File

SOURCE=.\src\minutils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\misctools.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_AboutExtDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_ComLoggingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_comlogimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_CommentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_commentdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_configdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_ConfigPlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_configplayerdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_ConfigurationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_CreateMovieDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_createmoviedlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_DiaInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_diainfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_doccontroler.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_dyngraphop.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_DynTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_dyntextdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_EnterValueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_EventMapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_eventmapdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_HelpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_helpdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_hitemview.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_LicenseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_mindiawindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_pcdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_pddlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_PlayInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_playinfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_PresentationDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_ProjectorControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_sndinfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_SoundInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_timelineview.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_timerjobproc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\pcdlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\pddlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PlayInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\playinfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PresentationDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ProjectorControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\qtmtlock.cpp
# End Source File
# Begin Source File

SOURCE=.\src\read_mp3.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rolleicom.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sndinfodlgimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\soundinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SoundInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\timelineaxis.cpp
# End Source File
# Begin Source File

SOURCE=.\src\timelineitem.cpp
# End Source File
# Begin Source File

SOURCE=.\src\timelineview.cpp
# End Source File
# Begin Source File

SOURCE=.\src\timerjobproc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\wavfile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\writexml.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\AboutExtDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\appconfig.h
# End Source File
# Begin Source File

SOURCE=.\src\applscriptenv.h
# End Source File
# Begin Source File

SOURCE=.\src\ComLoggingDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\comlogimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing comlogimpl.h...
InputDir=.\src
InputPath=.\src\comlogimpl.h
InputName=comlogimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing comlogimpl.h...
InputDir=.\src
InputPath=.\src\comlogimpl.h
InputName=comlogimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\CommentDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\commentdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing commentdlgimpl.h...
InputDir=.\src
InputPath=.\src\commentdlgimpl.h
InputName=commentdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing commentdlgimpl.h...
InputDir=.\src
InputPath=.\src\commentdlgimpl.h
InputName=commentdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\config.h
# End Source File
# Begin Source File

SOURCE=.\src\configdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing configdlgimpl.h...
InputDir=.\src
InputPath=.\src\configdlgimpl.h
InputName=configdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing configdlgimpl.h...
InputDir=.\src
InputPath=.\src\configdlgimpl.h
InputName=configdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ConfigPlayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\configplayerdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing configplayerdlgimpl.h...
InputDir=.\src
InputPath=.\src\configplayerdlgimpl.h
InputName=configplayerdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing configplayerdlgimpl.h...
InputDir=.\src
InputPath=.\src\configplayerdlgimpl.h
InputName=configplayerdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ConfigurationDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\CreateMovieDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\createmoviedlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing createmoviedlgimpl.h...
InputDir=.\src
InputPath=.\src\createmoviedlgimpl.h
InputName=createmoviedlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing createmoviedlgimpl.h...
InputDir=.\src
InputPath=.\src\createmoviedlgimpl.h
InputName=createmoviedlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\diacallback.h
# End Source File
# Begin Source File

SOURCE=.\src\diainfo.h
# End Source File
# Begin Source File

SOURCE=.\src\DiaInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\diainfodlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing diainfodlgimpl.h...
InputDir=.\src
InputPath=.\src\diainfodlgimpl.h
InputName=diainfodlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing diainfodlgimpl.h...
InputDir=.\src
InputPath=.\src\diainfodlgimpl.h
InputName=diainfodlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\diapresentation.h
# End Source File
# Begin Source File

SOURCE=.\src\diaprojector.h
# End Source File
# Begin Source File

SOURCE=.\src\dllbase.h
# End Source File
# Begin Source File

SOURCE=.\src\dllexport.h
# End Source File
# Begin Source File

SOURCE=.\src\doccontroler.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing doccontroler.h...
InputDir=.\src
InputPath=.\src\doccontroler.h
InputName=doccontroler

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing doccontroler.h...
InputDir=.\src
InputPath=.\src\doccontroler.h
InputName=doccontroler

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\dyngraphop.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing dyngraphop.h...
InputDir=.\src
InputPath=.\src\dyngraphop.h
InputName=dyngraphop

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing dyngraphop.h...
InputDir=.\src
InputPath=.\src\dyngraphop.h
InputName=dyngraphop

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\DynTextDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\dyntextdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing dyntextdlgimpl.h...
InputDir=.\src
InputPath=.\src\dyntextdlgimpl.h
InputName=dyntextdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing dyntextdlgimpl.h...
InputDir=.\src
InputPath=.\src\dyntextdlgimpl.h
InputName=dyntextdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\EnterValueDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\EventMapDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\eventmapdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing eventmapdlgimpl.h...
InputDir=.\src
InputPath=.\src\eventmapdlgimpl.h
InputName=eventmapdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing eventmapdlgimpl.h...
InputDir=.\src
InputPath=.\src\eventmapdlgimpl.h
InputName=eventmapdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\global_patch.h
# End Source File
# Begin Source File

SOURCE=.\src\HelpDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\helpdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing helpdlgimpl.h...
InputDir=.\src
InputPath=.\src\helpdlgimpl.h
InputName=helpdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing helpdlgimpl.h...
InputDir=.\src
InputPath=.\src\helpdlgimpl.h
InputName=helpdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\hitem.h
# End Source File
# Begin Source File

SOURCE=.\src\hitemview.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing hitemview.h...
InputDir=.\src
InputPath=.\src\hitemview.h
InputName=hitemview

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing hitemview.h...
InputDir=.\src
InputPath=.\src\hitemview.h
InputName=hitemview

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\iniconst.h
# End Source File
# Begin Source File

SOURCE=.\src\iocontainer.h
# End Source File
# Begin Source File

SOURCE=.\src\iscript.h
# End Source File
# Begin Source File

SOURCE=.\src\LicenseDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\minbase.h
# End Source File
# Begin Source File

SOURCE=.\src\mincmdproc.h
# End Source File
# Begin Source File

SOURCE=.\src\mindiawindow.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing mindiawindow.h...
InputDir=.\src
InputPath=.\src\mindiawindow.h
InputName=mindiawindow

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing mindiawindow.h...
InputDir=.\src
InputPath=.\src\mindiawindow.h
InputName=mindiawindow

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\minexception.h
# End Source File
# Begin Source File

SOURCE=.\src\minhandle.h
# End Source File
# Begin Source File

SOURCE=.\src\miniini.h
# End Source File
# Begin Source File

SOURCE=.\src\minisound.h
# End Source File
# Begin Source File

SOURCE=.\src\minlog.h
# End Source File
# Begin Source File

SOURCE=.\src\minutils.h
# End Source File
# Begin Source File

SOURCE=.\src\misctools.h
# End Source File
# Begin Source File

SOURCE=.\src\osdep2.h
# End Source File
# Begin Source File

SOURCE=.\src\pcdlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing pcdlgimpl.h...
InputDir=.\src
InputPath=.\src\pcdlgimpl.h
InputName=pcdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing pcdlgimpl.h...
InputDir=.\src
InputPath=.\src\pcdlgimpl.h
InputName=pcdlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\pddlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing pddlgimpl.h...
InputDir=.\src
InputPath=.\src\pddlgimpl.h
InputName=pddlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing pddlgimpl.h...
InputDir=.\src
InputPath=.\src\pddlgimpl.h
InputName=pddlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\PlayInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\playinfodlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing playinfodlgimpl.h...
InputDir=.\src
InputPath=.\src\playinfodlgimpl.h
InputName=playinfodlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing playinfodlgimpl.h...
InputDir=.\src
InputPath=.\src\playinfodlgimpl.h
InputName=playinfodlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\PresentationDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\ProjectorControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\qtmtlock.h
# End Source File
# Begin Source File

SOURCE=.\src\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\rolleicom.h
# End Source File
# Begin Source File

SOURCE=.\src\sndinfodlgimpl.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing sndinfodlgimpl.h...
InputDir=.\src
InputPath=.\src\sndinfodlgimpl.h
InputName=sndinfodlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing sndinfodlgimpl.h...
InputDir=.\src
InputPath=.\src\sndinfodlgimpl.h
InputName=sndinfodlgimpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\soundinfo.h
# End Source File
# Begin Source File

SOURCE=.\src\SoundInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\sysdep.h
# End Source File
# Begin Source File

SOURCE=.\src\timelineaxis.h
# End Source File
# Begin Source File

SOURCE=.\src\timelineitem.h
# End Source File
# Begin Source File

SOURCE=.\src\timelineview.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing timelineview.h...
InputDir=.\src
InputPath=.\src\timelineview.h
InputName=timelineview

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing timelineview.h...
InputDir=.\src
InputPath=.\src\timelineview.h
InputName=timelineview

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\timerjobproc.h

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing timerjobproc.h...
InputDir=.\src
InputPath=.\src\timerjobproc.h
InputName=timerjobproc

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing timerjobproc.h...
InputDir=.\src
InputPath=.\src\timerjobproc.h
InputName=timerjobproc

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"%qtdir%\bin\moc.exe" "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\wavfile.h
# End Source File
# Begin Source File

SOURCE=.\src\writexml.h
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

SOURCE=.\src\AboutExtDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing AboutExtDlg.ui...
InputDir=.\src
InputPath=.\src\AboutExtDlg.ui
InputName=AboutExtDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing AboutExtDlg.ui...
InputDir=.\src
InputPath=.\src\AboutExtDlg.ui
InputName=AboutExtDlg

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
# Begin Source File

SOURCE=.\src\ComLoggingDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ComLoggingDlg.ui...
InputDir=.\src
InputPath=.\src\ComLoggingDlg.ui
InputName=ComLoggingDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ComLoggingDlg.ui...
InputDir=.\src
InputPath=.\src\ComLoggingDlg.ui
InputName=ComLoggingDlg

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
# Begin Source File

SOURCE=.\src\CommentDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing CommentDlg.ui...
InputDir=.\src
InputPath=.\src\CommentDlg.ui
InputName=CommentDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing CommentDlg.ui...
InputDir=.\src
InputPath=.\src\CommentDlg.ui
InputName=CommentDlg

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
# Begin Source File

SOURCE=.\src\ConfigPlayerDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ConfigPlayerDlg.ui...
InputDir=.\src
InputPath=.\src\ConfigPlayerDlg.ui
InputName=ConfigPlayerDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ConfigPlayerDlg.ui...
InputDir=.\src
InputPath=.\src\ConfigPlayerDlg.ui
InputName=ConfigPlayerDlg

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
# Begin Source File

SOURCE=.\src\ConfigurationDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ConfigurationDlg.ui...
InputDir=.\src
InputPath=.\src\ConfigurationDlg.ui
InputName=ConfigurationDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ConfigurationDlg.ui...
InputDir=.\src
InputPath=.\src\ConfigurationDlg.ui
InputName=ConfigurationDlg

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
# Begin Source File

SOURCE=.\src\DiaInfoDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing DiaInfoDlg.ui...
InputDir=.\src
InputPath=.\src\DiaInfoDlg.ui
InputName=DiaInfoDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing DiaInfoDlg.ui...
InputDir=.\src
InputPath=.\src\DiaInfoDlg.ui
InputName=DiaInfoDlg

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
# Begin Source File

SOURCE=.\src\DynTextDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing DynTextDlg.ui...
InputDir=.\src
InputPath=.\src\DynTextDlg.ui
InputName=DynTextDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing DynTextDlg.ui...
InputDir=.\src
InputPath=.\src\DynTextDlg.ui
InputName=DynTextDlg

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
# Begin Source File

SOURCE=.\src\EnterValueDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing EnterValueDlg.ui...
InputDir=.\src
InputPath=.\src\EnterValueDlg.ui
InputName=EnterValueDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing EnterValueDlg.ui...
InputDir=.\src
InputPath=.\src\EnterValueDlg.ui
InputName=EnterValueDlg

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
# Begin Source File

SOURCE=.\src\EventMapDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing EventMapDlg.ui...
InputDir=.\src
InputPath=.\src\EventMapDlg.ui
InputName=EventMapDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing EventMapDlg.ui...
InputDir=.\src
InputPath=.\src\EventMapDlg.ui
InputName=EventMapDlg

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
# Begin Source File

SOURCE=.\src\HelpDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing HelpDlg.ui...
InputDir=.\src
InputPath=.\src\HelpDlg.ui
InputName=HelpDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing HelpDlg.ui...
InputDir=.\src
InputPath=.\src\HelpDlg.ui
InputName=HelpDlg

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
# Begin Source File

SOURCE=.\src\LicenseDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing LicenseDlg.ui...
InputDir=.\src
InputPath=.\src\LicenseDlg.ui
InputName=LicenseDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing LicenseDlg.ui...
InputDir=.\src
InputPath=.\src\LicenseDlg.ui
InputName=LicenseDlg

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
# Begin Source File

SOURCE=.\src\PlayInfoDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing PlayInfoDlg.ui...
InputDir=.\src
InputPath=.\src\PlayInfoDlg.ui
InputName=PlayInfoDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing PlayInfoDlg.ui...
InputDir=.\src
InputPath=.\src\PlayInfoDlg.ui
InputName=PlayInfoDlg

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
# Begin Source File

SOURCE=.\src\PresentationDataDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing PresentationDataDlg.ui...
InputDir=.\src
InputPath=.\src\PresentationDataDlg.ui
InputName=PresentationDataDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing PresentationDataDlg.ui...
InputDir=.\src
InputPath=.\src\PresentationDataDlg.ui
InputName=PresentationDataDlg

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
# Begin Source File

SOURCE=.\src\ProjectorControlDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ProjectorControlDlg.ui...
InputDir=.\src
InputPath=.\src\ProjectorControlDlg.ui
InputName=ProjectorControlDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ProjectorControlDlg.ui...
InputDir=.\src
InputPath=.\src\ProjectorControlDlg.ui
InputName=ProjectorControlDlg

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
# Begin Source File

SOURCE=.\src\SoundInfoDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing SoundInfoDlg.ui...
InputDir=.\src
InputPath=.\src\SoundInfoDlg.ui
InputName=SoundInfoDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing SoundInfoDlg.ui...
InputDir=.\src
InputPath=.\src\SoundInfoDlg.ui
InputName=SoundInfoDlg

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
# End Group
# Begin Source File

SOURCE=.\src\CreateMovieDlg.ui

!IF  "$(CFG)" == "mindia - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing CreateMovieDlg.ui...
InputDir=.\src
InputPath=.\src\CreateMovieDlg.ui
InputName=CreateMovieDlg

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

!ELSEIF  "$(CFG)" == "mindia - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing CreateMovieDlg.ui...
InputDir=.\src
InputPath=.\src\CreateMovieDlg.ui
InputName=CreateMovieDlg

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
