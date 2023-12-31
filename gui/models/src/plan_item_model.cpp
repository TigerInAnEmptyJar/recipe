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
    return QString::fromStdString(item->item().title());
  case ItemRoles::image_path_role:
    if (item->item().image_path().empty()) {
      return {};
    }
    if (item->item().image_path().is_relative()) {
      return QString::fromStdString((_database_path / item->item().image_path()).native());
    }
    return QString::fromStdString(item->item().image_path().native());
  case ItemRoles::subscriber_role: {
    auto const& subscribers = item->subscribers();
    QStringList result;
    std::for_each(subscribers.begin(), subscribers.end(), [&result](auto const& element) {
      result.append(QString::fromStdString(element));
    });
    return result;
  }

  case ItemRoles::image_role:
    if (item->item().image_path().empty()) {
      return {};
    }
    if (item->item().image_path().is_relative()) {
      return "file://" + QString::fromStdString(
                             (_database_path / "recipes" / item->item().image_path()).native());
    }
    return "file://" + QString::fromStdString(item->item().image_path().native());
  }
  return {};
}

QHash<int, QByteArray> plan_item_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(ItemRoles::title_role, "title");
  roles.insert(ItemRoles::image_role, "image");
  roles.insert(ItemRoles::subscriber_role, "subscribers");
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

QString plan_item_model::databasePath() const
{
  return QString::fromStdString(_database_path.native());
}

void plan_item_model::setDatabasePath(std::filesystem::path const& database_path)
{
  _database_path = database_path;
}

FullRecipeModel::FullRecipeModel(
    plan_item* data, std::filesystem::path const& database_path,
    std::function<std::optional<recipe>(boost::uuids::uuid const&)> finder)
    : _data(data), _database_path(database_path), _finder(finder)
{
}

int FullRecipeModel::rowCount(QModelIndex const& /*parent*/) const
{
  return std::distance(_data->begin_full(), _data->end_full());
}

QVariant FullRecipeModel::data(QModelIndex const& index, int role) const
{
  auto item = _data->begin_full();
  std::advance(item, index.row());
  if (item == _data->end_full()) {
    return {};
  }
  switch (role) {
  case ItemRoles::title_role:
    return QString::fromStdString(item->title());
  case ItemRoles::image_path_role:
    if (item->image_path().empty()) {
      return {};
    }
    if (item->image_path().is_relative()) {
      return QString::fromStdString((_database_path / item->image_path()).native());
    }
    return QString::fromStdString(item->image_path().native());
  case ItemRoles::image_role:
    if (item->image_path().empty()) {
      return {};
    }
    if (item->image_path().is_relative()) {
      return "file://" +
             QString::fromStdString((_database_path / "recipes" / item->image_path()).native());
    }
    return "file://" + QString::fromStdString(item->image_path().native());
  }
  return {};
}

QHash<int, QByteArray> FullRecipeModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(ItemRoles::title_role, "title");
  roles.insert(ItemRoles::image_path_role, "imagePath");
  roles.insert(ItemRoles::image_role, "image");
  return roles;
}

void FullRecipeModel::addRecipe(QString const& recipe)
{
  boost::uuids::string_generator gen;
  auto rec = _finder(gen(recipe.toStdString()));
  if (!rec.has_value()) {
    return;
  }
  beginResetModel();
  _data->addFullRecipe(*rec);
  endResetModel();
}

void FullRecipeModel::deleteRecipe(int index)
{
  auto item = _data->begin_full();
  std::advance(item, index);
  if (item == _data->end_full()) {
    return;
  }

  beginResetModel();
  _data->removeFullRecipe(item);
  endResetModel();
}

} // namespace gui
} // namespace recipe
