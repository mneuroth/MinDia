PROJECT			= gendev
TARGET			= gendev
TEMPLATE		= lib
CONFIG          = warn_on static thread stl exceptions
#CONFIG          = warn_on dll debug thread
HEADERS         = igendev.h
SOURCES         = gendev.cpp
LIBS				+= ../minsrv/libminsrv.a
INCLUDEPATH   		+= ../minsrv ../src

debug {
    CONFIG -= release
}

macx {
CONFIG += x86 ppc
}
    
