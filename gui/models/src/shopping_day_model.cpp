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
  shopping_day& _data;
  enum_adapter<amounted_ingredient::amount_t> _amounts_adapter;
  enum_adapter<ingredient::category_t> _category_adapter;
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
    return QString::fromStdString(std::get<0>(*element).base_ingredient().name());
  case shopping_day_model::Roles::needed_amount_role: {
    QString needed;
    for (auto item : std::get<0>(*element)) {
      needed += QString("%1 %2 + ").arg(item.second).arg(_amounts_adapter.to_string(item.first));
    }
    if (needed.length() > 0) {
      needed = needed.left(needed.length() - 3);
    }
    return needed;
  }
  case shopping_day_model::Roles::available_value_role:
    return std::get<2>(*element);
  case shopping_day_model::Roles::available_unit_role:
    return static_cast<int>(std::get<1>(*element));
  case shopping_day_model::Roles::category_role:
    return _category_adapter.to_string(std::get<0>(*element).base_ingredient().category());
  case shopping_day_model::Roles::check_all_role:
    return std::get<3>(*element);
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
  roles.insert(shopping_day_model::Roles::check_all_role, "check_all");
  roles.insert(shopping_day_model::Roles::category_role, "category");
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
    if (value.metaType().id() == QMetaType::Type::QString) {
      bool ok = true;
      auto v = value.toFloat(&ok);
      if (ok) {
        std::get<2>(*element) = v;
        dataChanged(index, index, {role});
        return true;
      }
      break;
    }
    break;
  case shopping_day_model::Roles::available_unit_role:
    if (value.metaType().id() == QMetaType::Type::Int) {
      std::get<1>(*element) = static_cast<amounted_ingredient::amount_t>(value.toInt());
      dataChanged(index, index, {role});
      return true;
    }
    break;
  case shopping_day_model::Roles::check_all_role:
    if (value.metaType().id() == QMetaType::Type::Bool) {
      std::get<3>(*element) = value.toBool();
      dataChanged(index, index, {role});
      return true;
    }
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

  if (std::get<0>(*left).base_ingredient().category() !=
      std::get<0>(*right).base_ingredient().category()) {
    return std::get<0>(*left).base_ingredient().category() <
           std::get<0>(*right).base_ingredient().category();
  }
  return std::get<0>(*left).base_ingredient().category() <
         std::get<0>(*right).base_ingredient().category();
}

QStringList data_model::amount_types() const { return _amounts_adapter.all(); }

} // namespace shopping_days
} // namespace gui
} // namespace recipe
#include "shopping_day_model.moc"
