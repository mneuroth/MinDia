; -- mindia.iss --
; install script for mindia application using Inno Setup Application

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=MinDia
AppVerName=MinDia Version 0.97.3
DefaultDirName={pf}\mindia
DefaultGroupName=MinDia
UninstallDisplayIcon={app}\mindia.exe
LicenseFile=COPYING
ShowLanguageDialog=yes
DisableProgramGroupPage=no

[Files]
Source: "release\mindia.exe"; DestDir: "{app}"; Components: main
Source: "release\minsrv.dll"; DestDir: "{app}"; Components: main
Source: "release\gendev.dll"; DestDir: "{app}"; Components: main
Source: "release\mindiapyc.dll"; DestDir: "{app}"; Components: script
Source: "E:\home\Entwicklung\projects\OPEN\Python-2.2.2\PCbuild\python22.dll"; DestDir: "{app}"; Components: script
;Source: "D:\Programme\qt\bin\qt-mt230nc.dll"; DestDir: "{app}"; Components: main
Source: "D:\Programme\qt3\bin\qt-mtnc321.dll"; DestDir: "{app}"; Components: main
;Source: "D:\windows\system32\msvcp60.dll";  DestDir: "{sys}"; Flags: restartreplace uninsneveruninstall
;Source: "D:\windows\system32\msvcrt.dll";   DestDir: "{sys}"; Flags: restartreplace uninsneveruninstall
Source: "D:\windows\system32\msvcrt.dll"; DestDir: "{app}"; Components: main
Source: "D:\windows\system32\msvcp60.dll"; DestDir: "{app}"; Components: main
Source: "mindia_de.qm"; DestDir: "{app}"; Components: main
Source: "mindia_nl.qm"; DestDir: "{app}"; Components: main
Source: "README"; DestDir: "{app}"; Components: main
Source: "INSTALL"; DestDir: "{app}"; Components: main
Source: "HISTORY"; DestDir: "{app}"; Components: main
Source: "COPYING"; DestDir: "{app}"; Components: main
Source: "BUGS"; DestDir: "{app}"; Components: main
Source: "AUTHORS"; DestDir: "{app}"; Components: main
Source: "D:\Programme\Inno Setup 4\license.txt"; DestDir: "{app}"; Components: main
Source: "mindia_en.html"; DestDir: "{app}"; Components: help\english
Source: "mindia_de.html"; DestDir: "{app}"; Components: help\german
Source: "mindia_nl.html"; DestDir: "{app}"; Components: help\netherland
Source: "scripts\*.py"; DestDir: "{app}\scripts"; Components: script
;Source: "data\example_nz.dia"; DestDir: "{app}\data"; Components: examples
;Source: "music\songd_32b.mp3"; DestDir: "{app}\music"; Components: examples
;Source: "images\*.jpg"; DestDir: "{app}\images"; Components: examples
Source: "readme.txt"; DestDir: "{app}"; Flags: isreadme; Components: main

[Components]
Name: "main"; Description: "Main Files"; Types: full compact custom; Flags: fixed; Languages: en nl;
Name: "script"; Description: "Script Support"; Types: full; Languages: en nl;
;Name: "examples"; Description: "Examples"; Types: full; Languages: en nl;
Name: "help"; Description: "Help Files"; Types: full; Languages: en nl;
Name: "help\english"; Description: "English"; Types: full; Languages: en nl;
Name: "help\german"; Description: "German"; Types: full; Languages: en nl;
Name: "help\netherland"; Description: "Dutch"; Types: full; Languages: en nl;

Name: "main"; Description: "Hauptdateien"; Types: full compact custom; Flags: fixed; Languages: de;
Name: "script"; Description: "Skript Unterstützung"; Types: full; Languages: de;
;Name: "examples"; Description: "Beispiele"; Types: full; Languages: de;
Name: "help"; Description: "Hilfedateien"; Types: full; Languages: de;
Name: "help\german"; Description: "deutsch"; Types: full; Languages: de;
Name: "help\english"; Description: "englisch"; Types: full; Languages: de;
Name: "help\netherland"; Description: "niederländisch"; Types: full; Languages: de;

;[Types]
;Name: "full"; Description: "Full installation"
;Name: "compact"; Description: "Compact installation"
;Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Dirs]
; ggf. in User-Documents ? {userappdata} --> gibt dann Probleme mit *.qm und *.html Dateien !
Name: "{app}\scripts"
Name: "{app}\images"
Name: "{app}\music"
Name: "{app}\data"

[Registry]
; Position der Ini-Datei eintragen
Root: HKCU; Subkey: "Software\mindia"; ValueType: string; ValueName: "inifile"; ValueData: "{app}"; Flags: uninsdeletekey
; Datei-Extension registrieren
Root: HKCR; Subkey: ".dia"; ValueType: string; ValueName: ""; ValueData: "MinDia"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "MinDia"; ValueType: string; ValueName: ""; ValueData: "MinDia Show"; Flags: uninsdeletekey
Root: HKCR; Subkey: "MinDia\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\mindia.exe,0"
Root: HKCR; Subkey: "MinDia\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\mindia.exe"" ""%1"""; Languages: en;
Root: HKCR; Subkey: "MinDia\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\mindia.exe"" ""-de"" ""%1"""; Languages: de;
Root: HKCR; Subkey: "MinDia\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\mindia.exe"" ""-nl"" ""%1"""; Languages: nl;

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

