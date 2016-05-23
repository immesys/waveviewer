TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    waveviewer.cpp

RESOURCES += qml.qrc

include(qtlibbw/bosswave.pri)
include(material/material.pri)

QTPLUGIN += BOSSWAVE

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
HEADERS += \
    waveviewer.h \
    real_version.h

DISTFILES +=
