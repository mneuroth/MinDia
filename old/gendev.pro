PROJECT			= gendev
TEMPLATE		= lib
CONFIG          = warn_on dll release thread
#CONFIG          = warn_on dll debug thread
HEADERS         = gendev/igendev.h
SOURCES         = gendev/gendev.cpp
TARGET			= gendev
unix:OBJECTS_DIR		= tmp
unix:LIBS				+= -ldl libminsrv.dylib
unix:INCLUDEPATH   		= ./minsrv ./src
win32:TMAKE_CXXFLAGS	= -GX -MD
win32:DEFINES			= QT_DLL _DEBUG

macx {
CONFIG += x86 ppc
}
    