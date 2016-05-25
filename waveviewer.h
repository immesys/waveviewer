#ifndef WAVEVIEWER_H
#define WAVEVIEWER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <libbw.h>
#include "openurlclient.h"
#include <QAndroidJniObject>
#include <QtAndroid>

#define VER_MAJOR 1
#define VER_MINOR 1

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
        OpenUrlClient *c = new OpenUrlClient(this);
        cs = "";
#ifdef Q_OS_ANDROID
    qDebug() << "SXXXX: checking activity";
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        qDebug() << "SXXXX: 1";
        QAndroidJniObject intent = activity.callObjectMethod("getIntent", "()Landroid/content/Intent;");
        qDebug() << "SXXXX: 2";
        if (intent.isValid()) {
            qDebug() << "SXXXX: 3";
            QAndroidJniObject data = intent.callObjectMethod("getDataString", "()Ljava/lang/String;");
            qDebug() << "SXXXX: 4y";
            if (data.isValid()) {
                cs = data.toString();
                cs = cs.right(cs.size()-6);
#if 0
                qDebug() << "SXXXX: 5";
                QAndroidJniObject path = data.callObjectMethod("getPath", "()Ljava/lang/String;");
                qDebug() << "SXXXX: 6";
                if (path.isValid())
                {
                    qDebug() << "SXXXX: 7";
                    // Here path.toString() returns the path of the input file
                  //  cs = path.toString();
                    //QMetaObject::invokeMethod(rootComponent, "setSourcePath", Q_ARG(QVariant, QVariant("file://" + path.toString())));
                }
                else{
                    qDebug() << "SXXXX: 8";

                }
#endif
            }
            else{
                qDebug() << "SXXXX: 9";

            }
        }
        else{
            qDebug() << "SXXXX: 10";

        }
    }
    else{
        qDebug() << "SXXXX: 11";

    }
    //QMetaObject::invokeMethod(m_engine->rootObjects()[0], "setthing", Q_ARG(QVariant, cs));
#endif
        m_engine = new QQmlApplicationEngine();
        m_engine->addImportPath(":/.");
     //   connect(c,&OpenUrlClient::urlSelected, this, [=](QString s)
     //   {
         //   cs = s;
           /* if (m_engine->rootObjects().size() > 0)
            {
                QMetaObject::invokeMethod(m_engine->rootObjects()[0], "setthing", Q_ARG(QVariant, s));
            }*/

    //    });
        bw = BW::instance();
        QObject::connect(bw, &BW::agentChanged, this, &WaveViewer::agentChanged);
        QObject::connect(m_engine, &QQmlApplicationEngine::objectCreated, this, &WaveViewer::appLoadComplete);
#ifdef Q_OS_ANDROID
        bw->connectAgent("bunker.cs.berkeley.edu",28589);
#else
        bw->connectAgent("localhost",28589);
#endif
    }
    static QObject *qmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);



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
    QString cs;
};

#endif // WAVEVIEWER_H
