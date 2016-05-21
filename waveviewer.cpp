#include "waveviewer.h"

void WaveViewer::fatal(QString err)
{
   qFatal(err.toStdString().c_str());
}

void WaveViewer::agentChanged()
{
    if (m_active)
    {
        fatal("Agent changed, but we are already active");
        return;
    }
    bw->setEntityFromEnviron([&](QString s)
    {
        qDebug() << "got callback";
        if (!s.isEmpty())
        {
            fatal(QString("Could not set entity file: %1").arg(s));
            return;
        }
        m_engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

     /*   bw->publishMsgPack("/foo/bar", 5, {{"hello", 5}, {"foobar", "hiiii"}}, [](Status s)
        {
            qDebug() << "TRIG DONE" << s.msg();
        });*/

    });
}

QObject *WaveViewer::qmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    return WaveViewer::instance();
}
WaveViewer *WaveViewer::instance()
{
    static WaveViewer *wv = new WaveViewer();
    return wv;
}
