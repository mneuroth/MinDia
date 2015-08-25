#include(qt-qtserialport/src/serialport/serialport-lib.pri)

PROJECT			= mindia
TARGET			= mindia
TEMPLATE        = app
#CONFIG          += qt warn_on release thread stl exceptions
#for debug:
CONFIG          += qt warn_on thread stl exceptions
QT              = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia printsupport
lessThan(QT_MAJOR_VERSION, 5): QT += phonon
!android {
QT              += serialport
}
else
{
}
#QT              -=network
#QT              -=sql
#QT              -=svg
#QT              -=script
#QT              -=qml
#CONFIG          -= debug

win32:RC_FILE = mindia.rc

HEADERS         = diainfodlgimpl.h \
                  playinfodlgimpl.h \
                  comlogimpl.h \
                  pddlgimpl.h \
                  configdlgimpl.h \
                  sndinfodlgimpl.h \
                  pcdlgimpl.h \
                  commentdlgimpl.h \
                  dyntextdlgimpl.h \
                  helpdlgimpl.h \
                  iocontainer.h \
                  minisound.h \
                  soundinfo.h \
                  applscriptenv.h \
                  diainfo.h \
                  diapresentation.h \
                  diaprojector.h \
                  hitem.h \
                  doccontroler.h \
                  hitemview.h \
                  timelineview.h \
                  timelineitem.h \
                  timelineaxis.h \
                  mindiawindow.h \
                  writexml.h \
                  minutils.h \
                  minlog.h \
                  rolleicom.h \
                  mincmdproc.h \
                  timerjobproc.h \
                  dyngraphop.h \
                  diacallback.h \
                  minexception.h \
                  iscript.h \
                  qtmtlock.h \
                  minhandle.h \
                  misctools.h \
                  appconfig.h \
                  CreateMovieDlg4.h \
                  ../gendev/igendev.h \
                  ../minsrv/minbase.h \
                  ../minsrv/dllbase.h \
                  playinfographicsview.h \
                  dynamictextitem.h \
                  playscreenediting.h \
                  graphicsitemresizeablerect.h

SOURCES         = diainfodlgimpl.cpp \
                  playinfodlgimpl.cpp \
                  comlogimpl.cpp \
                  pcdlgimpl.cpp \
                  configdlgimpl.cpp \
                  pddlgimpl.cpp \
                  sndinfodlgimpl.cpp \
                  commentdlgimpl.cpp \
                  dyntextdlgimpl.cpp \
                  helpdlgimpl.cpp \
                  minisound.cpp \
                  soundinfo.cpp \
                  applscriptenv.cpp \
                  diainfo.cpp \
                  diapresentation.cpp \
                  diaprojector.cpp \
                  hitem.cpp \
                  doccontroler.cpp \
                  hitemview.cpp \
                  timelineview.cpp \
                  timelineitem.cpp \
                  timelineaxis.cpp \
                  mindiawindow.cpp \
                  writexml.cpp \
                  minutils.cpp \
                  rolleicom.cpp \
                  mincmdproc.cpp \
                  timerjobproc.cpp \
                  dyngraphop.cpp \
                  qtmtlock.cpp \
                  misctools.cpp \
                  appconfig.cpp \
                  main.cpp \
                  CreateMovieDlg4.cpp \
                  ../gendev/gendev.cpp \
                  ../minsrv/minbase.cpp \
                  ../minsrv/dllbase.cpp \
                  playinfographicsview.cpp \
                  dynamictextitem.cpp \
                  playscreenediting.cpp \
                  graphicsitemresizeablerect.cpp

FORMS       = CreateMovieDlg4.ui \
                  PresentationDataDlg4.ui \
                  ProjectorControlDlg4.ui \
                  ComLoggingDlg4.ui \
                  ConfigurationDlg4.ui \
                  PlayInfoDlg4.ui \
                  CommentDlg4.ui  \
                  SoundInfoDlg4.ui \
                  DiaInfoDlg4.ui \
                  AboutExtDlg4.ui \
                  HelpDlg4.ui \
                  DynTextDlg4.ui

TRANSLATIONS = mindia_de.ts \
    mindia_nl.ts

INCLUDEPATH   		= ../gendev ../minsrv

macx {
#    UI_DIR = .ui
#    MOC_DIR = .moc
#    OBJECTS_DIR = .obj
#    CONFIG += x86 #\
#        ppc

    # QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5u.sdk
    # QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
#    target.path = /Applications
#    INSTALLS = target
#    utilities.path = Contents/Resources
#    utilities.files += ../src/mindia_de.qm  #\
#        ../src/mindia_nl.qm # \

    # qt_de.qm
#    QMAKE_BUNDLE_DATA += utilities
    #INSTALLS += utilities
    ICON = mindia_new.icns
    QMAKE_INFO_PLIST = Info.plist

    # binary ffmpeg for mac --> http://ffmpegmac.net
    files.path = Contents/MacOS/files
    files.files += ../ffmpeg/mac/ffmpeg

    QMAKE_BUNDLE_DATA += files
    INSTALLS += files
}

lessThan(QT_MAJOR_VERSION, 5) {
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qt-qtserialport/src/serialport/release/ -lqtserialport
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qt-qtserialport/src/serialport/debug/ -lqtserialportd
else:unix:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qt-qtserialport/src/serialport/ -lQtSerialPort
else:unix:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qt-qtserialport/src/serialport/ -lQtSerialPort_debug

INCLUDEPATH += $$PWD/../qt-qtserialport/include
DEPENDPATH += $$PWD/../qt-qtserialport/src/serialport
}

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt-qtserialport/src/serialport/release/libqtserialport.a
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt-qtserialport/src/serialport/debug/libqtserialportd.a
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qt-qtserialport/src/serialport/libqtserialport.a

RESOURCES += \
    mindia.qrc



