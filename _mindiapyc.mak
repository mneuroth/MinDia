#############################################################################
# Makefile for building: libmindiapyc.so.1.0.0
# Generated by qmake (1.04a) (Qt 3.1.1) on: Fri Feb 20 23:21:06 2004
# Project:  mindiapyc.pro
# Template: lib
# Command: $(QMAKE) -o _mindiapyc.mak mindiapyc.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -O2 -march=i586 -mcpu=i686 -fmessage-length=0 -fPIC -DNO_DEBUG -Wall -W -O2 -march=i586 -mcpu=i686 -fmessage-length=0 -fPIC -DNO_DEBUG -D_REENTRANT -fPIC  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -O2 -march=i586 -mcpu=i686 -fmessage-length=0 -fPIC -DNO_DEBUG -Wall -W -O2 -march=i586 -mcpu=i686 -fmessage-length=0 -fPIC -DNO_DEBUG -D_REENTRANT -fPIC  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I$(QTDIR)/mkspecs/default -I. -I$(PYTHON_INC) -Iminsrv -Isrc -I/usr/include -I$(QTDIR)/include -Imoc/
LINK     = g++
LFLAGS   = -shared -Wl,-soname,libmindiapyc.so.1
LIBS     = $(SUBLIBS) -L/usr/lib/ -L$(QTDIR)/lib/ -L/usr/X11R6/lib/ $(PYTHON_LIB) libminsrv.so -lqt-mt -lXext -lX11 -lm -lpthread
AR       = ar cqs
RANLIB   = 
MOC      = $(QTDIR)/bin/moc
UIC      = $(QTDIR)/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = tmp/

####### Files

HEADERS = mindiapyc/scriptfcn.h \
		mindiapyc/scriptdlgimpl.h
SOURCES = mindiapyc/_minutils.cpp \
		mindiapyc/mindiapy.cpp \
		mindiapyc/scriptfcn.cpp \
		mindiapyc/scriptdlgimpl.cpp \
		mindiapyc/mindia_wrap.cpp
OBJECTS = tmp/_minutils.o \
		tmp/mindiapy.o \
		tmp/scriptfcn.o \
		tmp/scriptdlgimpl.o \
		tmp/mindia_wrap.o \
		tmp/ScriptDlg.o
FORMS = mindiapyc/ScriptDlg.ui
UICDECLS = mindiapyc/ScriptDlg.h
UICIMPLS = mindiapyc/ScriptDlg.cpp
SRCMOC   = moc/moc_scriptfcn.cpp \
		moc/moc_scriptdlgimpl.cpp \
		moc/moc_ScriptDlg.cpp
OBJMOC = tmp/moc_scriptfcn.o \
		tmp/moc_scriptdlgimpl.o \
		tmp/moc_ScriptDlg.o
DIST	   = mindiapyc.pro
QMAKE_TARGET = mindiapyc
DESTDIR  = 
TARGET   = libmindiapyc.so.1.0.0
TARGETA	= libmindiapyc.a
TARGETD	= libmindiapyc.so.1.0.0
TARGET0	= libmindiapyc.so
TARGET1	= libmindiapyc.so.1
TARGET2	= libmindiapyc.so.1.0

first: all
####### Implicit rules

.SUFFIXES: .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: _mindiapyc.mak  $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(SUBLIBS) 
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)



staticlib: $(TARGETA)

$(TARGETA): $(UICDECLS) $(OBJECTS) $(OBJMOC)
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS) $(OBJMOC)

mocables: $(SRCMOC)

$(MOC): 
	( cd $(QTDIR)/src/moc ; $(MAKE) )

_mindiapyc.mak: mindiapyc.pro  $(QTDIR)/mkspecs/default/qmake.conf 
	$(QMAKE) -o _mindiapyc.mak mindiapyc.pro
qmake: 
	@$(QMAKE) -o _mindiapyc.mak mindiapyc.pro

dist: 
	@mkdir -p tmp/mindiapyc && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) tmp/mindiapyc/ && ( cd `dirname tmp/mindiapyc` && $(TAR) mindiapyc.tar mindiapyc && $(GZIP) mindiapyc.tar ) && $(MOVE) `dirname tmp/mindiapyc`/mindiapyc.tar.gz . && $(DEL_FILE) -r tmp/mindiapyc

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:
	-$(DEL_FILE) $(UICIMPLS) $(UICDECLS)

yaccclean:
lexclean:
clean: mocclean uiclean
	-$(DEL_FILE) $(OBJECTS) 
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)
	-$(DEL_FILE) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA)


FORCE:

####### Compile

tmp/_minutils.o: mindiapyc/_minutils.cpp src/minutils.cpp \
		src/minutils.h \
		src/osdep2.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/_minutils.o mindiapyc/_minutils.cpp

tmp/mindiapy.o: mindiapyc/mindiapy.cpp mindiapyc/scriptfcn.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/mindiapy.o mindiapyc/mindiapy.cpp

tmp/scriptfcn.o: mindiapyc/scriptfcn.cpp mindiapyc/scriptfcn.h \
		mindiapyc/scriptdlgimpl.h \
		mindiapyc/ScriptDlg.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/scriptfcn.o mindiapyc/scriptfcn.cpp

tmp/scriptdlgimpl.o: mindiapyc/scriptdlgimpl.cpp mindiapyc/scriptdlgimpl.h \
		mindiapyc/ScriptDlg.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/scriptdlgimpl.o mindiapyc/scriptdlgimpl.cpp

tmp/mindia_wrap.o: mindiapyc/mindia_wrap.cpp mindiapyc/mindia_wrap.c
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/mindia_wrap.o mindiapyc/mindia_wrap.cpp

mindiapyc/ScriptDlg.h: mindiapyc/ScriptDlg.ui 
	$(UIC) mindiapyc/ScriptDlg.ui -o mindiapyc/ScriptDlg.h

mindiapyc/ScriptDlg.cpp: mindiapyc/ScriptDlg.h mindiapyc/ScriptDlg.ui 
	$(UIC) mindiapyc/ScriptDlg.ui -i ScriptDlg.h -o mindiapyc/ScriptDlg.cpp

tmp/ScriptDlg.o: mindiapyc/ScriptDlg.cpp mindiapyc/ScriptDlg.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/ScriptDlg.o mindiapyc/ScriptDlg.cpp

tmp/moc_scriptfcn.o: moc/moc_scriptfcn.cpp mindiapyc/scriptfcn.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_scriptfcn.o moc/moc_scriptfcn.cpp

tmp/moc_scriptdlgimpl.o: moc/moc_scriptdlgimpl.cpp mindiapyc/scriptdlgimpl.h mindiapyc/ScriptDlg.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_scriptdlgimpl.o moc/moc_scriptdlgimpl.cpp

tmp/moc_ScriptDlg.o: moc/moc_ScriptDlg.cpp mindiapyc/ScriptDlg.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_ScriptDlg.o moc/moc_ScriptDlg.cpp

moc/moc_scriptfcn.cpp: $(MOC) mindiapyc/scriptfcn.h
	$(MOC) mindiapyc/scriptfcn.h -o moc/moc_scriptfcn.cpp

moc/moc_scriptdlgimpl.cpp: $(MOC) mindiapyc/scriptdlgimpl.h
	$(MOC) mindiapyc/scriptdlgimpl.h -o moc/moc_scriptdlgimpl.cpp

moc/moc_ScriptDlg.cpp: $(MOC) mindiapyc/ScriptDlg.h
	$(MOC) mindiapyc/ScriptDlg.h -o moc/moc_ScriptDlg.cpp

####### Install

install: all 

uninstall: 

