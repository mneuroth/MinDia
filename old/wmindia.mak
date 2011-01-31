#
# start makefile for Windows
#
# see the file mindiaall.mak for all possible targets !
#
# possible options:
#	debug=1
#	python=1
#

MKDIR		= mkdir
CDDIR		= cd
DEL		= del
DEL_FORCE	= del /Q
COPY		= copy 
ECHO		= echo
ZIP		= zip

BIN_ZIP		= mindia_bin.zip

#PYTHON_INC 	= d:/Programme/Python20/include
#PYTHON_LIB 	= d:/Programme/Python20/libs/python20.lib

SOURCE_DIR		= .
SRC_SCRIPT_DIR	= $(SOURCE_DIR)\scripts
SRC_DATA_DIR	= $(SOURCE_DIR)\data
RELEASE_DIR		= .\Release
BINDIST_DIR		= .\bindist
SCRIPT_DIR		= $(BINDIST_DIR)\scripts
IMAGE_DIR		= $(BINDIST_DIR)\images
MUSIC_DIR		= $(BINDIST_DIR)\music
DATA_DIR		= $(BINDIST_DIR)\data
QT_DIR			= d:\programme\qt
#PYTHON_DIR		= d:\programme\Python20
#PYTHON_DIR		= e:\home\Entwicklung\projects\OPEN\Python-2.2\pcbuild
PYTHON_DIR		= e:\home\Entwicklung\projects\OPEN\Python-2.2.2\pcbuild
SYSTEM_DIR		= d:\windows\system32

# here comes the autoconf section...

TMAKE		= tmake

!if "$(TMAKE)"==""
NO_TMAKE_FOUND	= _no_tmake
!else
NO_TMAKE_FOUND	=
!endif

!ifdef debug
TMAKEFLAGS 	= "CONFIG+=debug"
!else
TMAKEFLAGS 	= 
!endif

!ifdef python
WITH_PYTHON	= _with_python
!else
WITH_PYTHON	= 
!endif

# *********************************************************** 
#
# pass all the work to the makefile, which does the real work
#
!include mindiaall.mak
