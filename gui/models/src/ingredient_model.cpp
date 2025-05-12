#include "ingredient_model.hpp"

#include "enum_adapter.hpp"

#include "io_provider.h"

#include <QImage>
#include <QSettings>
#include <QUrl>

#include <boost/uuid/uuid_io.hpp>

#include <iostream>

namespace {
QString const ingredientFile = "last_ingredient_file";
}

namespace recipe {
namespace gui {
namespace ingredients {
class data_model : public QAbstractListModel
{
  Q_OBJECT
public:
  using QAbstractListModel::QAbstractListModel;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE QStringList categories() const;
  Q_INVOKABLE QStringList amounts() const;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  Q_INVOKABLE void addItem();
  Q_INVOKABLE void deleteItem(int index);

  Q_INVOKABLE void loadLast();
  Q_INVOKABLE void load(QUrl const& url);
  Q_INVOKABLE void storeLast();
  Q_INVOKABLE void store(QUrl const& url);

  Q_INVOKABLE QString databasePath() const;

  std::optional<ingredient> findIngredient(boost::uuids::uuid const& id);
  bool lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const;

private:
  std::vector<ingredient> _data;
  std::filesystem::path _database_path;
  enum_adapter<ingredient::amount_type> _amount_adapter;
  enum_adapter<ingredient::category_t> _category_adapter;
};
} // namespace ingredients

ingredient_model::ingredient_model()
    : QSortFilterProxyModel(), _model(std::make_shared<ingredients::data_model>())
{
  setSourceModel(_model.get());
}

QStringList ingredient_model::categories() const
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    return model->categories();
  }
  return {};
}

QStringList ingredient_model::amounts() const
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    return model->amounts();
  }
  return {};
}

void ingredient_model::addItem()
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    model->addItem();
  }
}

void ingredient_model::deleteItem(int index)
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    model->deleteItem(mapToSource(this->index(index, 0)).row());
  }
}

void ingredient_model::loadLast()
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    model->loadLast();
  }
}

void ingredient_model::load(QUrl const& url)
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    model->load(url);
  }
}

void ingredient_model::storeLast()
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    model->storeLast();
  }
}

void ingredient_model::store(QUrl const& url)
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    model->store(url);
  }
}

QString ingredient_model::databasePath() const
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    return model->databasePath();
  }
  return {};
}

std::optional<ingredient> ingredient_model::findIngredient(boost::uuids::uuid const& id)
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    return model->findIngredient(id);
  }
  return {};
}

bool ingredient_model::lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const
{
  if (auto model = std::dynamic_pointer_cast<ingredients::data_model>(_model)) {
    return model->lessThan(lhs, rhs);
  }
  return false;
}

namespace ingredients {
int data_model::rowCount(QModelIndex const&) const { return _data.size(); }

QVariant data_model::data(QModelIndex const& index, int role) const
{
  if (!index.isValid() || static_cast<size_t>(index.row()) >= _data.size()) {
    return {};
  }

  switch (role) {
  case ingredient_model::IngredientRoles::name_role:
    return QString::fromStdString(_data[index.row()].name());
  case ingredient_model::IngredientRoles::category_role: {
    return _category_adapter.to_string(_data[index.row()].category());
  }
  case ingredient_model::IngredientRoles::amount_role: {
    return _amount_adapter.to_string(_data[index.row()].default_amount());
  }
  case ingredient_model::IngredientRoles::sectioned_role:
    return _data[index.row()].sectioned();
  case ingredient_model::IngredientRoles::image_path_role: {
    auto path = _data[index.row()].image_path();
    if (path.empty()) {
      return QVariant();
    }
    return QString::fromStdString(path.native());
  }
  case ingredient_model::IngredientRoles::image_role: {
    auto path = _data[index.row()].image_path();
    if (path.empty()) {
      return QVariant();
    }
    if (path.is_absolute()) {
      return "file:://" + QString::fromStdString(path.native());
    }
    return "file://" + QString::fromStdString((_database_path / path).native());
  }
  case ingredient_model::IngredientRoles::id_role:
    return QString::fromStdString(boost::uuids::to_string(_data[index.row()].id()));
  }
  return {};
}

bool data_model::lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const
{
  if (!lhs.isValid() || !rhs.isValid() || static_cast<size_t>(lhs.row()) > _data.size() ||
      static_cast<size_t>(rhs.row()) > _data.size()) {
    return false;
  }
  auto& left = _data[lhs.row()];
  auto& right = _data[rhs.row()];
  if (left.category() != right.category()) {
    return left.category() < right.category();
  }
  return left.name() < right.name();
}

QHash<int, QByteArray> data_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(ingredient_model::IngredientRoles::name_role, "name");
  roles.insert(ingredient_model::IngredientRoles::category_role, "category");
  roles.insert(ingredient_model::IngredientRoles::amount_role, "default_amount");
  roles.insert(ingredient_model::IngredientRoles::sectioned_role, "isSectioned");
  roles.insert(ingredient_model::IngredientRoles::image_path_role, "image_path");
  roles.insert(ingredient_model::IngredientRoles::image_role, "image");
  roles.insert(ingredient_model::IngredientRoles::id_role, "id");
  return roles;
}

QStringList data_model::categories() const { return _category_adapter.all(); }

QStringList data_model::amounts() const { return _amount_adapter.all(); }

Qt::ItemFlags data_model::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool data_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return false;
  }
  switch (role) {
  case ingredient_model::IngredientRoles::name_role: {
    if (value.metaType() != QMetaType{QMetaType::Type::QString}) {
      return false;
    }
    _data[position].name(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  }
  case ingredient_model::IngredientRoles::category_role: {
    auto item = _category_adapter.to_enum(value);
    if (item) {
      _data[position].category(*item);
      dataChanged(index, index, {role});
      return true;
    }
      return false;
  }
  case ingredient_model::IngredientRoles::amount_role: {
    auto item = _amount_adapter.to_enum(value);
    if (item) {
      _data[position].default_amount(*item);
      dataChanged(index, index, {role});
      return true;
    }
      return false;
  }
  case ingredient_model::IngredientRoles::sectioned_role: {
    if (value.metaType() != QMetaType{QMetaType::Type::Bool}) {
      return false;
    }
    _data[position].sectioned(value.toBool());
    dataChanged(index, index, {role});
    return true;
  }
  case ingredient_model::IngredientRoles::image_path_role: {
    if (value.metaType() != QMetaType{QMetaType::Type::QString}) {
      return false;
    }
    auto val = value.toString().toStdString();
    if (val.starts_with("file://")) {
      val = val.substr(7);
    }
    auto image_path = std::filesystem::path{val};
    _data[position].image_path(std::filesystem::relative(image_path, _database_path));
    dataChanged(index, index, {role, ingredient_model::IngredientRoles::image_role});
    return true;
  }
  case ingredient_model::IngredientRoles::id_role:
    return false;
  }
  return false;
}

void data_model::addItem()
{
  beginInsertRows({}, _data.size(), _data.size());
  _data.push_back(ingredient{});
  endInsertRows();
}

void data_model::deleteItem(int index)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }
  beginRemoveRows({}, index, index);
  _data.erase(_data.begin() + index);
  endRemoveRows();
}

void data_model::loadLast()
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

void data_model::load(QUrl const& url)
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

void data_model::storeLast()
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

void data_model::store(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();
  provider.ingredient(provider.installed_ingredient().begin()->first)->write(_data, path);
  QSettings settings;
  settings.setValue(::ingredientFile, url.path());
}

QString data_model::databasePath() const
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::ingredientFile).toString().toStdString()};
  return QString::fromStdString(_database_path.native());
}

std::optional<ingredient> data_model::findIngredient(boost::uuids::uuid const& id)
{
  auto item =
      std::find_if(std::begin(_data), std::end(_data), [id](auto item) { return item.id() == id; });
  if (item != std::end(_data)) {
    return *item;
  }
  return {};
}
} // namespace ingredients
} // namespace gui
} // namespace recipe
#include "ingredient_model.moc"
