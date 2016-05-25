TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    waveviewer.cpp \
    openurlclient.cpp

RESOURCES += qml.qrc
RESOURCES +=

include(qtlibbw/bosswave.pri)
include(material/material.pri)

QTPLUGIN += BOSSWAVE

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
HEADERS += \
    waveviewer.h \
    real_version.h \
    openurlclient.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

OTHER_FILES += \
    android/src/io/bw2/OpenUrlClient.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
