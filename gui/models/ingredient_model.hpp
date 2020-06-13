#pragma once

#include "ingredient.h"

#include <QAbstractListModel>

namespace recipe {
namespace gui {

class ingredient_model : public QAbstractListModel
{
  Q_OBJECT

  enum Roles
  {
    name_role = Qt::UserRole + 1,
    category_role,
    amount_role,
    sectioned_role,
    image_path_role,
    image_role,
  };

public:
  using QAbstractListModel::QAbstractListModel;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void load(QUrl const& url);
  Q_INVOKABLE void store(QUrl const& url);

private:
  std::vector<ingredient> _data;
};

} // namespace gui
} // namespace recipe
