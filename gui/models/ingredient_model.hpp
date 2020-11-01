#pragma once

#include "ingredient.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

namespace recipe {
namespace gui {

class ingredient_model : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  enum IngredientRoles
  {
    name_role = Qt::UserRole + 1,
    category_role,
    amount_role,
    sectioned_role,
    image_path_role,
    image_role,
    id_role
  };

  ingredient_model();

  Q_INVOKABLE void addItem();
  Q_INVOKABLE void deleteItem(int index);

  Q_INVOKABLE void loadLast();
  Q_INVOKABLE void load(QUrl const& url);
  Q_INVOKABLE void storeLast();
  Q_INVOKABLE void store(QUrl const& url);

  Q_INVOKABLE QString databasePath() const;

  std::optional<ingredient> findIngredient(boost::uuids::uuid const& id);

private:
  bool lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const override;

  std::shared_ptr<QAbstractListModel> _model;
};
} // namespace gui
} // namespace recipe
