TEMPLATE	= app
#CONFIG		= qt warn_on debug
CONFIG		= qt warn_on release
HEADERS		= zmindia.h \
              sysdep.h \
              wavfile.h \
              writexml.h \
              minutils.h \
              minisound.h \
              soundinfo.h \
              rolleicom.h \
              miniini.h \
              mincmdproc.h \
              diaprojector.h \
              diainfo.h \
              diapresentation.h \
              doccontroler.h \
              pcdlgimpl.h \
              diainfodlgimpl.h \
              mp3file.h \
			  configplayerdlgimpl.h\
              configdlgimpl.h
SOURCES		= zmain.cpp \
              zmindia.cpp \
              wavfile.cpp \
              writexml.cpp \
              minutils.cpp \
              minisound.cpp \
              soundinfo.cpp \
              rolleicom.cpp \
              miniini.cpp \
              mincmdproc.cpp \
              diaprojector.cpp \
              diainfo.cpp \
              diapresentation.cpp \
              doccontroler.cpp \
              pcdlgimpl.cpp \
              diainfodlgimpl.cpp \
			  configplayerdlgimpl.cpp\
              configdlgimpl.cpp \
              mp3file.cpp \
              sysdep1.c
INCLUDEPATH	+= $(QPEDIR)/include
DEPENDPATH	+= $(QPEDIR)/include
MOC_DIR					= zmoc
unix:OBJECTS_DIR		= ztmp
#LIBS            += -lqpe
LIBS            += -lqpe -lstdc++ -lpthread
#TMAKE_CXXFLAGS 	= -fexceptions
DEFINES		= _POSIX QWS ZAURUS HAVE_CONFIG_H LOCALEDIR=\"/usr/local/share/locale\"
INTERFACES	= SaveAsDlg.ui AboutDlg.ui ZConfigurationDlg.ui ZProjectorControlDlg.ui ZDiaInfoDlg.ui ZConfigPlayerDlg.ui
TARGET		= zmindia



