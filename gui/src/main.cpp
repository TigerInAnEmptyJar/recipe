#include "ingredient_model.hpp"
#include "plan_model.hpp"
#include "recipe_model.hpp"

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
  ingredients.loadLast();
  recipe::gui::recipe_model recipes;
  recipes.setFinder(std::bind(&recipe::gui::ingredient_model::findIngredient, &ingredients,
                              std::placeholders::_1));
  recipes.loadLast();
  recipe::gui::plan_model plan(
      std::bind(&recipe::gui::recipe_model::findRecipe, &recipes, std::placeholders::_1));
  plan.loadLast();

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
  engine.rootContext()->setContextProperty("recipes", &recipes);
  engine.rootContext()->setContextProperty("plan", &plan);
  engine.load(url);

  return app.exec();
}
