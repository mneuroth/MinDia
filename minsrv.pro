PROJECT			= minsrv
TEMPLATE		= lib
CONFIG          = warn_on dll release thread
#CONFIG          = warn_on dll debug thread
HEADERS         = minbase.h \
				  miniini.h \
				  dllbase.h
SOURCES         = minbase.cpp \
				  miniini.cpp \
				  dllbase.cpp
TARGET			= minsrv
unix:OBJECTS_DIR		= tmp
unix:LIBS				+= -ldl
win32:TMAKE_CXXFLAGS	= -GX -MD
win32:DEFINES			= QT_DLL _DEBUG
