#include "shopping_day_model.hpp"

#include "enum_adapter.hpp"

#include <iostream>

namespace recipe {
namespace gui {
namespace shopping_days {
class data_model : public QAbstractListModel
{
  Q_OBJECT

public:
  data_model(shopping_day& day);

  int rowCount(QModelIndex const& parent = QModelIndex()) const override;

  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  bool lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const;
  QStringList amount_types() const;

private:
  shopping_day _data;
  enum_adapter<amounted_ingredient::amount_t> _amounts_adapter;
};

} // namespace shopping_days

shopping_day_model::shopping_day_model(shopping_day& day)
    : QSortFilterProxyModel(), _model(std::make_shared<shopping_days::data_model>(day))
{
  setSourceModel(_model.get());
}

bool shopping_day_model::lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const
{
  if (auto model = std::dynamic_pointer_cast<shopping_days::data_model>(_model)) {
    return model->lessThan(lhs, rhs);
  }
  return false;
}

QStringList shopping_day_model::amount_types() const
{
  if (auto model = std::dynamic_pointer_cast<shopping_days::data_model>(_model)) {
    return model->amount_types();
  }
  return {};
}

namespace shopping_days {
data_model::data_model(shopping_day& day) : _data(day) {}

int data_model::rowCount(QModelIndex const&) const
{
  return std::distance(std::begin(_data), std::end(_data));
}

QVariant data_model::data(QModelIndex const& index, int role) const
{
  if (!index.isValid()) {
    return {};
  }
  auto element = std::begin(_data);
  std::advance(element, index.row());
  if (element >= std::end(_data)) {
    return {};
  }

  switch (role) {
  case shopping_day_model::Roles::name_role:
    return QString::fromStdString(element->first.base_ingredient().name());
  case shopping_day_model::Roles::needed_amount_role:
    break;
  case shopping_day_model::Roles::available_value_role:
    return element->second.second;
  case shopping_day_model::Roles::available_unit_role:
    return _amounts_adapter.to_string(element->second.first);
  }
  return {};
}

QHash<int, QByteArray> data_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(shopping_day_model::Roles::name_role, "name");
  roles.insert(shopping_day_model::Roles::needed_amount_role, "needed");
  roles.insert(shopping_day_model::Roles::available_value_role, "available_value");
  roles.insert(shopping_day_model::Roles::available_unit_role, "available_unit");
  return roles;
}

Qt::ItemFlags data_model::flags(QModelIndex const& index) const
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

bool data_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  if (!index.isValid()) {
    return false;
  }
  auto element = std::begin(_data);
  std::advance(element, index.row());
  if (element >= std::end(_data)) {
    return false;
  }

  switch (role) {
  case shopping_day_model::Roles::available_value_role:
    if (value.type() == QVariant::Double) {
      element->second.second = value.toFloat();
      dataChanged(index, index, {role});
    }
    break;
  case shopping_day_model::Roles::available_unit_role:
    break;
  }
  return false;
}

bool data_model::lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const
{
  if (!lhs.isValid() || !rhs.isValid()) {
    return false;
  }

  auto right = std::begin(_data);
  auto left = std::begin(_data);
  std::advance(right, rhs.row());
  std::advance(left, lhs.row());
  if (left >= std::end(_data) || right >= std::end(_data)) {
    return false;
  }

  if (left->first.base_ingredient().category() != right->first.base_ingredient().category()) {
    return left->first.base_ingredient().category() < right->first.base_ingredient().category();
  }
  return left->first.base_ingredient().category() < right->first.base_ingredient().category();
}

QStringList data_model::amount_types() const { return _amounts_adapter.all(); }

} // namespace shopping_days
} // namespace gui
} // namespace recipe
#include "shopping_day_model.moc"
