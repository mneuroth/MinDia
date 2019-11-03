echo "make distribution of mindia for Windows (*.exe and Inno-Setup):"
echo "call qmake"
qmake -r mindia.pro CONFIG+=release
echo "make app"
mingw32-make Release
echo "compress executable"
upx -9 src\release\mindia.exe
echo "call Inno-Setup to create setup file"
"C:\Programme\Inno Setup 5\iscc" dist\mindia_static.iss /Fmindia_setup /O./dist
rem "C:\Programme\Inno Setup 5\iscc" dist\cliphist2.iss /Fcliphist2_setup /O./dist
