#ifndef OPENURLCLIENT_H
#define OPENURLCLIENT_H

#include <QObject>

class UrlIntent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlSelected)
public:
    explicit UrlIntent(QObject *parent = 0, QObject *root = 0);
    QString url() const;
    static UrlIntent* getInstance();

public slots:
    void setUrl(const QString &url);

signals:
    void urlSelected(const QString &url);

private:
    QString m_url;
    QObject* m_root;
    static UrlIntent* m_instance;
};

#endif
