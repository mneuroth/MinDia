PROJECT			= mindia
TEMPLATE        = app
CONFIG          += qt warn_on release thread stl exceptions
#CONFIG          += qt warn_on debug thread stl exceptions
HEADERS         = src/diainfodlgimpl.h \
				  src/playinfodlgimpl.h \
				  src/comlogimpl.h \
				  src/pddlgimpl.h \
				  src/configdlgimpl.h \
				  src/configplayerdlgimpl.h\
				  src/sndinfodlgimpl.h \
				  src/pcdlgimpl.h \
				  src/commentdlgimpl.h \
				  src/eventmapdlgimpl.h \
				  src/dyntextdlgimpl.h \
				  src/iocontainer.h \
				  src/minisound.h \
				  src/soundinfo.h \
				  src/applscriptenv.h \
                  src/diainfo.h \
                  src/diapresentation.h \
                  src/diaprojector.h \
                  src/hitem.h \
                  src/doccontroler.h \
                  src/hitemview.h \
				  src/timelineview.h \
				  src/timelineitem.h \
				  src/timelineaxis.h \
                  src/mindiawindow.h \
				  src/writexml.h \
				  src/minutils.h \
				  src/minlog.h \
				  src/rolleicom.h \
				  src/wavfile.h \
				  src/mincmdproc.h \
				  src/timerjobproc.h \
				  src/dyngraphop.h \
				  src/diacallback.h \
				  src/minexception.h \
				  src/iscript.h \
				  src/qtmtlock.h \
				  src/minhandle.h \
				  src/misctools.h \
				  src/appconfig.h \
				  src/osdep2.h
SOURCES         = src/diainfodlgimpl.cpp \
				  src/playinfodlgimpl.cpp \
				  src/comlogimpl.cpp \
				  src/pcdlgimpl.cpp \
				  src/configdlgimpl.cpp \
  				  src/configplayerdlgimpl.cpp\
				  src/pddlgimpl.cpp \
				  src/sndinfodlgimpl.cpp \
				  src/commentdlgimpl.cpp \
				  src/eventmapdlgimpl.cpp \
				  src/dyntextdlgimpl.cpp \
				  src/minisound.cpp \
				  src/soundinfo.cpp \
				  src/applscriptenv.cpp \
                  src/diainfo.cpp \
                  src/diapresentation.cpp \
                  src/diaprojector.cpp \
                  src/hitem.cpp \
                  src/doccontroler.cpp \
                  src/hitemview.cpp \
				  src/timelineview.cpp \
				  src/timelineitem.cpp \
  				  src/timelineaxis.cpp \
                  src/mindiawindow.cpp \
				  src/writexml.cpp \
                  src/minutils.cpp \
				  src/rolleicom.cpp \
				  src/wavfile.cpp \
				  src/mincmdproc.cpp \
				  src/timerjobproc.cpp \
				  src/dyngraphop.cpp \
				  src/qtmtlock.cpp \
				  src/misctools.cpp \
				  src/appconfig.cpp \
                  src/main.cpp
INTERFACES      = src/DiaInfoDlg.ui \
				  src/PlayInfoDlg.ui \
				  src/ComLoggingDlg.ui \
				  src/ProjectorControlDlg.ui \
				  src/PresentationDataDlg.ui\
				  src/EventMapDlg.ui \
				  src/ConfigurationDlg.ui\
				  src/ConfigPlayerDlg.ui\
				  src/SoundInfoDlg.ui \
				  src/CommentDlg.ui \
				  src/HelpDlg.ui \
				  src/LicenseDlg.ui \
				  src/AboutExtDlg.ui \
				  src/DynTextDlg.ui \
				  src/EnterValueDlg.ui
TARGET					= mindia
MOC_DIR					= moc
unix:OBJECTS_DIR		= tmp
unix:TMAKE_UIC			= $(QTDIR)/bin/uic
unix:LIBS       		+= -lpthread libminsrv.so $(LIBMINDIAPYC_SO)
unix:HEADERS			+= src/mp3file.h
unix:SOURCES			+= src/sysdep1.c \
						  src/mp3file.cpp
unix:DEFINES    		= _POSIX HAVE_CONFIG_H LOCALEDIR=\"/usr/local/share/locale\"
unix:INCLUDEPATH   		= ./gendev ./minsrv
win32:TMAKE_UIC			= $(QTDIR)\bin\uic.exe
win32:TMAKE_CXXFLAGS 	= -GX -MD
win32-borland:TMAKE_CXXFLAGS 	=
win32:DEFINES			= QT_DLL QT_NO_DEBUG QT_THREAD_SUPPORT QT_NON_COMMERCIAL
#_DEBUG
win32:LIBS				+= minsrv.lib

