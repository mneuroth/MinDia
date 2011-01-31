; -- mindia.iss --
; install script for an example slide show for the mindia application using Inno Setup Application

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=MinDia-Examples
AppVerName=MinDia-Examples
DefaultDirName={pf}\mindia
OutputBaseFileName=Example001
DefaultGroupName=MinDia
;UninstallDisplayIcon={app}\mindia.exe
ShowLanguageDialog=yes
;DisableProgramGroupPage=no

[Files]
Source: "data\example_nz.dia"; DestDir: "{app}\data";
Source: "music\songd_32b.mp3"; DestDir: "{app}\music";
Source: "images\*.jpg"; DestDir: "{app}\images";
Source: "example.txt"; DestDir: "{app}"; Flags: isreadme;

[Dirs]
Name: "{app}\scripts"
Name: "{app}\images"
Name: "{app}\music"
Name: "{app}\data"

[Icons]
Name: "{group}\Uninstall Examples for MinDia"; Filename: "{uninstallexe}"; Languages: en;
Name: "{group}\Uninstall Beispiele für MinDia"; Filename: "{uninstallexe}"; Languages: de;
Name: "{group}\Uninstall Voorbeelds voor MinDia"; Filename: "{uninstallexe}"; Languages: nl;

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:German.isl"
Name: "nl"; MessagesFile: "compiler:Dutch.isl"

