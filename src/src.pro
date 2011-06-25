PROJECT			= mindia
TARGET			= mindia
TEMPLATE        = app
#CONFIG          += qt warn_on release thread stl exceptions
#for debug:
CONFIG          += qt warn_on thread stl exceptions
QT              = core gui
QT              -=network
QT              -=sql
QT              -=svg
QT              -=script
#CONFIG          -= debug

HEADERS         = diainfodlgimpl.h \
				  playinfodlgimpl.h \
				  comlogimpl.h \
				  pddlgimpl.h \
				  configdlgimpl.h \
				  configplayerdlgimpl.h\
				  sndinfodlgimpl.h \
				  pcdlgimpl.h \
				  commentdlgimpl.h \
				  dyntextdlgimpl.h \
				  helpdlgimpl.h \
				  iocontainer.h \
				  minisound.h \
				  soundinfo.h \
				  applscriptenv.h \
                  diainfo.h \
                  diapresentation.h \
                  diaprojector.h \
                  hitem.h \
                  doccontroler.h \
                  hitemview.h \
				  timelineview.h \
				  timelineitem.h \
				  timelineaxis.h \
                  mindiawindow.h \
				  writexml.h \
				  minutils.h \
				  minlog.h \
				  rolleicom.h \
				  wavfile.h \
				  mincmdproc.h \
				  timerjobproc.h \
				  dyngraphop.h \
				  diacallback.h \
				  minexception.h \
				  iscript.h \
				  qtmtlock.h \
				  minhandle.h \
				  misctools.h \
				  appconfig.h \
                  osdep2.h \
                  CreateMovieDlg4.h \
                  ../gendev/igendev.h \
                  ../minsrv/minbase.h \
                  ../minsrv/dllbase.h
SOURCES         = diainfodlgimpl.cpp \
				  playinfodlgimpl.cpp \
				  comlogimpl.cpp \
				  pcdlgimpl.cpp \
				  configdlgimpl.cpp \
  				  configplayerdlgimpl.cpp\
				  pddlgimpl.cpp \
				  sndinfodlgimpl.cpp \
				  commentdlgimpl.cpp \
				  dyntextdlgimpl.cpp \
				  helpdlgimpl.cpp \
				  minisound.cpp \
				  soundinfo.cpp \
				  applscriptenv.cpp \
                  diainfo.cpp \
                  diapresentation.cpp \
                  diaprojector.cpp \
                  hitem.cpp \
                  doccontroler.cpp \
                  hitemview.cpp \
				  timelineview.cpp \
				  timelineitem.cpp \
  				  timelineaxis.cpp \
                  mindiawindow.cpp \
				  writexml.cpp \
                  minutils.cpp \
				  rolleicom.cpp \
				  wavfile.cpp \
				  mincmdproc.cpp \
				  timerjobproc.cpp \
				  dyngraphop.cpp \
				  qtmtlock.cpp \
				  misctools.cpp \
				  appconfig.cpp \
                  main.cpp \
                  CreateMovieDlg4.cpp \
                  ../gendev/gendev.cpp \
                  ../minsrv/minbase.cpp \
                  ../minsrv/dllbase.cpp

FORMS       = CreateMovieDlg4.ui \
                  PresentationDataDlg4.ui \
                  ProjectorControlDlg4.ui \
                  ComLoggingDlg4.ui \
                  ConfigurationDlg4.ui \
                  ConfigPlayerDlg4.ui \
                  PlayInfoDlg4.ui \
                  CommentDlg4.ui  \
                  SoundInfoDlg4.ui \
                  DiaInfoDlg4.ui \
                  AboutExtDlg4.ui \
                  HelpDlg4.ui \
                  DynTextDlg4.ui

MOC_DIR					= moc
#macx:LIBS               += -lpthread ./libminsrv.dylib $(LIBMINDIAPYC_SO)
#macx:LIBS               += -lpthread ../minsrv/libminsrv.a $(LIBMINDIAPYC_SO)
OBJECTS_DIR		        = obj
#unix:TMAKE_UIC			= $(QTDIR)/bin/uic
#unix:LIBS       		+= -lpthread libminsrv.so $(LIBMINDIAPYC_SO)
unix:HEADERS			+= mp3file.h
unix:SOURCES			+= mp3file.cpp
                            # sysdep1.c
unix:DEFINES    		= _POSIX HAVE_CONFIG_H LOCALEDIR=\"/usr/local/share/locale\"
INCLUDEPATH   		= ../gendev ../minsrv
#win32:TMAKE_UIC			= $(QTDIR)\bin\uic.exe
#win32:TMAKE_CXXFLAGS 	= -GX -MD
#win32-borland:TMAKE_CXXFLAGS 	=
#win32:DEFINES			= QT_DLL QT_NO_DEBUG QT_THREAD_SUPPORT QT_NON_COMMERCIAL
#_DEBUG
#win32:LIBS				+= minsrv.lib
win32:LIBS				+= libwinmm
#LIBS                += /Users/min/Documents/home/Entwicklung/projects/mindia_qt4/qextserialport/src/build/libqextserialportd.a
#LIBS                += ../qextserialport/src/build/libqextserialport.a

Debug:win32:win32:LIBS += ../qextserialport/src/build/libqextserialportd.a
debug {
    mac:LIBS += ../qextserialport/src/build/libqextserialportd.a
    unix:LIBS += ../qextserialport/src/build/libqextserialportd.a
    CONFIG -= release
}
Release:win32:win32:LIBS += ../qextserialport/src/build/libqextserialport.a
release {
    mac:LIBS += ../qextserialport/src/build/libqextserialport.a
    unix:LIBS += ../qextserialport/src/build/libqextserialport.a
}

#debug {
#    CONFIG -= release
#}

macx {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
#    CONFIG += x86 #\
#        ppc

    # QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5u.sdk
    # QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
    target.path = /Applications
    INSTALLS = target
    utilities.path = Contents/Resources
    utilities.files += ../mindia_de.qm \
        ../mindia_nl.qm # \

    # qt_de.qm
    QMAKE_BUNDLE_DATA += utilities
    #INSTALLS += utilities
    ICON = mindia_512x512.icns
    QMAKE_INFO_PLIST = Info.plist
}

