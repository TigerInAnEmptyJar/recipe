#include "shopping_list_model.hpp"
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
// namespace shopping_lists {
// class data_model : public QAbstractListModel
//{
//  Q_OBJECT

// public:
//  using QAbstractListModel::QAbstractListModel;

//  int rowCount(QModelIndex const& parent = QModelIndex()) const override;

//  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
//  QHash<int, QByteArray> roleNames() const override;

//  Qt::ItemFlags flags(QModelIndex const& index) const override;
//  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

//  Q_INVOKABLE void loadLast();
//  Q_INVOKABLE void load(QUrl const& url);
//  Q_INVOKABLE void storeLast();
//  Q_INVOKABLE void store(QUrl const& url);

//  Q_INVOKABLE QString databasePath() const;

//  void setFinder(std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder);
//  bool lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const;

// private:
//  shopping_list _data;
//  std::vector<std::shared_ptr<shopping_day_model>> _days;
//  std::filesystem::path _database_path;
//  std::function<std::optional<ingredient>(boost::uuids::uuid const&)> _finder;
//};

//}

// void shopping_list_model::loadLast()
//{
//  if (auto model = std::dynamic_pointer_cast<shopping_lists::data_model>(_model)) {
//    model->loadLast();
//  }
//}

// void shopping_list_model::load(QUrl const& url)
//{
//  if (auto model = std::dynamic_pointer_cast<shopping_lists::data_model>(_model)) {
//    model->load(url);
//  }
//}

// void shopping_list_model::storeLast()
//{
//  if (auto model = std::dynamic_pointer_cast<shopping_lists::data_model>(_model)) {
//    model->storeLast();
//  }
//}

// void shopping_list_model::store(QUrl const& url)
//{
//  if (auto model = std::dynamic_pointer_cast<shopping_lists::data_model>(_model)) {
//    model->store(url);
//  }
//}

// void shopping_list_model::setFinder(std::function<std::optional<ingredient>(boost::uuids::uuid
// const&)> finder)
//{
//  if (auto model = std::dynamic_pointer_cast<shopping_lists::data_model>(_model)) {
//    return model->setFinder(finder);
//  }
//}

// bool shopping_list_model::lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const
//{
//  if (auto model = std::dynamic_pointer_cast<shopping_lists::data_model>(_model)) {
//    return model->lessThan(lhs, rhs);
//  }
//  return false;
//}

int shopping_list_model::rowCount(QModelIndex const&) const
{
  return std::distance(std::begin(_data), std::end(_data));
}

QVariant shopping_list_model::data(QModelIndex const& index, int role) const
{
  if (!index.isValid()) {
    return {};
  }
  if (role == shopping_list_model::Roles::title_role) {
    auto element = std::begin(_data);
    std::advance(element, index.row());
    if (element >= std::end(_data)) {
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
  if (!index.isValid()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  auto element = std::begin(_data);
  std::advance(element, index.row());
  if (element >= std::end(_data)) {
    return Qt::ItemFlag::NoItemFlags;
  }

  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool shopping_list_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  if (!index.isValid() || role != shopping_list_model::Roles::title_role) {
    return false;
  }
  auto element = std::begin(_data);
  std::advance(element, index.row());
  if (element >= std::end(_data)) {
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

  beginResetModel();
  _data = *data;
  _days.clear();
  std::transform(std::begin(*data), std::end(*data), std::back_inserter(_days),
                 [](auto& item) { return std::make_shared<shopping_day_model>(item); });
  endResetModel();
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

  beginResetModel();
  _data = *data;
  _days.clear();
  std::transform(std::begin(*data), std::end(*data), std::back_inserter(_days),
                 [](auto& item) { return std::make_shared<shopping_day_model>(item); });
  endResetModel();
}

void shopping_list_model::storeLast()
{
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
  provider.shopping(provider.installed_shopping().begin()->first)->write(_data, path);
}

void shopping_list_model::store(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();
  provider.shopping(provider.installed_shopping().begin()->first)->write(_data, path);
  QSettings settings;
  settings.setValue(::shoppingListFile, url.path());
}

QString shopping_list_model::databasePath() const
{
  return QString::fromStdString(_database_path.native());
}
} // namespace gui
} // namespace recipe
