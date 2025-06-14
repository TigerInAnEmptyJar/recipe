#include "amounted_list_model.hpp"

#include <boost/uuid/uuid_io.hpp>

namespace recipe {
namespace gui {

amounted_list_model::amounted_list_model(std::shared_ptr<amount_list_adapter> adapter,
                                         std::filesystem::path const& db_path)
    : _database_path(db_path), _adapter(adapter)
{
}

int amounted_list_model::rowCount(const QModelIndex&) const { return _adapter->size(); }

QVariant amounted_list_model::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || static_cast<size_t>(index.row()) >= _adapter->size()) {
    return {};
  }

  switch (role) {
  case IngredientRoles::name_role:
    return QString::fromStdString(_adapter->at(index.row()).base_ingredient().name());
  case IngredientRoles::amounts_role: {
    return {};
  }
  case IngredientRoles::image_role: {
    auto path = _adapter->at(index.row()).base_ingredient().image_path();
    if (path.empty()) {
      return QVariant();
    }
    if (path.is_absolute()) {
      return "file://" + QString::fromStdString(path.native());
      ;
    }
    return "file://" + QString::fromStdString((_database_path / path).native());
  }
  case IngredientRoles::amount_count_role:
    return static_cast<int>(
        std::distance(_adapter->at(index.row()).begin(), _adapter->at(index.row()).end()));
  case IngredientRoles::amount_value_role:
    if (_adapter->at(index.row()).begin() == _adapter->at(index.row()).end()) {
      return {};
    }
    return _adapter->at(index.row()).begin()->second;
  case IngredientRoles::amount_amount_role: {
    return _amount_adapter.to_string(_adapter->at(index.row()).begin()->first);
  }
  case IngredientRoles::amount_amount_index_role:
    return static_cast<int>(_adapter->at(index.row()).begin()->first);
  case IngredientRoles::id_role:
    return QString::fromStdString(
        boost::uuids::to_string(_adapter->at(index.row()).base_ingredient().id()));
  }
  return {};
}

QHash<int, QByteArray> amounted_list_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(IngredientRoles::name_role, "name");
  roles.insert(IngredientRoles::image_role, "image");
  roles.insert(IngredientRoles::id_role, "id");
  roles.insert(IngredientRoles::amounts_role, "amounts");
  roles.insert(IngredientRoles::amount_count_role, "count");
  roles.insert(IngredientRoles::amount_value_role, "value");
  roles.insert(IngredientRoles::amount_amount_role, "amount");
  roles.insert(IngredientRoles::amount_amount_index_role, "amountIndex");
  return roles;
}

Qt::ItemFlags amounted_list_model::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _adapter->size()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool amounted_list_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _adapter->size()) {
    return false;
  }
  auto& ing = _adapter->at(index.row());
  switch (role) {
  case IngredientRoles::image_role:
  case IngredientRoles::id_role:
  case IngredientRoles::name_role:
  case IngredientRoles::amount_count_role:
  case IngredientRoles::amounts_role:
    return false;
  case amount_value_role: {
    if ((value.metaType() != QMetaType{QMetaType::Type::QString}) || !_adapter->singleAmount() ||
        ing.begin() == ing.end()) {
      return false;
    }
    ing.begin()->second = value.toString().toDouble();
    dataChanged(index, index, {amount_value_role});
    return true;
  }
  case amount_amount_role: {
    if (value.metaType() != QMetaType{QMetaType::Type::Int} || !_adapter->singleAmount() || ing.begin() == ing.end()) {
      return false;
    }
    if (value.toInt() <= 12) {
      auto v = ing.begin()->second;
      ing.remove(ing.begin());
      ing.add(static_cast<amounted_ingredient::amount_t>(value.toInt()), v);
    }
    dataChanged(index, index, {amount_amount_role});
    return true;
  }
  }
  return false;
}

void amounted_list_model::addItem() {}

void amounted_list_model::deleteItem(int i)
{
  beginRemoveRows(QModelIndex{}, i, i);
  _adapter->removeAt(i);
  endRemoveRows();
}

QStringList amounted_list_model::amountList() const { return _amount_adapter.all(); }

void amounted_list_model::startModelChange() { beginResetModel(); }

void amounted_list_model::endModelChange() { endResetModel(); }

void amounted_list_model::loadLast() {}

void amounted_list_model::load(QUrl const&) {}

void amounted_list_model::storeLast() {}

void amounted_list_model::store(QUrl const&) {}

QString amounted_list_model::databasePath() const { return {}; }

void amounted_list_model::setFinder(
    std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder)
{
  _finder = finder;
}

} // namespace gui
} // namespace recipe
