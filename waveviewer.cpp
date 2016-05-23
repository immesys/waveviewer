#include "waveviewer.h"
#include "allocations.h"
#include <QResource>
#include <QTemporaryFile>
#include <QVariant>
#include "material/src/plugin.h"
#include "material/src/core/device.h"
#include "material/src/core/units.h"

#include "real_version.h"

class MaterialRegisterHelper {

public:
    MaterialRegisterHelper(const char *uri) {
        qmlRegisterSingletonType<Device>(uri, 0, 1, "Device", Device::qmlSingleton);
        qmlRegisterUncreatableType<Units>(uri, 0, 3, "Units", QStringLiteral("Units can only be used via the attached property."));
    }
};

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
    qDebug() << "qmlsingleton called";
    return WaveViewer::instance();
}
WaveViewer *WaveViewer::instance()
{
    static MaterialRegisterHelper registerHelper("Material");
    static WaveViewer *wv = new WaveViewer();
    return wv;
}

void WaveViewer::switchapp(PMessage app_msg, PayloadObject* app)
{
    m_app_msg = app_msg;
    m_app_po = app;
    m_app_f = new QTemporaryFile();
    if (m_app_f->open())
    {
        m_app_f->write(app->content(), app->length());
        m_app_f->flush();
        bool res = QResource::registerResource(m_app_f->fileName(),"/app");
       // QQmlComponent *newApp = new QQmlComponent(m_engine, QUrl("qrc:/app/app/src/main.qml"), QQmlComponent::Asynchronous);
       // newApp->
        m_engine->load(QUrl("qrc:/app/src/main.qml"));
    }
    else
    {
        fatal(QString("Could not create temporary file"));
    }
   /* bool res = QResource::registerResource("loaded.rcc","/app");
    qDebug() << "res result: " << res;
    m_engine->load(QUrl("qrc:/app/app/src/main.qml"));*/
   /*
    f->open(QFile::WriteOnly);
    f->write(app->content(), app->length());
    f->close();*/
    //delete f;


}
void WaveViewer::appLoadComplete(QObject *obj, const QUrl &url)
{
    auto robjz = m_engine->rootObjects();
    if (robjz.length() > 1)
    {
        QQuickItem* qi = qobject_cast<QQuickItem*>(obj);
        QVariant v;
        v.setValue<QQuickItem*>(qi);
        QMetaObject::invokeMethod(robjz[0], "loadMain", Q_ARG(QVariant, v));
        m_app_loaded = true;
    }
    else
    {
        //Initial window load
        //This is set by the publishing script
#ifdef REAL_VERSION
        robjz[0]->setProperty("title", QString("Wavelet Viewer v%1").arg(REAL_VERSION));
#else
        robjz[0]->setProperty("title", QString("Wavelet Viewer v%1.%2.x").arg(VER_MAJOR, VER_MINOR));
#endif
    }
    /*if (obj != nullptr)
    {
        QObject *win = m_engine->findChild<QObject*>("appwin");
        qDebug() << "win was: " << win;
        QMetaObject::invokeMethod(win, "loadMain", Q_ARG(QObject*, obj));
    }
    else
    {
        qDebug("wass nil0");
    }*/
}

void WaveViewer::loadWavelet(QString uri)
{
    if (m_app_loaded) {
        fatal(QString("Application already loaded!"));
        return;
    }
    if (!uri.endsWith("/")) {
        uri += "/";
    }
    uri += "i.wavelet/signal/app";
    bw->query(uri, [&](QString err, QList<PMessage> results){
        if (!err.isEmpty()) {
            fatal(QString("Could not load wavelet: %1").arg(err));
            return;
        }
        if (results.length() == 0) {
            fatal(QString("Could not find wavelet"));
            return;
        }
        auto m = results[0];
        auto poz = m->FilterPOs(bwpo::num::Wavelet);
        if (poz.length() == 0) {
            fatal(QString("Could not find wavelet PO"));
            return;
        }
        switchapp(m, poz[0]);

        //Otherwise, go forth and do things
        qDebug("Would have gone forth and done things");

    });
}
