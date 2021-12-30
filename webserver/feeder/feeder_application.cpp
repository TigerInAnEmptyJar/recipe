#include "feeder_application.hpp"

#include "io_provider.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>

#include <boost/uuid/uuid_io.hpp>

#include <iostream>
#include <filesystem>

namespace {
template <typename T>
std::shared_ptr<T> getDefaultProvider()
{
  recipe::io::io_provider provider;
  provider.setup();

  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<T>>> providers;
  if constexpr(std::is_same_v<T, recipe::io::ingredient_io>) {
    providers = provider.installed_ingredient();
  } else if constexpr (std::is_same_v<T, recipe::io::recipe_io>) {
    providers = provider.installed_recipe();
  } else if constexpr (std::is_same_v<T, recipe::io::plan_io>) {
    providers = provider.installed_plan();
  } else if constexpr (std::is_same_v<T, recipe::io::shopping_io>) {
    providers = provider.installed_shopping();
  } else {
    return {};
  }
  for (auto single : providers) {
    if (single.second.first.find("(*.json") != std::string::npos) {
      return single.second.second;
    }
  }
  return {};
}

}
namespace recipe::webserver {

feeder_application::~feeder_application() = default;

void feeder_application::start()
{
  _webAccess = std::make_unique<communication::webserver_access>("http://localhost", "8080");
  std::cout << std::left << std::setfill('.');
  _uploads.back().second();
}

void feeder_application::parseCommandline()
{
  // Define commandline
  QCommandLineOption ingredientDb{{"i", "ingredients"}, "database path for ingredients", "ingredient_path"};
  QCommandLineOption recipeDb{{"r", "recipes"}, "database path for recipes", "recipe_path"};
  QCommandLineOption plans{{"p", "plan"}, "paths for meal plans", "plan_paths"};
  QCommandLineOption shoppings{{"s", "shopping"}, "paths for shopping lists", "shopping_paths"};
  QCommandLineOption doUploadRecipes{{"R", "upload_recipes"}, "Recipes will be uploaded if this option is provided"};
  QCommandLineOption doUploadPlans{{"P", "upload_plans"}, "Meal-plans will be uploaded if this option is provided"};
  QCommandLineOption doUploadShopping{{"S", "upload_shopping"}, "Shopping-lists will be uploaded if this option is provided"};
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOptions(QList<QCommandLineOption>() << ingredientDb << recipeDb << plans << shoppings
                    << doUploadRecipes << doUploadPlans << doUploadShopping);

  parser.process(*this);
  for(auto const& a: arguments()) {
      std::cout << a.toStdString() << std::endl;
  }

  std::filesystem::path ingredientPath;
  std::filesystem::path recipePath;
  std::vector<std::filesystem::path> planPaths;
  std::vector<std::filesystem::path> shoppingPaths;

  // parse and extract arguments
  if (!parser.values(ingredientDb).isEmpty()) {
      std::cout << "\"" << parser.value(ingredientDb).toStdString() << "\"" << std::endl;
      for(auto const&s:parser.values(ingredientDb)) {
          std::cout << s.toStdString() << std::endl;
      }
      ingredientPath = parser.value(ingredientDb).toStdString();
  } else {
      std::cout << "Sorry, ingredient path is mandatory" << std::endl;
      parser.showHelp(1);
  }
  if (parser.isSet(recipeDb)) {
      recipePath = parser.value(recipeDb).toStdString();
  } else {
      std::cout << "Sorry, recipe path is mandatory" << std::endl;
      parser.showHelp(1);
  }
  if (parser.isSet(plans)) {
      for (auto const& p : parser.values(plans)) {
          planPaths.push_back(p.toStdString());
      }
  }
  if (parser.isSet(shoppings)) {
      for (auto const&s : parser.values(shoppings)) {
          shoppingPaths.push_back(s.toStdString());
      }
  }

  // read database
  //  read ingredients
  std::cout << "read ingredients from file: " << ingredientPath << " ... " << std::flush;
  auto ing = getDefaultProvider<io::ingredient_io>()->read(ingredientPath);
  if (!ing) {
      std::cout << "invalid ingredient file: " << ingredientPath << std::endl;
      quit();
      return;
  } else {
      std::cout << "read " << ing->size() << " ingredients" << std::endl;
  }
  auto ingredients = *ing;
  auto ingredientFinder = [&ingredients](auto id)->std::optional<ingredient>{
      auto in = std::find_if(std::begin(ingredients), std::end(ingredients), [&id](auto const& el){
          return el.id() == id;
      });
      if (in != std::end(ingredients)) {
          return *in;
      }
      return std::nullopt;
  };

  //  read recipes
  std::cout << "read recipes from file: " << recipePath << " ... " << std::flush;
  auto rec = getDefaultProvider<io::recipe_io>()->read(recipePath, ingredientFinder);
  if (!rec) {
      std::cout << "invalid recipe file: " << recipePath << std::endl;
      quit();
      return;
  } else {
      std::cout << "read " << rec->size() << " recipes" << std::endl;
  }
  auto recipes = *rec;
  auto recipeFinder = [&recipes](auto id)->std::optional<recipe>{
      auto in = std::find_if(std::begin(recipes), std::end(recipes), [&id](auto const& el){
          return el.id() == id;
      });
      if (in != std::end(recipes)) {
          return *in;
      }
      return std::nullopt;
  };

  //  read plans
  std::vector<plan> allPlans;
  auto planProvider = getDefaultProvider<io::plan_io>();
  std::cout << "read meal-plans ... " << std::flush;
  if (planProvider){
    for (auto const& planPath : planPaths) {
      auto pln = planProvider->read(planPath, recipeFinder);
      if (pln) {
        allPlans.push_back(*pln);
      }
    }
  }
  std::cout << "read " << allPlans.size() << " meal-plans from " << planPaths.size() << " files" << std::endl;

  //  read shopping lists
  std::vector<shopping_list> allShoppingLists;
  auto shoppingProvider = getDefaultProvider<io::shopping_io>();
  std::cout << "read shopping-lists ... " << std::flush;
  if (shoppingProvider){
    for (auto const& shoppingPath : shoppingPaths) {
      auto sho = shoppingProvider->read(shoppingPath, ingredientFinder);
      if (sho) {
        allShoppingLists.push_back(*sho);
      }
    }
  }
  std::cout << "read " << allShoppingLists.size() << " shopping-lists from " << shoppingPaths.size() << " files" << std::endl;

  // define upload commands
  _uploads.push_back({"finished", [this](){
    std::cout << "All uploaded. Finish." << std::endl;
    this->quit();
  }});

  // commands for recipes
  if (parser.isSet(doUploadRecipes)) {
    std::for_each(
        std::begin(recipes), std::end(recipes),
        [this, database_path = recipePath.parent_path()](auto const& single) {
          if (!single.image_path().empty()) {
            _uploads.push_back({single.title() + " image",
                                [single, this, database_path, last = _uploads.back().second]() {
                                  std::cout << "send: " << std::setw(_formatting_length)
                                            << single.title() + " image " << std::flush;
                                  _webAccess->sendRecipeImage(single, database_path, [last]() {
                                    std::cout << " finished" << std::endl;
                                    last();
                                  });
                                }});
            if (_formatting_length < single.title().length() + 6) {
              _formatting_length = single.title().length() + 6;
            }
          }
          std::cout << "create command for recipe: " << single.title() << std::endl;
          _uploads.push_back({single.title(), [single, this, last = _uploads.back().second]() {
                                std::cout << "send: " << std::setw(_formatting_length)
                                          << single.title() + " " << std::flush;
                                _webAccess->sendRecipe(single, [last]() {
                                  std::cout << " finished" << std::endl;
                                  last();
                                });
                              }});
          if (_formatting_length < single.title().length()) {
            _formatting_length = single.title().length();
          }
        });
  }

  // commands for paths
  if (parser.isSet(doUploadPlans)) {
    std::for_each(std::begin(allPlans), std::end(allPlans), [this](auto const& single) {
      std::cout << "create command for plan: " << single.name() << std::endl;
      _uploads.push_back({single.name(), [single, this, last = _uploads.back().second]() {
                            std::cout << "send: " << std::setw(_formatting_length)
                                      << single.name() + " " << std::flush;
                            _webAccess->sendPlan(single, [last]() {
                              std::cout << " finished" << std::endl;
                              last();
                            });
                          }});
      if (_formatting_length < single.name().length()) {
        _formatting_length = single.name().length();
      }
    });
  }
  // commands for shoppinglists
  if (parser.isSet(doUploadShopping)) {
    std::for_each(
        std::begin(allShoppingLists), std::end(allShoppingLists), [this](auto const& single) {
          std::cout << "create command for shopping-list: " << single.name() << std::endl;
          _uploads.push_back({single.name(), [single, this, last = _uploads.back().second]() {
                                std::cout << "send: " << std::setw(_formatting_length)
                                          << single.name() + " " << std::flush;
                                _webAccess->sendShopping(single, [last]() {
                                  std::cout << " finished" << std::endl;
                                  last();
                                });
                              }});
          if (_formatting_length < single.name().length()) {
            _formatting_length = single.name().length();
          }
        });
  }
}

}
