#pragma once

#include "shopping_day.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include <memory>

namespace recipe {
class ingredient;
namespace gui {

class shopping_day_model : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  enum Roles
  {
    name_role = Qt::UserRole + 1,
    needed_amount_role,
    available_value_role,
    available_unit_role,
  };

public:
  shopping_day_model(shopping_day& day);

  Q_INVOKABLE QStringList amount_types() const;

  bool lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const override;

private:
  std::shared_ptr<QAbstractListModel> _model;
};

} // namespace gui
} // namespace recipe
