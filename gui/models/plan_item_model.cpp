#include "plan_item_model.hpp"

#include <boost/uuid/string_generator.hpp>

namespace recipe {
namespace gui {

plan_item_model::plan_item_model(
    plan_item* data, std::function<std::optional<recipe>(boost::uuids::uuid const&)> finder)
    : _data(data), _finder(finder)
{
}

int plan_item_model::rowCount(QModelIndex const&) const
{
  return std::distance(_data->begin(), _data->end());
}

QVariant plan_item_model::data(QModelIndex const& index, int role) const
{
  auto item = _data->begin();
  std::advance(item, index.row());
  if (item == _data->end()) {
    return {};
  }
  switch (role) {
  case ItemRoles::title_role:
    return QString::fromStdString(item->title());
  case ItemRoles::image_path_role:
    return QString::fromStdString(item->image_path().native());
  case ItemRoles::image_role:
    return "file://" + QString::fromStdString((_database_path / item->image_path()).native());
  }
  return {};
}

QHash<int, QByteArray> plan_item_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(ItemRoles::title_role, "title");
  roles.insert(ItemRoles::image_role, "image");
  roles.insert(ItemRoles::image_path_role, "imagePath");
  return roles;
}

void plan_item_model::addRecipe(QString const& recipe)
{
  boost::uuids::string_generator gen;
  auto rec = _finder(gen(recipe.toStdString()));
  if (!rec.has_value()) {
    return;
  }
  beginResetModel();
  _data->add(*rec);
  endResetModel();
}

void plan_item_model::deleteRecipe(int index)
{
  auto item = _data->begin();
  std::advance(item, index);
  if (item == _data->end()) {
    return;
  }

  beginResetModel();
  _data->remove(item);
  endResetModel();
}

void plan_item_model::addSubscriber(QString const& eater) { _data->add(eater.toStdString()); }

void plan_item_model::removeSubscriber(QString const& eater) { _data->remove(eater.toStdString()); }

QString plan_item_model::databasePath() const
{
  return QString::fromStdString(_database_path.native());
}

} // namespace gui
} // namespace recipe
