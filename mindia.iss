; -- mindia.iss --
; install script for mindia application using Inno Setup Application

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=MinDia
AppVerName=MinDia Version 0.97.2
DefaultDirName={pf}\mindia
DefaultGroupName=MinDia
UninstallDisplayIcon={app}\mindia.exe
LicenseFile=COPYING
ShowLanguageDialog=yes
DisableProgramGroupPage=no

[Files]
Source: "release\mindia.exe"; DestDir: "{app}"
Source: "release\minsrv.dll"; DestDir: "{app}"
Source: "release\gendev.dll"; DestDir: "{app}"
Source: "release\mindiapyc.dll"; DestDir: "{app}"
Source: "D:\Programme\qt\bin\qt-mt230nc.dll"; DestDir: "{app}"
Source: "E:\home\Entwicklung\projects\OPEN\Python-2.2.2\PCbuild\python22.dll"; DestDir: "{app}"
;Source: "D:\windows\system32\msvcrt.dll"; DestDir: "{app}"
;Source: "D:\windows\system32\msvcp60.dll"; DestDir: "{app}"
Source: "mindia_de.qm"; DestDir: "{app}"
Source: "mindia_nl.qm"; DestDir: "{app}"
Source: "README"; DestDir: "{app}"
Source: "INSTALL"; DestDir: "{app}"
Source: "HISTORY"; DestDir: "{app}"
Source: "COPYING"; DestDir: "{app}"
Source: "BUGS"; DestDir: "{app}"
Source: "AUTHORS"; DestDir: "{app}"
Source: "D:\Programme\Inno Setup 4\license.txt"; DestDir: "{app}"
Source: "mindia_en.html"; DestDir: "{app}"
Source: "mindia_de.html"; DestDir: "{app}"
Source: "scripts\*.py"; DestDir: "{app}\scripts"
Source: "data\example_nz.dia"; DestDir: "{app}\data"
Source: "images\*.jpg"; DestDir: "{app}\images"
Source: "readme.txt"; DestDir: "{app}"; Flags: isreadme

[Dirs]
; ggf. in User-Documents ? {userappdata} --> gibt dann Probleme mit *.qm und *.html Dateien !
Name: "{app}\scripts"
Name: "{app}\images"
Name: "{app}\music"
Name: "{app}\data"

[Registry]
Root: HKCU; Subkey: "Software\mindia"; ValueType: string; ValueName: "inifile"; ValueData: "{app}"

[UninstallDelete]
Type: files; Name: "{app}\.mindia.ini"
Type: files; Name: "{app}\scripts\*.pyc"

[Tasks]
Name: desktopicon; Description: "create a desktop icon"; Languages: en;
Name: desktopicon; Description: "Icon auf der Arbeitsfläche anlegen"; Languages: de;
Name: desktopicon; Description: "Icon op de desktop anlegen"; Languages: nl;
; GroupDescription: "Additional icons:"
;Name: startmenu; Description: "Create an &entry in the startmenu"; GroupDescription: "Additional icons:"

[Icons]
Name: "{group}\MinDia"; Filename: "{app}\mindia.exe"; WorkingDir: "{app}"; Parameters: "-en"; Languages: en;
Name: "{group}\MinDia"; Filename: "{app}\mindia.exe"; WorkingDir: "{app}"; Parameters: "-de"; Languages: de;
Name: "{group}\MinDia"; Filename: "{app}\mindia.exe"; WorkingDir: "{app}"; Parameters: "-nl"; Languages: nl;
Name: "{userdesktop}\MinDia"; Filename: "{app}\mindia.exe"; WorkingDir: "{app}"; Tasks: desktopicon; Parameters: "-en"; Languages: en;
Name: "{userdesktop}\MinDia"; Filename: "{app}\mindia.exe"; WorkingDir: "{app}"; Tasks: desktopicon; Parameters: "-de"; Languages: de;
Name: "{userdesktop}\MinDia"; Filename: "{app}\mindia.exe"; WorkingDir: "{app}"; Tasks: desktopicon; Parameters: "-nl"; Languages: nl;
Name: "{group}\Uninstall MinDia"; Filename: "{uninstallexe}"

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:German.isl"
Name: "nl"; MessagesFile: "compiler:Dutch.isl"

