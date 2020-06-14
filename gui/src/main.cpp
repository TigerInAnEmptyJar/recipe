#include "ingredient_model.hpp"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

int main(int argc, char** argv)
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QCoreApplication::setOrganizationName("Tiegerzahn ltd.");
  QCoreApplication::setOrganizationDomain("Tiegerzahn.com");
  QGuiApplication app(argc, argv);

  recipe::gui::ingredient_model ingredients;
  ingredients.load(QUrl("/tmp/ingredients.json"));

  QUrl const url(QStringLiteral("qrc:/main.qml"));

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.rootContext()->setContextProperty("ingredients", &ingredients);
  engine.load(url);

  return app.exec();
}
