TEMPLATE = subdirs
#SUBDIRS  = minsrv \
#           gendev \
#           src
SUBDIRS = src

lessThan(QT_MAJOR_VERSION, 5): SUBDIRS += qt-qtserialport

#CONFIG += c++11

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/src/eu/licentia/necessitas/industrius/QtActivity.java \
    android/src/eu/licentia/necessitas/industrius/QtApplication.java \
    android/src/eu/licentia/necessitas/industrius/QtLayout.java \
    android/src/eu/licentia/necessitas/industrius/QtSurface.java \
    android/src/eu/licentia/necessitas/ministro/IMinistro.aidl \
    android/src/eu/licentia/necessitas/ministro/IMinistroCallback.aidl \
    android/src/eu/licentia/necessitas/mobile/QtAndroidContacts.java \
    android/src/eu/licentia/necessitas/mobile/QtCamera.java \
    android/src/eu/licentia/necessitas/mobile/QtFeedback.java \
    android/src/eu/licentia/necessitas/mobile/QtLocation.java \
    android/src/eu/licentia/necessitas/mobile/QtMediaPlayer.java \
    android/src/eu/licentia/necessitas/mobile/QtSensors.java \
    android/src/eu/licentia/necessitas/mobile/QtSystemInfo.java

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/qextserialport/release/ -lqextserialport
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/qextserialport/debug/ -lqextserialport
#else:symbian: LIBS += -lqextserialport
#else:unix: LIBS += -L$$OUT_PWD/qextserialport/ -lqextserialport

#INCLUDEPATH += $$PWD/qt-qtserialport
#DEPENDPATH += $$PWD/qt-qtserialport

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/qextserialport/release/qextserialport.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/qextserialport/debug/qextserialport.lib
#else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/qextserialport/libqextserialport.a


