TEMPLATE	= app
#CONFIG		= qt warn_on debug
CONFIG		= qt warn_on release
HEADERS		= zaurus/zmindia.h \
              src/sysdep.h \
              src/wavfile.h \
              src/writexml.h \
              src/minutils.h \
              src/minisound.h \
              src/soundinfo.h \
              src/rolleicom.h \
              src/miniini.h \
              src/mincmdproc.h \
              src/diaprojector.h \
              src/diainfo.h \
              src/diapresentation.h \
              src/doccontroler.h \
              src/pcdlgimpl.h \
              src/diainfodlgimpl.h \
              src/mp3file.h \
			  src/configplayerdlgimpl.h\
			  src/playinfodlgimpl.h \
              src/dyngraphop.h \
              src/misctools.h \
              src/configdlgimpl.h
SOURCES		= zaurus/zmain.cpp \
              zaurus/zmindia.cpp \
              src/wavfile.cpp \
              src/writexml.cpp \
              src/minutils.cpp \
              src/minisound.cpp \
              src/soundinfo.cpp \
              src/rolleicom.cpp \
              src/miniini.cpp \
              src/mincmdproc.cpp \
              src/diaprojector.cpp \
              src/diainfo.cpp \
              src/diapresentation.cpp \
              src/doccontroler.cpp \
              src/pcdlgimpl.cpp \
              src/diainfodlgimpl.cpp \
			  src/configplayerdlgimpl.cpp\
              src/configdlgimpl.cpp \
			  src/playinfodlgimpl.cpp \
              src/dyngraphop.cpp \
              src/misctools.cpp \
              src/mp3file.cpp \
              src/sysdep1.c
INCLUDEPATH	+= $(QPEDIR)/include ./minsrv ./gendev ./src ./zaurus
DEPENDPATH	+= $(QPEDIR)/include
MOC_DIR					= zmoc
unix:OBJECTS_DIR		= ztmp
#LIBS            += -lqpe
LIBS            += -lqpe -lstdc++ -lpthread
#TMAKE_CXXFLAGS 	= -fexceptions
DEFINES		= _POSIX QWS ZAURUS HAVE_CONFIG_H LOCALEDIR=\"/usr/local/share/locale\"
INTERFACES	= zaurus/SaveAsDlg.ui \
              zaurus/AboutDlg.ui \
              zaurus/ZConfigurationDlg.ui \
              zaurus/ZProjectorControlDlg.ui \
              zaurus/ZDiaInfoDlg.ui \
              zaurus/ZPlayInfoDlg.ui \
              zaurus/ZConfigPlayerDlg.ui
TARGET		= zmindia



