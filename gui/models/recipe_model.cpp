#include "recipe_model.hpp"

#include "amount_list_adapter.hpp"

#include "io_provider.h"

#include <QImage>
#include <QSettings>
#include <QUrl>

#include <boost/uuid/string_generator.hpp>

#include <iostream>

namespace {
QString const recipeFile = "last_recipe_file";
}

namespace recipe {
namespace gui {

template <>
class amount_list_adapter_impl<recipe> : public amount_list_adapter
{
public:
  amount_list_adapter_impl(recipe& obj) : _object(obj) {}
  size_t size() override { return std::distance(std::begin(_object), std::end(_object)); }
  amounted_ingredient& at(size_t i) override { return *(_object.begin() + i); }
  amounted_ingredient const& at(size_t i) const override { return *(_object.begin() + i); }
  bool singleAmount() const override { return true; }

private:
  recipe& _object;
};

int recipe_model::rowCount(QModelIndex const&) const { return _data.size(); }

QVariant recipe_model::data(QModelIndex const& index, int role) const
{
  if (!index.isValid() || static_cast<size_t>(index.row()) >= _data.size()) {
    return {};
  }

  switch (role) {
  case title_role:
    return QString::fromStdString(_data[index.row()].first.title());
  case type_role: {
    static std::map<meal_t, QString> const meal_types{
        std::make_pair(meal_t::fish, tr("Fish")),
        std::make_pair(meal_t::vegetarian, tr("Vegetarian")),
        std::make_pair(meal_t::vegan, tr("Vegan")), std::make_pair(meal_t::sweet, tr("Sweet")),
        std::make_pair(meal_t::other, tr("Other"))};
    auto item = meal_types.find(_data[index.row()].first.meal_type());
    if (item != std::end(meal_types)) {
      return item->second;
    }
    return {};
  }
  case servings_role:
    return _data[index.row()].first.servings();
  case time_role:
    return static_cast<quint64>(_data[index.row()].first.preparation_time().count());
  case ingredient_role:
    std::cout << "data: ingredients: " << _data[index.row()].second->rowCount() << std::endl;
    return QVariant::fromValue(_data[index.row()].second.get());
  case instructions_role:
    return QString::fromStdString(_data[index.row()].first.instructions());
  case image_path_role:
    return QString::fromStdString(_data[index.row()].first.image_path().native());
  case image_role:
    return "file://" + QString::fromStdString(
                           (_database_path / _data[index.row()].first.image_path()).native());
  case fat_role:
    return _data[index.row()].first.g_fat();
  case protein_role:
    return _data[index.row()].first.g_proteins();
  case carbo_role:
    return _data[index.row()].first.g_carbohydrates();
  case calories_role:
    return _data[index.row()].first.calories();
  case joules_role:
    return _data[index.row()].first.joules();
  case source_role:
    return QString::fromStdString(_data[index.row()].first.source());
  case tag_role: {
    QStringList tags;
    for (auto tag : _data[index.row()].first.tagList()) {
      tags.append(QString::fromStdString(tag));
    }
    return tags;
  }
  case eater_role: {
    QStringList eaters;
    for (auto eater : _data[index.row()].first.eaterList()) {
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

QStringList recipe_model::amount_types() const
{
  static QStringList const amountTypes{
      tr("Liter"),  tr("Milliliter"), tr("Cups"),  tr("Grams"),  tr("Kg"),
      tr("Ounces"), tr("Pounds"),     tr("Piece"), tr("Bundle"), tr("Can"),
  };
  return amountTypes;
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
    _data[position].first.title(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::type_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first.meal_type(static_cast<meal_t>(value.toInt()));
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::servings_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first.servings(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::time_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first.preparation_time(std::chrono::minutes{value.toInt()});
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::instructions_role:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].first.instructions(value.toString().toStdString());
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
    _data[position].first.image_path(std::filesystem::relative(image_path, _database_path));
    dataChanged(index, index, {role, image_role});
    return true;
  }
  case RecipeRoles::image_role:
    break;
  case RecipeRoles::fat_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first.g_fat(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::protein_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first.g_proteins(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::carbo_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first.g_carbohydrates(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::calories_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first.calories(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::joules_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first.joules(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case RecipeRoles::source_role:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].first.source(value.toString().toStdString());
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
  _data.push_back(std::make_pair(recipe{}, nullptr));
  using adapter_t = amount_list_adapter_impl<recipe>;
  _data.back().second = std::make_shared<amounted_list_model>(
      std::make_shared<adapter_t>(_data.back().first), _database_path);
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

void recipe_model::addTag(int index, QString const& tag)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  _data[index].first.addTag(tag.toStdString());
  dataChanged(this->index(index), this->index(index), {tag_role});
}

void recipe_model::removeTag(int index, QString const& tag)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  _data[index].first.removeTag(tag.toStdString());
  dataChanged(this->index(index), this->index(index), {tag_role});
}

void recipe_model::addEater(int index, QString const& eater)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  _data[index].first.addEater(eater.toStdString());
  dataChanged(this->index(index), this->index(index), {eater_role});
}

void recipe_model::removeEater(int index, QString const& eater)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  _data[index].first.removeEater(eater.toStdString());
  dataChanged(this->index(index), this->index(index), {eater_role});
}

void recipe_model::addIngredient(int index, QString const& id)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  boost::uuids::string_generator gen;
  auto ingredient = _finder(gen(id.toStdString()));
  if (ingredient.has_value()) {
    _data[index].second->startModelChange();
    amounted_ingredient element{*ingredient};
    switch (ingredient->default_amount()) {
    case ingredient::amount_type::mass:
      element.add(amounted_ingredient::amount_t::grams, 100);
      break;
    case ingredient::amount_type::volume:
      element.add(amounted_ingredient::amount_t::milliliter, 100);
      break;
    case ingredient::amount_type::piece:
      element.add(amounted_ingredient::amount_t::piece, 1);
      break;
    }
    _data[index].first.add(element);
    _data[index].second->endModelChange();
    dataChanged(this->index(index), this->index(index), {ingredient_role});
  }
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
  _data.clear();
  std::for_each(std::begin(*data), std::end(*data), [this](auto const& item) {
    _data.push_back(std::make_pair(item, nullptr));
    using adapter_t = amount_list_adapter_impl<recipe>;
    _data.back().second = std::make_shared<amounted_list_model>(
        std::make_shared<adapter_t>(_data.back().first), _database_path);
  });
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
  _data.clear();
  std::for_each(std::begin(*data), std::end(*data), [this](auto const& item) {
    _data.push_back(std::make_pair(item, nullptr));
    using adapter_t = amount_list_adapter_impl<recipe>;
    _data.back().second = std::make_shared<amounted_list_model>(
        std::make_shared<adapter_t>(_data.back().first), _database_path);
  });
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
  std::vector<recipe> out;
  std::transform(std::begin(_data), std::end(_data), std::back_inserter(out),
                 [](auto const& item) { return item.first; });
  provider.recipe(provider.installed_recipe().begin()->first)->write(out, path);
}

void recipe_model::store(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();
  std::vector<recipe> out;
  std::transform(std::begin(_data), std::end(_data), std::back_inserter(out),
                 [](auto const& item) { return item.first; });
  provider.recipe(provider.installed_recipe().begin()->first)->write(out, path);
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

std::optional<recipe> recipe_model::findRecipe(boost::uuids::uuid const& id) const
{
  auto element = std::find_if(std::begin(_data), std::end(_data),
                              [id](auto item) { return item.first.id() == id; });
  if (element != std::end(_data)) {
    return element->first;
  }
  return {};
}

} // namespace gui
} // namespace recipe
