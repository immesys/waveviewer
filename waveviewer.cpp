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
#include <QDir>
class MaterialRegisterHelper {

public:
    MaterialRegisterHelper(const char *uri) {
        qmlRegisterSingletonType<Device>(uri, 0, 1, "Device", Device::qmlSingleton);
        qmlRegisterUncreatableType<Units>(uri, 0, 3, "Units", QStringLiteral("Units can only be used via the attached property."));
    }
};

WaveViewer::WaveViewer() : QObject(),
    m_active(false),
    m_app_msg(nullptr),
    m_app_po(nullptr),
    m_app_f(nullptr),
    m_app_loaded(false)
{
    qmlRegisterSingletonType<WaveViewer>("WaveViewer", 1, 0, "WV", &WaveViewer::qmlSingleton);
    loadFavorites();
    intended_uri = "";
#ifdef Q_OS_ANDROID
QAndroidJniObject activity = QtAndroid::androidActivity();
if (activity.isValid()) {
    QAndroidJniObject intent = activity.callObjectMethod("getIntent", "()Landroid/content/Intent;");
    if (intent.isValid()) {
        QAndroidJniObject data = intent.callObjectMethod("getDataString", "()Ljava/lang/String;");
        if (data.isValid()) {
            intended_uri = data.toString();
            intended_uri = intended_uri.right(cs.size()-6);
        }
    }
}
#endif
    m_engine = new QQmlApplicationEngine();
    m_engine->addImportPath(":/.");
    bw = BW::instance();
    QObject::connect(bw, &BW::agentChanged, this, &WaveViewer::agentChanged);
    QObject::connect(m_engine, &QQmlApplicationEngine::objectCreated, this, &WaveViewer::appLoadComplete);
    auto ent = getUsersEntity();
    m_agent_conn = false;
    if (ent.length() > 0)
    {
        m_has_ent = true;
        bw->connectAgent(ent);
    }
    else
    {
        m_has_ent = false;
    }
    QMetaObject::invokeMethod(m_engine,"load",Qt::QueuedConnection, Q_ARG(QUrl,QUrl(QStringLiteral("qrc:/main.qml"))));
}
void WaveViewer::fatal(QString err)
{
   qFatal("fatal error: %s", err.toStdString().c_str());
}

void WaveViewer::loadFavorites()
{
    QString favpath = QStandardPaths::locate(QStandardPaths::AppDataLocation, "favorites.bin");
    if (favpath.length() == 0)
    {
        qDebug() << "could not find path";
        favorites = QStringList();
        return;
    }
    QFile f(favpath);
    if (!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "could not open favorites file";
        favorites = QStringList();
        return;
    }
    QDataStream ds(&f);
    ds >> this->favorites;
    qDebug() << "loaded" << this->favorites;
    return;
}

void WaveViewer::saveFavorites()
{
    QString favpath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir(favpath).mkpath(".");//::mkpath(favpath);
    favpath += "/favorites.bin";
    qDebug() << "writing to" << favpath;
    QFile f(favpath);
    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug() << "could not open favorites file";
        return;
    }
    QDataStream ds(&f);
    ds << this->favorites;
    return;
}

QStringList WaveViewer::getRecentURIs()
{
    qDebug() << "no big deal brah";
    return favorites;
}
void WaveViewer::removeRecentURI(QString uri)
{
    QStringList nlist;
    foreach(auto s, favorites)
    {
        if (s != uri) nlist.append(s);
    }
    favorites = nlist;
    saveFavorites();
}
void WaveViewer::addRecentURI(QString uri)
{
    QStringList nlist;
    nlist.append(uri);
    foreach(auto s, favorites)
    {
        if (s != uri) nlist.append(s);
    }
    favorites = nlist;
    qDebug() << "saving" << nlist;
    saveFavorites();
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
        m_engine->load(QUrl("qrc:/app/src/main.qml"));
    }
    else
    {
        fatal(QString("Could not create temporary file"));
    }
}
void WaveViewer::appLoadComplete(QObject *obj, const QUrl &url)
{
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
        addRecentURI(m_app_uri);
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
