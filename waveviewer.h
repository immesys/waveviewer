#ifndef WAVEVIEWER_H
#define WAVEVIEWER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <libbw.h>

#define VER_MAJOR 1
#define VER_MINOR 0

class WaveViewer : public QObject
{
public:
    WaveViewer() : QObject(), m_active(false)
    {
        qmlRegisterSingletonType<WaveViewer>("WaveViewer", VER_MAJOR, VER_MINOR, "WaveViewer", &WaveViewer::qmlSingleton);
        m_engine = new QQmlApplicationEngine();
        bw = BW::instance();
        QObject::connect(bw, &BW::agentChanged, this, &WaveViewer::agentChanged);
        bw->connectAgent("localhost",28589);
    }
    static QObject *qmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);
    static WaveViewer *instance();
    void fatal(QString err);
private slots:
    void agentChanged();
private:
    BW* bw;
    //Agent is connected and window displayed
    bool m_active;
    QQmlApplicationEngine *m_engine;
};

#endif // WAVEVIEWER_H
