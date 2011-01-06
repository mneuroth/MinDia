ls -1 *.h *.cpp *.ui *.ts *.qm *.po *.pro *.qrc *.mak *.i *.iss *.spec *.m4 *.in *. AUTHORS BUGS COPYING HISTORY INSTALL README *.html *.txt *.clp *.desktop *.sh *.bat *.inf *.dia *.ini *.png *.icns *.ico *.xcf *.rc dist/* >file.lst
ls -d -1 gendev/* >>file.lst
ls -d -1 minsrv/* >>file.lst
ls -d -1 mindiapyc/* >>file.lst
ls -d -1 qextserialport/* >>file.lst
ls -d -1 qextserialport/src/* >>file.lst
ls -d -1 src/* >>file.lst
ls -d -1 scripts/* >>file.lst
ls -d -1 zaurus/* >>file.lst