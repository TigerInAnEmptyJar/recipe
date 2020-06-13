#include "ingredient_model.hpp"

#include "io_provider.h"

#include <QImage>
#include <QUrl>

#include <iostream>

namespace recipe {
namespace gui {

int ingredient_model::rowCount(QModelIndex const&) const { return _data.size(); }

QVariant ingredient_model::data(QModelIndex const& index, int role) const
{
  if (!index.isValid() || index.row() >= _data.size()) {
    return {};
  }

  static std::map<ingredient::category_t, std::string> const categories{
      std::make_pair(ingredient::category_t::fruit, "Fruit"),
      std::make_pair(ingredient::category_t::vegetable, "Vegetable"),
      std::make_pair(ingredient::category_t::meat, "Meat"),
      std::make_pair(ingredient::category_t::fish, "Fish"),
      std::make_pair(ingredient::category_t::spices, "Spices"),
      std::make_pair(ingredient::category_t::diary, "Diary"),
      std::make_pair(ingredient::category_t::noodles, "Noodles"),
      std::make_pair(ingredient::category_t::baking, "Baking"),
      std::make_pair(ingredient::category_t::drink, "Drink"),
      std::make_pair(ingredient::category_t::frozen, "Frozen"),
      std::make_pair(ingredient::category_t::canned, "Canned"),
  };

  static std::map<ingredient::amount_type, std::string> const amounts{
      std::make_pair(ingredient::amount_type::volume, "Volume"),
      std::make_pair(ingredient::amount_type::mass, "Mass"),
      std::make_pair(ingredient::amount_type::piece, "Piece"),
  };

  switch (role) {
  case name_role:
    return QString::fromStdString(_data[index.row()].name());
  case category_role: {
    auto item = categories.find(_data[index.row()].category());
    if (item != std::end(categories)) {
      return QString::fromStdString(item->second);
    }
    return {};
  }
  case amount_role: {
    auto item = amounts.find(_data[index.row()].default_amount());
    if (item != std::end(amounts)) {
      return QString::fromStdString(item->second);
    }
    return {};
  }
  case sectioned_role:
    return _data[index.row()].sectioned();
  case image_path_role:
    return QString::fromStdString(_data[index.row()].image_path().native());
  case image_role: {
    if (_data[index.row()].image_path().empty()) {
      return {};
    }
    return QImage{QString::fromStdString(_data[index.row()].image_path().native())};
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

void ingredient_model::load(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();

  if (!std::filesystem::exists(path)) {
    std::cout << "url doesn't exist " << path.native() << std::endl;
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

void ingredient_model::store(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();
  provider.ingredient(provider.installed_ingredient().begin()->first)->write(_data, path);
}

} // namespace gui
} // namespace recipe
