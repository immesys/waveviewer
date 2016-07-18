#ifndef WAVEVIEWER_H
#define WAVEVIEWER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <libbw.h>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroid>
#endif

#define VER_MAJOR 1
#define VER_MINOR 4

class WaveViewer : public QObject
{
    Q_OBJECT
public:
    WaveViewer() : QObject(),
        m_active(false),
        m_app_msg(nullptr),
        m_app_po(nullptr),
        m_app_f(nullptr),
        m_app_loaded(false)
    {
        qmlRegisterSingletonType<WaveViewer>("WaveViewer", 1, 0, "WV", &WaveViewer::qmlSingleton);
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

        m_engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    }
    static QObject *qmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE QList<QString> getRecentURIs();
    Q_INVOKABLE void removeRecentURI(QString uri);

    QByteArray getUsersEntity();

    /**
     * @brief gets the singleton instance of WaveViewer
     */
    static WaveViewer *instance();

    /**
     * @brief end the application and display the given error message
     * @param err the message to display
     *
     * @since 1.1
     */
    Q_INVOKABLE void fatal(QString err);

    /**
     * @brief Loads a wavelet at a given URI
     * @param uri the URI to load
     *
     * This first checks for <uri>/i.wavelet and if that is not found it
     * checks for the metadata key "app" and will load that URI
     *
     * @since 1.0
     */
    Q_INVOKABLE void loadWavelet(QString uri);

    /**
     * @brief Returns the URI of the loaded app (
     * @return The URI, up to but not including the i.wavelet
     *
     * @ingroup qml
     * @since 1.1
     */
    Q_INVOKABLE QString appURI();

    /**
     * @brief Return a canonical form of a given URI
     * @param The URI, possibly containing ".." or "."
     * @return The URI with translated ".."s
     *
     * @ingroup qml
     * @since 1.1
     */
    QString canonicalize(QString uri);

    Q_INVOKABLE void checkPreload();
private slots:
    void agentChanged();
    void appLoadComplete(QObject *obj, const QUrl &url);
private:

    void switchapp(PMessage app_msg, PayloadObject* app);



    BW* bw;
    //Agent is connected and window displayed
    bool m_active;
    QQmlApplicationEngine *m_engine;

    PMessage m_app_msg;
    PayloadObject* m_app_po;
    QTemporaryFile *m_app_f;
    bool m_app_loaded;
    QString m_app_uri;
    QString intended_uri;
    bool m_has_ent;
    bool m_agent_conn;
};

#endif // WAVEVIEWER_H
