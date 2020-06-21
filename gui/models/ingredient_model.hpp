#pragma once

#include "ingredient.h"

#include <QAbstractListModel>

namespace recipe {
namespace gui {

class ingredient_model : public QAbstractListModel
{
  Q_OBJECT

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

private:
  std::vector<ingredient> _data;
  std::filesystem::path _database_path;
};

} // namespace gui
} // namespace recipe
