Summary: MinDia, a program for slide shows.
Name: mindia
Version: 0.97.3
Release: 0
Copyright: GPL
Group: Applications/Graphics
Source: mindia-0.97.3.tar.gz
%description
MinDia, a program for slide shows.

MinDia is an application for creating, modifying, and presenting multimedia slide shows.
Slide shows can contain multimedia elements like images, sounds and text. 
In addition to displaying on screen, it can control Rollei slide projectors 
(Rolleivision 35 twin digital P and Rolleivision twin msc 3x0 P) via a serial port.

Homepage: http://mindia.sourceforge.net

Author:   Michael Neuroth (mindia@users.sourceforge.net)

%prep
%setup

%build
. setqt3
./configure
make

%install
. setqt3
make install

%files
#%doc AUTHORS BUGS COPYING HISTORY INSTALL README
%dir

/usr/local/bin/mindia
/usr/local/lib/libgendev.so
/usr/local/lib/libgendev.so.1
/usr/local/lib/libgendev.so.1.0.0
/usr/local/lib/libminsrv.so
/usr/local/lib/libminsrv.so.1
/usr/local/lib/libminsrv.so.1.0.0
/usr/local/lib/libmindiapyc.so
/usr/local/lib/libmindiapyc.so.1
/usr/local/lib/libmindiapyc.so.1.0.0
/usr/local/share/mindia/
/usr/local/share/mindia/scripts/

#%pre
#su

#%post
#ldconfig
