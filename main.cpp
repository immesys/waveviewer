#include <QGuiApplication>
#include <QQmlApplicationEngine>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroid>
#endif
#include "waveviewer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);



    WaveViewer *wv = WaveViewer::instance();
    Q_UNUSED(wv)

    return app.exec();
}
