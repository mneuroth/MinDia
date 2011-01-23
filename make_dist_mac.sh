#!/bin/sh
echo "make distribution of mindia for Mac OS X (*.app and *.dmg):"
cp /Developer/Applications/Qt/translations/qt_de.qm .
echo "call qmake"
#cd src
#qmake -macx -spec macx-g++ CONFIG+=release src.pro
qmake -macx -spec macx-g++ CONFIG+=release -r mindia.pro
echo "make app"
make
echo "make dmg"
cd src
# rm mindia.dmg
macdeployqt mindia.app -dmg
echo "clean up"
#rm qt_de.qm
