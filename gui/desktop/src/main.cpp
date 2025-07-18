#include "ingredient_model.hpp"
#include "plan_model.hpp"
#include "recipe_model.hpp"
#include "shopping_list_model.hpp"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSortFilterProxyModel>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

int main(int argc, char** argv)
{
  QCoreApplication::setOrganizationName("Tiegerzahn ltd.");
  QCoreApplication::setOrganizationDomain("Tiegerzahn.com");
  QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());
  QGuiApplication app(argc, argv);

  recipe::gui::ingredient_model ingredients;
  ingredients.loadLast();
  ingredients.setSortRole(recipe::gui::ingredient_model::IngredientRoles::category_role);
  ingredients.setDynamicSortFilter(true);
  ingredients.sort(0);

  recipe::gui::recipe_model recipes;
  recipes.setFinder(std::bind(&recipe::gui::ingredient_model::findIngredient, &ingredients,
                              std::placeholders::_1));
  recipes.loadLast();
  recipes.sort(0);

  recipe::gui::plan_model plan(
      std::bind(&recipe::gui::recipe_model::findRecipe, &recipes, std::placeholders::_1));
  plan.loadLast();

  recipe::gui::shopping_list_model shopping;
  shopping.loadLast();
  shopping.setPlan(&plan);

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
  engine.rootContext()->setContextProperty("shopping", &shopping);
  engine.load(url);

  return app.exec();
}
