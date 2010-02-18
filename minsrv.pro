PROJECT			= minsrv
TEMPLATE		= lib
#CONFIG          = warn_on dll release thread stl exceptions
CONFIG          = warn_on static release thread stl exceptions
QT              += qt3support
#CONFIG          = warn_on dll debug thread stl exceptions
HEADERS         = minsrv/minbase.h \
				  minsrv/miniini.h \
				  minsrv/dllbase.h
SOURCES         = minsrv/minbase.cpp \
				  minsrv/miniini.cpp \
				  minsrv/dllbase.cpp
TARGET			= minsrv
unix:OBJECTS_DIR		= tmp
unix:LIBS				+= -ldl
win32:TMAKE_CXXFLAGS	= -GX -MD
win32-borland:TMAKE_CXXFLAGS	= 
win32:DEFINES			= QT_DLL QT_NO_DEBUG QT_THREAD_SUPPORT QT_NON_COMMERCIAL 
#_DEBUG

macx {
CONFIG += x86 ppc
}
    