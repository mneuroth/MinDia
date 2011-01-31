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
# PYTHONMODDIR	= /usr/lib/python2.2
python_incdir	= /usr/include/python2.2

# LIB_QT			= @LIB_QT@
# LIB_X11 		= @LIB_X11@

# qt_includes 	= @qt_includes@
# kde_qt_verstring= @kde_qt_verstring@

ac_found_tmake	= yes
ac_tmake		= /usr/lib/qt3/bin/qmake

ac_default_prefix	= /usr/local
ac_install_prefix	= /usr/local

# *****************************

MKDIR		= mkdir
RMDIR		= rmdir
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
SRC_DATA_DIR	= $(SOURCE_DIR)/data
RELEASE_DIR		= 			# not needed !
BINDIST_DIR		= ./bindist
SCRIPT_DIR		= $(BINDIST_DIR)/scripts
IMAGE_DIR		= $(BINDIST_DIR)/images
MUSIC_DIR		= $(BINDIST_DIR)/music
DATA_DIR		= $(BINDIST_DIR)/data
MOVIE_DIR		= $(BINDIST_DIR)/movie
QT_DIR			= 			# not needed !
PYTHON_DIR		= 			# not needed !
SYSTEM_DIR		= 			# not needed !

#INSTALL_EXE_DIR		= /usr/local/bin
#INSTALL_DLL_DIR		= /usr/local/lib
#INSTALL_SHARE_DIR	= /usr/local/share/mindia

INSTALL_EXE_DIR		= $(ac_install_prefix)/bin
INSTALL_DLL_DIR		= $(ac_install_prefix)/lib
INSTALL_SHARE_DIR	= $(ac_install_prefix)/share/mindia



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
ifneq "$(PYTHON_INC)" "no"
WITH_PYTHON	= _with_python
else
WITH_PYTHON	=
endif

# on linux the python modul has to be linked to the application !
ifneq "$(PYTHON_INC)" "no"
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
