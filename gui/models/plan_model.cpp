#include "plan_model.hpp"
#include "plan_item_model.hpp"

#include "io_provider.h"

#include <QSettings>
#include <QUrl>

#include <iostream>

namespace {
QString const planFile{"lastPlanFile"};
}

namespace recipe {
namespace gui {

plan_model::plan_model(std::function<std::optional<recipe>(boost::uuids::uuid const&)> finder)
    : _data("New plan", 7, 3), _finder(finder)
{
}

int plan_model::rowCount(QModelIndex const&) const
{
  return std::distance(_data.begin(), _data.end());
}

QVariant plan_model::data(QModelIndex const& index, int role) const
{
  auto element = _data.begin();
  std::advance(element, index.row());
  if (element == _data.end()) {
    return {};
  }

  switch (role) {
  case PlanRoles::name_role:
    return QString::fromStdString(element->name());
  case PlanRoles::shopping_role:
    return element->shoppingBefore();
  case PlanRoles::subscribers_role: {
    auto const& subscribers = element->subscribers();
    QStringList result;
    std::for_each(subscribers.begin(), subscribers.end(), [&result](auto const& element) {
      result.append(QString::fromStdString(element));
    });
    return result;
  }
  case PlanRoles::item_role:
    return QVariant::fromValue(_items[index.row()].get());
  }
  return {};
}

QHash<int, QByteArray> plan_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(PlanRoles::name_role, "name");
  roles.insert(PlanRoles::shopping_role, "shoppingBefore");
  roles.insert(PlanRoles::subscribers_role, "subscribers");
  roles.insert(PlanRoles::item_role, "item");
  return roles;
}

Qt::ItemFlags plan_model::flags(QModelIndex const& index) const
{
  auto position = index.row();
  if (position >= std::distance(_data.begin(), _data.end())) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool plan_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  auto item = _data.begin();
  std::advance(item, index.row());
  if (item == _data.end()) {
    return false;
  }
  switch (role) {
  case PlanRoles::name_role:
    if (value.type() == QVariant::String) {
      item->name(value.toString().toStdString());
      Q_EMIT dataChanged(index, index, {role});
      return true;
    }
  case PlanRoles::shopping_role:
    if (value.type() == QVariant::Bool) {
      item->shoppingBefore(value.toBool());
      Q_EMIT dataChanged(index, index, {role});
      return true;
    }
  case PlanRoles::subscribers_role:
  case PlanRoles::item_role:
    break;
  }
  return false;
}

void plan_model::newPlan(int ndays, int nmeals)
{
  beginResetModel();
  _items.clear();
  _data = plan{"New plan", static_cast<size_t>(ndays), static_cast<size_t>(nmeals)};
  std::transform(_data.begin(), _data.end(), std::back_inserter(_items), [this](auto& element) {
    return std::make_shared<plan_item_model>(&element, _finder);
  });
  endResetModel();
  Q_EMIT daysChanged(_data.days());
  Q_EMIT mealsChanged(_data.meals());
}

void plan_model::loadLast()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::planFile).toString().toStdString()};
  _database_path = path.parent_path();
  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();

  auto data = provider.plan(provider.installed_plan().begin()->first)->read(path, _finder);
  if (!data.has_value()) {
    return;
  }

  beginResetModel();
  _data = *data;
  _items.clear();
  std::transform(_data.begin(), _data.end(), std::back_inserter(_items), [this](auto& element) {
    return std::make_shared<plan_item_model>(&element, _finder);
  });
  endResetModel();
  Q_EMIT daysChanged(_data.days());
  Q_EMIT mealsChanged(_data.meals());
}

void plan_model::load(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();

  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();
  auto data = provider.plan(provider.installed_plan().begin()->first)->read(path, _finder);
  if (!data.has_value()) {
    return;
  }

  QSettings settings;
  settings.setValue(::planFile, url.path());
  _database_path = path.parent_path();

  beginResetModel();
  _data = *data;
  _items.clear();
  std::transform(_data.begin(), _data.end(), std::back_inserter(_items), [this](auto& element) {
    return std::make_shared<plan_item_model>(&element, _finder);
  });
  endResetModel();
  Q_EMIT daysChanged(_data.days());
  Q_EMIT mealsChanged(_data.meals());
}

void plan_model::store()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::planFile).toString().toStdString()};
  if (!std::filesystem::exists(path)) {
    std::cout << "Save to existing file not possible: url doesn't exist " << path.native()
              << std::endl;
    return;
  }
  _database_path = path.parent_path();
  io::io_provider provider;
  provider.setup();

  provider.plan(provider.installed_plan().begin()->first)->write(_data, path);
}

void plan_model::storeAs(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();

  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();

  QSettings settings;
  settings.setValue(::planFile, url.path());
  _database_path = path.parent_path();

  provider.plan(provider.installed_plan().begin()->first)->write(_data, path);
}

QString plan_model::databasePath() const { return QString::fromStdString(_database_path.native()); }

int plan_model::getDays() const { return _data.days(); }

int plan_model::getMeals() const { return _data.meals(); }

QString plan_model::getTitle() const { return QString::fromStdString(_data.name()); }

void plan_model::setTitle(QString const& title)
{
  _data.name(title.toStdString());
  Q_EMIT titleChanged(title);
}

} // namespace gui
} // namespace recipe
