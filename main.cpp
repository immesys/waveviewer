#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <libbw.h>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    auto bw = BW::instance();
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QObject::connect(bw, &BW::agentChanged, [&]
    {
        qDebug() << "agent connected fired";
        bw->setEntityFile("/home/immesys/.ssh/michael.key", [&](QString s)
        {
            qDebug() << "got callback";
            if (!s.isEmpty())
            {
                qFatal("Could not set entity file: %s", s.toStdString().c_str());
            }
            qDebug() << "invokign engine load";
            engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

         /*   bw->publishMsgPack("/foo/bar", 5, {{"hello", 5}, {"foobar", "hiiii"}}, [](Status s)
            {
                qDebug() << "TRIG DONE" << s.msg();
            });*/

        });
    });
    bw->connectAgent("localhost",28589);


    return app.exec();
}
