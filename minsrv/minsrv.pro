PROJECT			= minsrv
TEMPLATE		= lib
#CONFIG          = warn_on dll release thread stl exceptions
CONFIG          = warn_on static thread stl exceptions
QT              += qt3support
#CONFIG          = warn_on dll debug thread stl exceptions
HEADERS         = minbase.h \
				  miniini.h \
				  dllbase.h
SOURCES         = minbase.cpp \
				  miniini.cpp \
				  dllbase.cpp
TARGET			= minsrv
unix:OBJECTS_DIR		= tmp
#unix:LIBS				+= -ldl
win32:TMAKE_CXXFLAGS	= -GX -MD
win32-borland:TMAKE_CXXFLAGS	= 
win32:DEFINES			= QT_DLL QT_NO_DEBUG QT_THREAD_SUPPORT QT_NON_COMMERCIAL 
#_DEBUG


debug {
    CONFIG -= release
}

macx {
CONFIG += x86 ppc
}
    
