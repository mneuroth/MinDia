PROJECT			= mindia
TEMPLATE        = app
CONFIG          += qt warn_on release thread stl exceptions
#CONFIG          += qt warn_on debug thread stl exceptions
HEADERS         = diainfodlgimpl.h \
				  playinfodlgimpl.h \
				  comlogimpl.h \
				  pddlgimpl.h \
				  configdlgimpl.h \
				  configplayerdlgimpl.h\
				  sndinfodlgimpl.h \
				  pcdlgimpl.h \
				  commentdlgimpl.h \
				  eventmapdlgimpl.h \
				  dyntextdlgimpl.h \
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
				  osdep2.h
SOURCES         = diainfodlgimpl.cpp \
				  playinfodlgimpl.cpp \
				  comlogimpl.cpp \
				  pcdlgimpl.cpp \
				  configdlgimpl.cpp \
  				  configplayerdlgimpl.cpp\
				  pddlgimpl.cpp \
				  sndinfodlgimpl.cpp \
				  commentdlgimpl.cpp \
				  eventmapdlgimpl.cpp \
				  dyntextdlgimpl.cpp \
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
                  main.cpp
INTERFACES      = DiaInfoDlg.ui \
				  PlayInfoDlg.ui \
				  ComLoggingDlg.ui \
				  ProjectorControlDlg.ui \
				  PresentationDataDlg.ui\
				  EventMapDlg.ui \
				  ConfigurationDlg.ui\
				  ConfigPlayerDlg.ui\
				  SoundInfoDlg.ui \
				  CommentDlg.ui \
				  HelpDlg.ui \
				  LicenseDlg.ui \
				  AboutExtDlg.ui \
				  DynTextDlg.ui \
				  EnterValueDlg.ui
TARGET					= mindia
MOC_DIR					= moc
unix:OBJECTS_DIR		= tmp
unix:TMAKE_UIC			= $(QTDIR)/bin/uic
unix:LIBS       		+= -lpthread libminsrv.so $(LIBMINDIAPYC_SO)
unix:HEADERS			+= mp3file.h
unix:SOURCES			+= sysdep1.c \
						  mp3file.cpp
unix:DEFINES    		= _POSIX HAVE_CONFIG_H LOCALEDIR=\"/usr/local/share/locale\"
#unix:INCLUDEPATH   		= .
win32:TMAKE_UIC			= $(QTDIR)\bin\uic.exe
win32:TMAKE_CXXFLAGS 	= -GX -MD
win32-borland:TMAKE_CXXFLAGS 	= 
win32:DEFINES			= QT_DLL QT_NO_DEBUG QT_THREAD_SUPPORT QT_NON_COMMERCIAL 
#_DEBUG
win32:LIBS				+= minsrv.lib

