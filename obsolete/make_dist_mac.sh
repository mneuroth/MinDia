#!/bin/sh
echo "make distribution of mindia for Mac OS X (*.app and *.dmg):"
#cp /Developer/Applications/Qt/translations/qt_de.qm .
cp /Users/min/Downloads/Programmierung/qt-everywhere-opensource-src-4.7.3/translations/qt_de.qm src/qt_de.qm
echo "call qmake"
#cd src
#qmake -macx -spec macx-g++ CONFIG+=release src.pro
qmake -r -macx -spec macx-g++ CONFIG+=release CONFIG-=debug mindia.pro
echo "make app"
make
cp -r /Users/min/Downloads/Programmierung/qt-everywhere-opensource-src-4.7.3/src/gui/mac/qt_menu.nib src/mindia.app/Contents/Resources
echo "make dmg"
cd src
rm mindia.dmg
macdeployqt mindia.app -dmg
echo "clean up"
rm qt_de.qm
