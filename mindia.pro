PROJECT			= mindia
TEMPLATE        = app
CONFIG          += qt warn_on release thread
#CONFIG          += qt warn_on debug thread
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
				  miniini.h \
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
				  minexception.h \
				  misctools.h \
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
				  miniini.cpp \
				  rolleicom.cpp \
				  wavfile.cpp \
				  mincmdproc.cpp \
				  timerjobproc.cpp \
				  dyngraphop.cpp \
				  qtmtlock.cpp \
				  misctools.cpp \
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
#win32:TMAKE_CFLAGS 	= -GX -MD
win32:TMAKE_CXXFLAGS 	= -GX -MD
win32:DEFINES			= QT_DLL _DEBUG

#../mad-0.14.2b/libmad/timer.o ../mad-0.14.2b/libmad/bit.o ../mad-0.14.2b/libmad/libmad.a ../mad-0.14.2b/libmad/timer.o ../mad-0.14.2b/libmad/bit.o ../mad-0.14.2b/libmad/libmad.a
