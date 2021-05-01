#include "shopping_list_model.hpp"
#include "plan_model.hpp"
#include "shopping_day_model.hpp"

#include "io_provider.h"
#include "shopping_list.h"

#include <QSettings>
#include <QUrl>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

namespace {
QString const shoppingListFile = "last_shopping_list_file";
}

namespace recipe {
namespace gui {

shopping_list_model::shopping_list_model() : _data(nullptr) {}

int shopping_list_model::rowCount(QModelIndex const&) const
{
  if (!_data) {
    return 0;
  }
  return std::distance(std::begin(*_data), std::end(*_data));
}

QVariant shopping_list_model::data(QModelIndex const& index, int role) const
{
  if (!_data) {
    return {};
  }
  if (!index.isValid()) {
    return {};
  }
  if (role == shopping_list_model::Roles::title_role) {
    auto element = std::begin(*_data);
    std::advance(element, index.row());
    if (element >= std::end(*_data)) {
      return {};
    }
    return QString::fromStdString(element->name());
  }
  if (role == shopping_list_model::Roles::day_role) {
    auto element = std::begin(_days);
    std::advance(element, index.row());
    if (element >= std::end(_days)) {
      return {};
    }
    return QVariant::fromValue(element->get());
  }
  return {};
}

QHash<int, QByteArray> shopping_list_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(shopping_list_model::Roles::title_role, "name");
  roles.insert(shopping_list_model::Roles::day_role, "day");
  return roles;
}

Qt::ItemFlags shopping_list_model::flags(QModelIndex const& index) const
{
  if (!_data) {
    return Qt::ItemFlag::NoItemFlags;
  }
  if (!index.isValid()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  auto element = std::begin(*_data);
  std::advance(element, index.row());
  if (element >= std::end(*_data)) {
    return Qt::ItemFlag::NoItemFlags;
  }

  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool shopping_list_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  if (!_data) {
    return false;
  }
  if (!index.isValid() || role != shopping_list_model::Roles::title_role) {
    return false;
  }
  auto element = std::begin(*_data);
  std::advance(element, index.row());
  if (element >= std::end(*_data)) {
    return false;
  }

  element->name(value.toString().toStdString());
  dataChanged(index, index, {role});
  return true;
}

void shopping_list_model::loadLast()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::shoppingListFile).toString().toStdString()};
  _database_path = path.parent_path();
  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();

  auto data = provider.shopping(provider.installed_shopping().begin()->first)->read(path, _finder);
  if (!data.has_value()) {
    return;
  }

  generated(*data);
}

void shopping_list_model::load(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();

  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();
  auto data = provider.shopping(provider.installed_shopping().begin()->first)->read(path, _finder);
  if (!data.has_value()) {
    return;
  }
  QSettings settings;
  settings.setValue(::shoppingListFile, url.path());
  _database_path = path.parent_path();

  generated(*data);
}

void shopping_list_model::storeLast()
{
  if (!_data) {
    return;
  }
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::shoppingListFile).toString().toStdString()};
  if (!std::filesystem::exists(path)) {
    std::cout << "Save to existing file not possible: url doesn't exist " << path.native()
              << std::endl;
    return;
  }
  _database_path = path.parent_path();
  io::io_provider provider;
  provider.setup();
  provider.shopping(provider.installed_shopping().begin()->first)->write(*_data, path);
}

void shopping_list_model::store(QUrl const& url)
{
  if (!_data) {
    return;
  }
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();
  provider.shopping(provider.installed_shopping().begin()->first)->write(*_data, path);
  QSettings settings;
  settings.setValue(::shoppingListFile, url.path());
}

QStringList shopping_list_model::exportFormats() const
{
  io::io_provider provider;
  provider.setup();

  auto installed = provider.installed_shopping();
  QStringList result;

  std::for_each(std::begin(installed), std::end(installed), [&result](auto const& element) {
    result.append(QString::fromStdString(element.second.first));
  });

  return result;
}

void shopping_list_model::exportList(QUrl const& url, int format) const
{
  if (!_data) {
    return;
  }
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();

  auto installedShopping = provider.installed_shopping();
  if (format < 0 || static_cast<size_t>(format) >= installedShopping.size()) {
    std::cout << "Export shopping-list not possible, bad format specifier:" << format << std::endl;
    return;
  }
  auto usedExporter = installedShopping.begin();
  std::advance(usedExporter, format);
  std::cout << "Exporting shopping-list to file " << path.native() << " in format "
            << usedExporter->second.first << std::endl;
  usedExporter->second.second->write(*_data, path);
}

QString shopping_list_model::databasePath() const
{
  return QString::fromStdString(_database_path.native());
}

void shopping_list_model::setPlan(plan_model* planModel) { _planModel = planModel; }

void shopping_list_model::generate() { generated(_planModel->generateList()); }

void shopping_list_model::generated(shopping_list const& shopping)
{
  beginResetModel();
  _data = std::make_unique<shopping_list>(shopping);
  _days.clear();
  std::transform(std::begin(*_data), std::end(*_data), std::back_inserter(_days), [](auto& item) {
    auto newModel = std::make_shared<shopping_day_model>(item);
    newModel->setSortRole(::recipe::gui::shopping_day_model::Roles::category_role);
    newModel->setDynamicSortFilter(true);
    newModel->sort(0);

    return newModel;
  });
  endResetModel();
}

} // namespace gui
} // namespace recipe
