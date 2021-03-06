
# *****************************************************
# This are the possible targets:
# *****************************************************

all:		make_make$(WITH_PYTHON)$(NO_TMAKE_FOUND) make_run$(WITH_PYTHON)

make:		make_make$(WITH_PYTHON)$(NO_TMAKE_FOUND)

compile:	make_run$(WITH_PYTHON)

install:	make_install$(WITH_PYTHON)

uninstall:	make_uninstall$(WITH_PYTHON)

clean:		make_clean$(WITH_PYTHON)

swig:		make_swig

doku:		make_doku

bindist:	make_bindist

cddist:		make_cddist

# just for testing...
test:		make_test$(WITH_PYTHON)$(NO_TMAKE_FOUND)


# *****************************************************
# WARNING: the following targets are only help targets,
#          do not use them directly !
# *****************************************************

make_make: 						minsrv_make mindia_make gendev_make

make_make_with_python: 			minsrv_make mindiapyc_make mindia_make gendev_make

make_make_no_tmake:				make_no_tmake

make_make_with_python_no_tmake:	make_no_tmake

make_run: 						minsrv_dll mindia_exe gendev_dll

make_run_with_python: 			minsrv_dll mindiapyc_dll mindia_exe gendev_dll

make_install:					minsrv_install mindia_install gendev_install post_install

make_install_with_python:		minsrv_install mindiapyc_install mindia_install gendev_install post_install

make_uninstall:					minsrv_uninstall mindia_uninstall gendev_uninstall post_uninstall

make_uninstall_with_python:		minsrv_uninstall mindiapyc_uninstall mindia_uninstall gendev_uninstall post_uninstall

make_clean:						minsrv_clean mindia_clean bindist_clean gendev_clean

make_clean_with_python:			make_clean mindiapyc_clean

# *****************************************************

make_test:
	@$(ECHO) This is a make Test target...

make_test_with_python:
	@$(ECHO) This is a make Test target with Python...

make_test_with_python_no_tmake:
	@$(ECHO) This is a make Test target with Python but without tmake...

make_no_tmake:
	@$(ECHO) WARNING: no tmake found to generate new makefiles, using old makefiles...

make_swig:
	swig -c++ -python -shadow -o mindia_wrap.c mindia.i 

make_doku:
	doxygen mindoc.ini

#
# WARNING: the bindist-target is only supported for windows
#
make_bindist_help:
	-$(MKDIR) $(BINDIST_DIR)
	-$(MKDIR) $(SCRIPT_DIR)
	-$(MKDIR) $(IMAGE_DIR)
	-$(MKDIR) $(MUSIC_DIR)
	-$(MKDIR) $(DATA_DIR)
	-$(MKDIR) $(MOVIE_DIR)
	$(COPY) $(RELEASE_DIR)\mindia.exe $(BINDIST_DIR)
	$(COPY) $(RELEASE_DIR)\minsrv.dll $(BINDIST_DIR)
	$(COPY) $(RELEASE_DIR)\gendev.dll $(BINDIST_DIR)
	$(COPY) $(RELEASE_DIR)\mindiapyc.dll $(BINDIST_DIR)
	$(COPY) $(SOURCE_DIR)\*.qm $(BINDIST_DIR)
	$(COPY) $(SOURCE_DIR)\*.html $(BINDIST_DIR)
	-$(COPY) $(SRC_SCRIPT_DIR)\*.py $(SCRIPT_DIR)
	-$(COPY) $(SRC_DATA_DIR)\*.dia $(DATA_DIR)
	$(COPY) $(SOURCE_DIR)\README $(BINDIST_DIR)
	$(COPY) $(SOURCE_DIR)\INSTALL $(BINDIST_DIR)
	$(COPY) $(SOURCE_DIR)\HISTORY $(BINDIST_DIR)
	$(COPY) $(SOURCE_DIR)\COPYING $(BINDIST_DIR)
	$(COPY) $(SOURCE_DIR)\BUGS $(BINDIST_DIR)
	$(COPY) $(SOURCE_DIR)\AUTHORS $(BINDIST_DIR)
#	-$(COPY) $(SYSTEM_DIR)\python20.dll $(BINDIST_DIR)
	-$(COPY) $(PYTHON_DIR)\python22.dll $(BINDIST_DIR)
	$(COPY) $(SYSTEM_DIR)\msvcrt.dll $(BINDIST_DIR)
	$(COPY) $(SYSTEM_DIR)\msvcp60.dll $(BINDIST_DIR)
	$(COPY) $(QT_DIR)\bin\qt-mt230nc.dll $(BINDIST_DIR)

make_bindist:	make_bindist_help
	$(CDDIR) $(BINDIST_DIR)
	$(ZIP) -u -r $(BIN_ZIP) * -x *.ini *.pyc

#
# TODO: hier ggf. in ein lib-Verzeichnis auf CD kopieren ?
#
make_cddist:	make_bindist_help
	-$(MKDIR) $(BINDIST_DIR)\lib
	-$(COPY) $(PYTHON_DIR)\..\lib\site-packages\libsip.dll $(BINDIST_DIR)\lib
	-$(COPY) $(PYTHON_DIR)\..\lib\site-packages\libqt*.pyd $(BINDIST_DIR)\lib
	-$(COPY) $(PYTHON_DIR)\..\lib\site-packages\qt*.py $(BINDIST_DIR)\lib
	-$(COPY) $(PYTHON_DIR)\..\lib\*.py $(BINDIST_DIR)\lib
	-$(COPY) $(PYTHON_DIR)\..\PCbuild\*.py $(BINDIST_DIR)\lib
	-$(COPY) $(PYTHON_DIR)\..\PCbuild\*.pyd $(BINDIST_DIR)\lib
	-$(COPY) $(PYTHON_DIR)\python.exe $(BINDIST_DIR)
	

# *****************************************************

minsrv_make:		minsrv.pro
	$(TMAKE) $(TMAKEFLAGS) minsrv.pro -o _minsrv.mak

mindia_make:		mindia.pro
	$(TMAKE) $(TMAKEFLAGS) mindia.pro -o _mindia.mak

mindiapyc_make:		mindiapyc.pro
	$(TMAKE) $(TMAKEFLAGS) mindiapyc.pro -o _mindiapyc.mak

gendev_make:		gendev.pro
	$(TMAKE) $(TMAKEFLAGS) gendev.pro -o _gendev.mak

# *****************************************************

# Remark: temp makefiles start with an underbar !

minsrv_dll:		_minsrv.mak
	$(MAKE) -f _minsrv.mak

mindia_exe:		_mindia.mak
	$(MAKE) LIBMINDIAPYC_SO=$(LIBMINDIAPYC_SO) -f _mindia.mak

mindiapyc_dll:	_mindiapyc.mak
	$(MAKE) PYTHON_INC=$(PYTHON_INC) PYTHON_LIB=$(PYTHON_LIB) -f _mindiapyc.mak

gendev_dll:		_gendev.mak
	$(MAKE) -f _gendev.mak

# *****************************************************

minsrv_clean:		_minsrv.mak
	$(MAKE) -f _minsrv.mak clean

mindia_clean:		_mindia.mak
	$(MAKE) -f _mindia.mak clean

mindiapyc_clean:	_mindiapyc.mak
	$(MAKE) -f _mindiapyc.mak clean

gendev_clean:		_gendev.mak
	$(MAKE) -f _gendev.mak clean

bindist_clean:
	-$(DEL_FORCE) $(BINDIST_DIR)\*

# *****************************************************

#
# WARNING: this installation procedure is only supported for Linux !
#

minsrv_install:		minsrv_dll
	$(COPY) libminsrv.so.1.0.0 $(INSTALL_DLL_DIR)
	-$(LN_SYMB) $(INSTALL_DLL_DIR)/libminsrv.so.1.0.0 $(INSTALL_DLL_DIR)/libminsrv.so
	-$(LN_SYMB) $(INSTALL_DLL_DIR)/libminsrv.so.1.0.0 $(INSTALL_DLL_DIR)/libminsrv.so.1

minsrv_uninstall:
	-$(DEL) $(INSTALL_DLL_DIR)/libminsrv.so.1.0.0
	-$(DEL) $(INSTALL_DLL_DIR)/libminsrv.so
	-$(DEL) $(INSTALL_DLL_DIR)/libminsrv.so.1

mindia_install:		mindia_exe
	$(COPY) mindia $(INSTALL_EXE_DIR)
	-$(MKDIR) $(INSTALL_SHARE_DIR)
	-$(MKDIR) $(INSTALL_SHARE_DIR)/scripts
	$(COPY) *.qm $(INSTALL_SHARE_DIR)
	$(COPY) *.html $(INSTALL_SHARE_DIR)
	$(COPY) scripts/*.py $(INSTALL_SHARE_DIR)/scripts
	$(COPY) README $(INSTALL_SHARE_DIR)
	$(COPY) INSTALL $(INSTALL_SHARE_DIR)
	$(COPY) HISTORY $(INSTALL_SHARE_DIR)
	$(COPY) COPYING $(INSTALL_SHARE_DIR)
	$(COPY) BUGS $(INSTALL_SHARE_DIR)
	$(COPY) AUTHORS $(INSTALL_SHARE_DIR)

mindia_uninstall:	
	-$(DEL) $(INSTALL_EXE_DIR)/mindia
	-$(DEL) -f $(INSTALL_SHARE_DIR)/*.qm
	-$(DEL) -f $(INSTALL_SHARE_DIR)/*.html 
	-$(DEL) -f $(INSTALL_SHARE_DIR)/scripts/*.py
	-$(DEL) -f $(INSTALL_SHARE_DIR)/README
	-$(DEL) -f $(INSTALL_SHARE_DIR)/INSTALL
	-$(DEL) -f $(INSTALL_SHARE_DIR)/HISTORY
	-$(DEL) -f $(INSTALL_SHARE_DIR)/COPYING
	-$(DEL) -f $(INSTALL_SHARE_DIR)/BUGS
	-$(DEL) -f $(INSTALL_SHARE_DIR)/AUTHORS
	-$(RMDIR) $(INSTALL_SHARE_DIR)/scripts
	-$(RMDIR) $(INSTALL_SHARE_DIR)

mindiapyc_install:	mindiapyc_dll
	$(COPY) libmindiapyc.so.1.0.0 $(INSTALL_DLL_DIR)
	-$(LN_SYMB) $(INSTALL_DLL_DIR)/libmindiapyc.so.1.0.0 $(INSTALL_DLL_DIR)/libmindiapyc.so
	-$(LN_SYMB) $(INSTALL_DLL_DIR)/libmindiapyc.so.1.0.0 $(INSTALL_DLL_DIR)/libmindiapyc.so.1

mindiapyc_uninstall:
	-$(DEL) $(INSTALL_DLL_DIR)/libmindiapyc.so.1.0.0 
	-$(DEL) $(INSTALL_DLL_DIR)/libmindiapyc.so
	-$(DEL) $(INSTALL_DLL_DIR)/libmindiapyc.so.1

gendev_install:		gendev_dll
	$(COPY) libgendev.so.1.0.0 $(INSTALL_DLL_DIR)
	-$(LN_SYMB) $(INSTALL_DLL_DIR)/libgendev.so.1.0.0 $(INSTALL_DLL_DIR)/libgendev.so
	-$(LN_SYMB) $(INSTALL_DLL_DIR)/libgendev.so.1.0.0 $(INSTALL_DLL_DIR)/libgendev.so.1

gendev_uninstall:
	-$(DEL) $(INSTALL_DLL_DIR)/libgendev.so.1.0.0
	-$(DEL) $(INSTALL_DLL_DIR)/libgendev.so
	-$(DEL) $(INSTALL_DLL_DIR)/libgendev.so.1

post_install:
	ldconfig

post_uninstall:
	echo done.
