zip -u %1 src/* minsrv/* mindiapyc/* gendev/* scripts/* zaurus/* 
zip -u %1 *.html *.ico *.po *.pro *.qm *.dsp *.dsw *.bat *.iss *.ini *.m4 *.specs
zip -u %1 mindiaall.mak wmindia.mak lmindia.mak *.mak Makefile 
zip -u %1 AUTHORS COPYING HISTORY INSTALL BUGS README readme.txt example.txt 
zip -u %1 *.txt *.in configure installapp dotgz setqt2 setqt3 jpg/inc/* jpg/jpgdlib/* jpg/lib/* -x moc_*

rem zip -u examples.zip *.dia *.bmp *.jpg
