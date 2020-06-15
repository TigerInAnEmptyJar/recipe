#include "recipe_model.hpp"

#include "io_provider.h"

#include <QImage>
#include <QSettings>
#include <QUrl>

#include <iostream>

namespace {
QString const recipeFile = "last_recipe_file";
}

namespace recipe {
namespace gui {

int recipe_model::rowCount(QModelIndex const&) const { return _data.size(); }

QVariant recipe_model::data(QModelIndex const& index, int role) const
{
  if (!index.isValid() || index.row() >= _data.size()) {
    return {};
  }

  switch (role) {
  case title_role:
    return QString::fromStdString(_data[index.row()].title());
  case type_role: {
    static std::map<meal_t, QString> const meal_types{
        std::make_pair(meal_t::fish, tr("Fish")),
        std::make_pair(meal_t::vegetarian, tr("Vegetarian")),
        std::make_pair(meal_t::vegan, tr("Vegan")), std::make_pair(meal_t::sweet, tr("Sweet")),
        std::make_pair(meal_t::other, tr("Other"))};
    auto item = meal_types.find(_data[index.row()].meal_type());
    if (item != std::end(meal_types)) {
      return item->second;
    }
    return {};
  }
  case servings_role:
    return _data[index.row()].servings();
  case time_role:
    return static_cast<quint64>(_data[index.row()].preparation_time().count());
  case ingredient_role: //! todo
    return {};
  case instructions_role:
    return QString::fromStdString(_data[index.row()].instructions());
  case image_path_role:
    return QString::fromStdString(_data[index.row()].image_path().native());
  case image_role:
    return "file://" +
           QString::fromStdString((_database_path / _data[index.row()].image_path()).native());
  case fat_role:
    return _data[index.row()].g_fat();
  case protein_role:
    return _data[index.row()].g_proteins();
  case carbo_role:
    return _data[index.row()].g_carbohydrates();
  case calories_role:
    return _data[index.row()].calories();
  case joules_role:
    return _data[index.row()].joules();
  case source_role:
    return QString::fromStdString(_data[index.row()].source());
  case tag_role: {
    QStringList tags;
    for (auto tag : _data[index.row()].tagList()) {
      tags.append(QString::fromStdString(tag));
    }
    return tags;
  }
  case eater_role: {
    QStringList eaters;
    for (auto eater : _data[index.row()].eaterList()) {
      eaters.append(QString::fromStdString(eater));
    }
    return eaters;
  }
  }
  return {};
}

QHash<int, QByteArray> recipe_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(title_role, "title");
  roles.insert(type_role, "meal_type");
  roles.insert(servings_role, "servings");
  roles.insert(time_role, "preparation_time");
  roles.insert(ingredient_role, "ingredient");
  roles.insert(instructions_role, "instructions");
  roles.insert(image_path_role, "image_path");
  roles.insert(image_role, "image");
  roles.insert(fat_role, "fat");
  roles.insert(protein_role, "protein");
  roles.insert(carbo_role, "carbohydrates");
  roles.insert(calories_role, "calories");
  roles.insert(joules_role, "joules");
  roles.insert(source_role, "source");
  roles.insert(tag_role, "tags");
  roles.insert(eater_role, "eaters");
  return roles;
}

QStringList recipe_model::meal_types() const
{
  static QStringList const mealList{
      tr("Fish"), tr("Vegetarian"), tr("Vegan"), tr("Sweet"), tr("Other"),
  };
  return mealList;
}

Qt::ItemFlags recipe_model::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool recipe_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return false;
  }
  switch (role) {
  case RecipeRoles::title_role:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].title(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::type_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].meal_type(static_cast<meal_t>(value.toInt()));
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::servings_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].servings(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::time_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].preparation_time(std::chrono::minutes{value.toInt()});
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::instructions_role:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].instructions(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::image_path_role: {
    if (value.type() != QVariant::String) {
      return false;
    }
    auto val = value.toString().toStdString();
    if (val.starts_with("file://")) {
      val = val.substr(7);
    }
    auto image_path = std::filesystem::path{val};
    _data[position].image_path(std::filesystem::relative(image_path, _database_path));
    dataChanged(index, index, {role, image_role});
    return true;
  }
  case RecipeRoles::image_role:
    break;
  case RecipeRoles::fat_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].g_fat(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::protein_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].g_proteins(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::carbo_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].g_carbohydrates(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::calories_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].calories(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::joules_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].joules(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::source_role:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].source(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::ingredient_role:
    break;
  case RecipeRoles::tag_role:
  case RecipeRoles::eater_role:
    break;
  }
  return false;
}

void recipe_model::addItem()
{
  beginInsertRows({}, _data.size(), _data.size());
  _data.push_back(recipe{});
  endInsertRows();
}

void recipe_model::deleteItem(int index)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }
  beginRemoveRows({}, index, index);
  _data.erase(_data.begin() + index);
  endRemoveRows();
}

void recipe_model::loadLast()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::recipeFile).toString().toStdString()};
  _database_path = path.parent_path();
  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();

  auto data = provider.recipe(provider.installed_recipe().begin()->first)->read(path, _finder);
  if (!data.has_value()) {
    return;
  }

  beginResetModel();
  _data = *data;
  endResetModel();
}

void recipe_model::load(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();

  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();
  auto data = provider.recipe(provider.installed_recipe().begin()->first)->read(path, _finder);
  if (!data.has_value()) {
    return;
  }
  QSettings settings;
  settings.setValue(::recipeFile, url.path());
  _database_path = path.parent_path();

  beginResetModel();
  _data = *data;
  endResetModel();
}

void recipe_model::storeLast()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::recipeFile).toString().toStdString()};
  if (!std::filesystem::exists(path)) {
    std::cout << "Save to existing file not possible: url doesn't exist " << path.native()
              << std::endl;
    return;
  }
  _database_path = path.parent_path();
  io::io_provider provider;
  provider.setup();
  provider.recipe(provider.installed_recipe().begin()->first)->write(_data, path);
}

void recipe_model::store(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();
  provider.recipe(provider.installed_recipe().begin()->first)->write(_data, path);
  QSettings settings;
  settings.setValue(::recipeFile, url.path());
}

QString recipe_model::databasePath() const
{
  return QString::fromStdString(_database_path.native());
}

void recipe_model::setFinder(
    std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder)
{
  _finder = finder;
}

} // namespace gui
} // namespace recipe
