#include "ingredient_model.hpp"

#include "io_provider.h"

#include <QImage>
#include <QSettings>
#include <QUrl>

#include <iostream>

namespace {
QString const ingredientFile = "last_ingredient_file";
}

namespace recipe {
namespace gui {

int ingredient_model::rowCount(QModelIndex const&) const { return _data.size(); }

QVariant ingredient_model::data(QModelIndex const& index, int role) const
{
  if (!index.isValid() || index.row() >= _data.size()) {
    return {};
  }

  static std::map<ingredient::category_t, QString> const categories{
      std::make_pair(ingredient::category_t::fruit, tr("Fruit")),
      std::make_pair(ingredient::category_t::vegetable, tr("Vegetable")),
      std::make_pair(ingredient::category_t::meat, tr("Meat")),
      std::make_pair(ingredient::category_t::fish, tr("Fish")),
      std::make_pair(ingredient::category_t::spices, tr("Spices")),
      std::make_pair(ingredient::category_t::diary, tr("Diary")),
      std::make_pair(ingredient::category_t::noodles, tr("Noodles")),
      std::make_pair(ingredient::category_t::baking, tr("Baking")),
      std::make_pair(ingredient::category_t::drink, tr("Drink")),
      std::make_pair(ingredient::category_t::frozen, tr("Frozen")),
      std::make_pair(ingredient::category_t::canned, tr("Canned")),
  };

  static std::map<ingredient::amount_type, QString> const amounts{
      std::make_pair(ingredient::amount_type::volume, tr("Volume")),
      std::make_pair(ingredient::amount_type::mass, tr("Mass")),
      std::make_pair(ingredient::amount_type::piece, tr("Piece")),
  };

  switch (role) {
  case name_role:
    return QString::fromStdString(_data[index.row()].name());
  case category_role: {
    auto item = categories.find(_data[index.row()].category());
    if (item != std::end(categories)) {
      return item->second;
    }
    return {};
  }
  case amount_role: {
    auto item = amounts.find(_data[index.row()].default_amount());
    if (item != std::end(amounts)) {
      return item->second;
    }
    return {};
  }
  case sectioned_role:
    return _data[index.row()].sectioned();
  case image_path_role:
    return QString::fromStdString(_data[index.row()].image_path().native());
  case image_role: {
    return "file://" +
           QString::fromStdString((_database_path / _data[index.row()].image_path()).native());
  }
  }
  return {};
}

QHash<int, QByteArray> ingredient_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(name_role, "name");
  roles.insert(category_role, "category");
  roles.insert(amount_role, "default_amount");
  roles.insert(sectioned_role, "isSectioned");
  roles.insert(image_path_role, "image_path");
  roles.insert(image_role, "image");
  return roles;
}

QStringList ingredient_model::categories() const
{
  static QStringList const categoryList{
      tr("Fruit"),   tr("Vegetable"), tr("Meat"),  tr("Fish"),   tr("Spices"), tr("Diary"),
      tr("Noodles"), tr("Baking"),    tr("Drink"), tr("Frozen"), tr("Canned"),
  };
  return categoryList;
}

QStringList ingredient_model::amounts() const
{
  static QStringList const amount_list{
      tr("Volume"),
      tr("Mass"),
      tr("Piece"),
  };
  return amount_list;
}

Qt::ItemFlags ingredient_model::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool ingredient_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return false;
  }
  switch (role) {
  case IngredientRoles::name_role: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].name(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  }
  case IngredientRoles::category_role: {
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].category(static_cast<ingredient::category_t>(value.toInt()));
    dataChanged(index, index, {role});
    return true;
  }
  case IngredientRoles::amount_role: {
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].default_amount(static_cast<ingredient::amount_type>(value.toInt()));
    dataChanged(index, index, {role});
    return true;
  }
  case IngredientRoles::sectioned_role: {
    if (value.type() != QVariant::Bool) {
      return false;
    }
    _data[position].sectioned(value.toBool());
    dataChanged(index, index, {role});
    return true;
  }
  case IngredientRoles::image_path_role: {
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
  }
  return false;
}

void ingredient_model::addItem()
{
  beginInsertRows({}, _data.size(), _data.size());
  _data.push_back(ingredient{});
  endInsertRows();
}

void ingredient_model::deleteItem(int index)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }
  beginRemoveRows({}, index, index);
  _data.erase(_data.begin() + index);
  endRemoveRows();
}

void ingredient_model::loadLast()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::ingredientFile).toString().toStdString()};
  _database_path = path.parent_path();
  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();

  auto data = provider.ingredient(provider.installed_ingredient().begin()->first)->read(path);
  if (!data.has_value()) {
    return;
  }

  beginResetModel();
  _data = *data;
  endResetModel();
}

void ingredient_model::load(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();

  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();
  auto data = provider.ingredient(provider.installed_ingredient().begin()->first)->read(path);
  if (!data.has_value()) {
    return;
  }
  QSettings settings;
  settings.setValue(::ingredientFile, url.path());
  _database_path = path.parent_path();

  beginResetModel();
  _data = *data;
  endResetModel();
}

void ingredient_model::storeLast()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::ingredientFile).toString().toStdString()};
  if (!std::filesystem::exists(path)) {
    std::cout << "Save to existing file not possible: url doesn't exist " << path.native()
              << std::endl;
    return;
  }
  _database_path = path.parent_path();
  io::io_provider provider;
  provider.setup();
  provider.ingredient(provider.installed_ingredient().begin()->first)->write(_data, path);
}

void ingredient_model::store(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();
  provider.ingredient(provider.installed_ingredient().begin()->first)->write(_data, path);
  QSettings settings;
  settings.setValue(::ingredientFile, url.path());
}

QString ingredient_model::databasePath() const
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::ingredientFile).toString().toStdString()};
  return QString::fromStdString(_database_path.native());
}

std::optional<ingredient> ingredient_model::findIngredient(boost::uuids::uuid const& id)
{
  auto item =
      std::find_if(std::begin(_data), std::end(_data), [id](auto item) { return item.id() == id; });
  if (item != std::end(_data)) {
    return *item;
  }
  return {};
}

} // namespace gui
} // namespace recipe
