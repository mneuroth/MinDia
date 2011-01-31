#!/bin/sh
echo "make binary distribution of mindia for Debian/Ubuntu (*.deb):"
echo "create source archive"
./make_src_tar.sh
cp /usr/share/qt4/translations/qt_de.qm .
echo "call qmake"
qmake -unix PREFIX=/usr mindia.pro CONFIG+=release
#echo "make application"
#make   # will be done by making deb package in next step
echo "make *.deb"
dh_make -s -c gpl -e michael.neuroth@freenet.de -f mindia-0.98.0.tar.gz
cp dist/rules debian/rules
cp dist/copyright debian/copyright
cp dist/control debian/control
dpkg-buildpackage -rfakeroot -b
echo "clean up"
rm qt_de.qm
      