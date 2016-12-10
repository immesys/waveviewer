TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    waveviewer.cpp

RESOURCES += qml.qrc
RESOURCES +=

include(qtlibbw/bosswave.pri)
include(mr-plotter-qml/mrplotter.pri)
include(material/material.pri)
include(qzxing-waveviewer/source/QZXing.pri)

QTPLUGIN += BOSSWAVE

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
HEADERS += \
    waveviewer.h \
    real_version.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

LIBS += -lcrypto -lssl \
    $$PWD/../ssl/libssl.a \
    $$PWD/../ssl/libcrypto.a

OTHER_FILES +=

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/prebuilt_ssl/libcrypto.so \
        $$PWD/prebuilt_ssl/libssl.so
}
