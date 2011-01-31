Summary: mindia, a program for creating slide shows.
Name: mindia
Version: 0.98.0
Release: 0
License: GPL
#Group: Productivity/Other
Group: Utilities/Text
Source: mindia-0.98.0.tar.gz
#BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Prefix: /usr


%description
Mindia is an application to create and show slide shows,
control slide shows with a rollei projector and create
movies.

Homepage: http://www.mneuroth.de/privat/mindi/index.html

Author:   Michael Neuroth (michael.neuroth@freenet.de)

%prep
#%setup -b 0
%setup

%build
qmake -unix PREFIX=/usr -r mindia.pro CONFIG+=release
make

%install
make install

%files
/usr/bin/mindia
/usr/share/applications/mindia.desktop
/usr/share/pixmaps/mindia_64x64.png
/usr/share/mindia/mindia_de.qm
/usr/share/mindia/mindia_nl.qm
#TODO: ggf. diese daten noch in das *.pro file fuer Installation aufnehemen !!!
#/usr/share/mindia/README
#/usr/share/mindia/BUGS
#/usr/share/mindia/AUTHORS
#/usr/share/mindia/COPYING
#/usr/share/mindia/HISTORY
#/usr/share/mindia/INSTALL
#/usr/share/doc/packages/mindia/INSTALL
