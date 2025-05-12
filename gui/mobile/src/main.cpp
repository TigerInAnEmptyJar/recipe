#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

int main(int argc, char** argv)
{
  QCoreApplication::setOrganizationName("Tiegerzahn ltd.");
  QCoreApplication::setOrganizationDomain("Tiegerzahn.com");
  QGuiApplication app(argc, argv);

  QUrl const url(QStringLiteral("qrc:/main.qml"));

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
