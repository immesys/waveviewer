#include "waveviewer.h"
#include "allocations.h"
#include <QResource>
#include <QTemporaryFile>
#include <QVariant>
#include "material/src/plugin.h"
#include "material/src/core/device.h"
#include "material/src/core/units.h"
#include <QCoreApplication>
#include <QStandardPaths>
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
   qFatal("fatal error: %s", err.toStdString().c_str());
}

QByteArray WaveViewer::getUsersEntity()
{
    QString entitypath = QStandardPaths::locate(QStandardPaths::AppDataLocation, "defaultEntity.ent");
    if (entitypath.length() == 0)
    {
        //Try environment variable
        QByteArray a = qgetenv("BW2_DEFAULT_ENTITY");
        if (a.length() != 0) {
            entitypath = a.data();
        } else {
            return QByteArray();
        }
    }
    QFile f(entitypath);
    if (!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "could not open entity file";
        return QByteArray();
    }
    QByteArray contents = f.readAll().mid(1);
    return contents;
}

void WaveViewer::agentChanged()
{
    if (m_active)
    {
        fatal("Agent changed, but we are already active");
        return;
    }
    //Agent has connected. If we have an entity, try load the URI, otherwise
    //display the menu
    QByteArray entity = getUsersEntity();
    if (entity.length() == 0) {
        qFatal("must show menu to select entity");
    }
    bw->setEntity(entity,[&](QString s, QString vk)
    {
        if (!s.isEmpty())
        {
            fatal(QString("Could not set entity file: %1").arg(s));
            return;
        }
        //Ok we are good to go
        m_agent_conn = true;
        checkPreload();
    });
#if 0
#ifdef Q_OS_ANDROID
    QByteArray inp;
    inp.append(entity);
    QByteArray ba = QByteArray::fromBase64(inp);
    ba = ba.right(ba.size()-1);
    bw->setEntity(ba,
#else
    bw->setEntityFromEnviron(
#endif
    [&](QString s, QString vk)
    {
        qDebug() << "got callback: " << s;
        if (!s.isEmpty())
        {
            fatal(QString("Could not set entity file: %1").arg(s));
            return;
        }

    });
#endif
}

QObject *WaveViewer::qmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
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
    qDebug() << "switchapp called";
    m_app_msg = app_msg;
    m_app_po = app;
    m_app_f = new QTemporaryFile();
    if (m_app_f->open())
    {
        m_app_f->write(app->content(), app->length());
        m_app_f->flush();
        bool res = QResource::registerResource(m_app_f->fileName(),"/app");
        if (!res)
        {
            fatal("Could not load application");
        }
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
    qDebug("app load complete happened");
    if (obj == nullptr)
    {
        fatal(QString("Could not load application at %1").arg(url.toString()));
        return;
    }
    auto robjz = m_engine->rootObjects();
    if (robjz.length() > 1)
    {
        //We have loaded another application
        QQuickItem* qi = qobject_cast<QQuickItem*>(obj);
        QVariant v;
        v.setValue<QQuickItem*>(qi);
        QMetaObject::invokeMethod(robjz[0], "loadMain", Q_ARG(QVariant, v));
        m_app_loaded = true;
    }
    else
    {
        QMetaObject::invokeMethod(robjz[0], "setState", Q_ARG(QVariant, m_has_ent),Q_ARG(QVariant, m_agent_conn));
        //This is set by the publishing script
#ifdef REAL_VERSION
        robjz[0]->setProperty("title", QString("Wavelet Viewer v%1").arg(REAL_VERSION));
#else
        robjz[0]->setProperty("title", QString("Wavelet Viewer v%1.%2.x").arg(VER_MAJOR).arg(VER_MINOR));
#endif
    }
#if 0
    if (obj == nullptr)
    {
        fatal(QString("Could not load application at %1").arg(url.toString()));
        return;
    }
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
        checkPreload();
      //  QMetaObject::invokeMethod(robjz[0], "setthing", Q_ARG(QVariant, QString(cs)));
        //Initial window load
        //This is set by the publishing script
#ifdef REAL_VERSION
        robjz[0]->setProperty("title", QString("Wavelet Viewer v%1").arg(REAL_VERSION));
#else
        robjz[0]->setProperty("title", QString("Wavelet Viewer v%1.%2.x").arg(VER_MAJOR).arg(VER_MINOR));
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
#endif
}

void WaveViewer::checkPreload()
{
    if (intended_uri.length() == 0 && QCoreApplication::arguments().size() == 1)
    {
        qDebug() << "no uri passed, loading menu";
        QMetaObject::invokeMethod(m_engine->rootObjects()[0], "setState",Q_ARG(QVariant, m_has_ent),Q_ARG(QVariant, m_agent_conn));
    }
    else
    {
        if (intended_uri.length() > 0)
        {
            loadWavelet(intended_uri);
        }
        else
        {
            loadWavelet(QCoreApplication::arguments()[1]);
        }
    }
#if 0
    if (cs != "")
    {
        qDebug() << "cs non nil" << m_engine->rootObjects().size();
        QMetaObject::invokeMethod(m_engine->rootObjects()[0], "setthing", Q_ARG(QVariant, QString(cs)));
        loadWavelet(cs);
    }
    else
    {
        if (QCoreApplication::arguments().size() > 1)
        {
            qDebug() << "cs nil using arg" << m_engine->rootObjects().size();
            QMetaObject::invokeMethod(m_engine->rootObjects()[0], "setthing", Q_ARG(QVariant, QCoreApplication::arguments()[1]));
            loadWavelet(QCoreApplication::arguments()[1]);
        }
        else
        {
            qDebug() << "cs nil no arg" << m_engine->rootObjects().size();
            QMetaObject::invokeMethod(m_engine->rootObjects()[0], "setthing", Q_ARG(QVariant, QString("")));
        }
    }
#endif
}

void WaveViewer::loadWavelet(QString uri)
{
    qDebug() << "load wavelet called";
    if (m_app_loaded) {
        fatal(QString("Application already loaded!"));
        return;
    }
    if (!uri.endsWith("/")) {
        uri += "/";
    }
    m_app_uri = uri;
    uri += "i.wavelet/signal/app";
    //omg sam this is the worst api
    qDebug() << "calling query list";
    bw->queryList(uri, "",true,QList<RoutingObject*>(),QDateTime(),-1,"",false,false,[&](QString err, QList<PMessage> results){
        qDebug() << "query list returned";
        if (!err.isEmpty()) {
            fatal(QString("Could not load wavelet: %1").arg(err));
            return;
        }
        if (results.length() == 0) {
            qFatal("should do metadata search here");
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

QString WaveViewer::appURI()
{
    return m_app_uri;
}

QString WaveViewer::canonicalize(QString uri)
{
    QStringList toks = uri.split("/",QString::SkipEmptyParts);
    QStringList rtoks;
    foreach(auto t, toks)
    {
        if (t == ".")
        {
            continue;
        }
        else if (t == "..")
        {
            if (!toks.isEmpty())
            {
                rtoks.removeLast();
            }
        }
        else
        {
            rtoks.append(t);
        }
    }
    return rtoks.join("/");

}
