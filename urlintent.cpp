#include <jni.h>
#include <QVariant>

#include "urlintent.h"

UrlIntent* UrlIntent::m_instance = NULL;

UrlIntent::OpenUrlClient(QObject *parent, QObject *root)
    : QObject(parent)
{
    // using signal slot approach
    //connect(this, SIGNAL(urlSelected(QString)), this, SLOT(setUrl(QString)));
    m_instance = this;
    m_root = root;
}

UrlIntent* UrlIntent::getInstance()
{
    if (!m_instance)
        m_instance = new OpenUrlClient;
    return m_instance;
}

void UrlIntent::setUrl(const QString &url)
{
    if (m_url == url)
        return;
    QObject *urlText = m_root->findChild<QObject*>("urlText");
    if (urlText)
        urlText->setProperty("text", "You clicked on: " + url);
    m_url = url;
}

QString UrlIntent::url() const
{
    return m_url;
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
  Java_io_bw2_UrlIntent_setUrl(JNIEnv *env,
                                jobject obj,
                                jstring url)
{
    const char *urlStr = env->GetStringUTFChars(url, NULL);
    // using signal slot approach
    //emit OpenUrlClient::getInstance()->urlSelected(urlStr);

    // using instance
    UrlIntent::getInstance()->setUrl(urlStr);
    env->ReleaseStringUTFChars(url, urlStr);
    return;
}

#ifdef __cplusplus
}
#endif
