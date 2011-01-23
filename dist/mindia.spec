Summary: cliphist2, a program for managing the clipboard history.
Name: cliphist2
Version: 1.0.0
Release: 0
License: GPL
#Group: Productivity/Other
Group: Utilities/Text
Source: cliphist2-1.0.0.tar.gz
#BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Prefix: /usr


%description
Cliphist2 is an application to show the history of the text 
content of the clipboard and to select old entries again.

Homepage: http://www.mneuroth.de/privat/cliphist2/index.html

Author:   Michael Neuroth (michael.neuroth@freenet.de)

%prep
#%setup -b 0
%setup

%build
qmake -unix PREFIX=/usr cliphist2.pro CONFIG+=release
make

%install
make install

%files
/usr/bin/cliphist2
/usr/share/applications/cliphist2.desktop
/usr/share/pixmaps/cliphist2_64x64.png
/usr/share/cliphist2/cliphist2_de.qm
/usr/share/cliphist2/cliphist2_nl.qm
#TODO: ggf. diese daten noch in das *.pro file fuer Installation aufnehemen !!!
#/usr/share/cliphist2/README
#/usr/share/cliphist2/BUGS
#/usr/share/cliphist2/AUTHORS
#/usr/share/cliphist2/COPYING
#/usr/share/cliphist2/HISTORY
#/usr/share/cliphist2/INSTALL
#/usr/share/doc/packages/cliphist2/INSTALL
