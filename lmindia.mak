#
# start makefile for Linux
#
# see the file mindiaall.mak for all possible targets !
#
# possible options:
#	debug=1
#	python=1
#

# *** the autoconf section ***

LIBPYTHON 		= -lpython2.2 -lpthread -lm -lutil -ldl 
PYTHONLIB 		= -L/usr/lib/python2.2/config
PYTHONINC 		= -I/usr/include/python2.2
PYTHONMODDIR	= /usr/lib/python2.2
python_incdir	= /usr/include/python2.2

LIB_QT			= -lqt-mt  $(LIBPNG) -lXext $(LIB_X11) $(LIBSM) -lpthread
LIB_X11 		= -lX11 $(LIBSOCKET)

qt_includes 	= /usr/lib/qt3/include
kde_qt_verstring= QT_VERSION >= 0x030100

ac_found_tmake	= yes
ac_tmake		= /usr/lib/qt3/bin/qmake

ac_default_prefix	= /usr/local

# *****************************

MKDIR		= mkdir
CDDIR		= cd
DEL			= rm
DEL_FORCE	= rm -f
LN_SYMB		= ln -s
COPY		= cp
ECHO		= echo
ZIP			= zip

BIN_ZIP		= mindia_bin.zip

# for Linux (SuSE 7.3)
#PYTHON_INC 	= /usr/include/python2.1
#PYTHON_LIB 	= /usr/lib/python2.1/config/libpython2.1.a

# use now values from autoconf...
PYTHON_INC	= $(python_incdir)
PYTHON_LIB	= "$(LIBPYTHON) $(PYTHONLIB)"

SOURCE_DIR		= .
SRC_SCRIPT_DIR	= $(SOURCE_DIR)/scripts
RELEASE_DIR		= 			# not needed !
BINDIST_DIR		= ./bindist
SCRIPT_DIR		= $(BINDIST_DIR)/scripts
QT_DIR			= 			# not needed !
PYTHON_DIR		= 			# not needed !
SYSTEM_DIR		= 			# not needed !

#INSTALL_EXE_DIR		= /usr/local/bin
#INSTALL_DLL_DIR		= /usr/local/lib
#INSTALL_SHARE_DIR	= /usr/local/share/mindia

INSTALL_EXE_DIR		= $(ac_default_prefix)/bin
INSTALL_DLL_DIR		= $(ac_default_prefix)/lib
INSTALL_SHARE_DIR	= $(ac_default_prefix)/share/mindia



# here comes the autoconf section...

# TODO: this has to be set via autoconf (qmake for Qt 3.0.x)
TMAKE		= $(ac_tmake)

ifeq "$(TMAKE)" ""
NO_TMAKE_FOUND	= _no_tmake
else
NO_TMAKE_FOUND	=
endif

ifdef debug
TMAKEFLAGS 	= "CONFIG+=debug"
else
TMAKEFLAGS 	=
endif

# use python if found with autoconf !
ifneq "$(PYTHON_INC)" ""
WITH_PYTHON	= _with_python
else
WITH_PYTHON	=
endif

# on linux the python modul has to be linked to the application !
ifneq "$(PYTHON_INC)" ""
LIBMINDIAPYC_SO = libmindiapyc.so
else
LIBMINDIAPYC_SO = 
endif

#bindist:
#	$(ECHO) Sorry, bindist not supportet under Linux
#	exit

# ***********************************************************
#
# pass all the work to the makefile, which does the real work
#
include mindiaall.mak
