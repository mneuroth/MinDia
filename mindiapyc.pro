PROJECT			= mindiapyc
TEMPLATE		= lib
CONFIG          = qt warn_on dll release thread
#CONFIG          = qt warn_on dll debug thread
HEADERS         = mindiapyc/scriptfcn.h \
				  mindiapyc/minutils.h \
				  mindiapyc/scriptdlgimpl.h
SOURCES         = mindiapyc/mindiapy.cpp \
				  mindiapyc/scriptfcn.cpp \
				  mindiapyc/minutils.cpp \
				  mindiapyc/scriptdlgimpl.cpp \
				  mindiapyc/mindia_wrap.cpp
INTERFACES      = mindiapyc/ScriptDlg.ui
TARGET			= mindiapyc
MOC_DIR			= moc
unix:OBJECTS_DIR		= tmp
unix:LIBS				+= $(PYTHON_LIB) libminsrv.so
unix:INCLUDEPATH		+= $(PYTHON_INC)
unix:TMAKE_UIC			= $(QTDIR)/bin/uic
win32:INCLUDEPATH		+= $(PYTHON_INC)
win32:LIBS				+= $(PYTHON_LIB)
win32:DEFINES			+= QT_DLL NO_DEBUG
win32:TMAKE_UIC			= $(QTDIR)\bin\uic.exe
win32:TMAKE_CXXFLAGS	= -GX -MD
