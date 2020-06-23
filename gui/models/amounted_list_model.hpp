#pragma once

#include "amounted_ingredient.h"

#include "amount_list_adapter.hpp"

#include <QAbstractListModel>

namespace recipe {
namespace gui {

class amounted_list_model : public QAbstractListModel
{
  Q_OBJECT

  enum IngredientRoles
  {
    name_role = Qt::UserRole + 1,
    image_role,
    id_role,
    amounts_role,
    amount_count_role,
    amount_value_role,
    amount_amount_role,
    amount_amount_index_role,
  };

public:
  amounted_list_model(std::shared_ptr<amount_list_adapter> adapter,
                      std::filesystem::path const& db_path);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  Q_INVOKABLE void addItem();
  Q_INVOKABLE void deleteItem(int index);
  Q_INVOKABLE QStringList amountList() const;

  void startModelChange();
  void endModelChange();

  Q_INVOKABLE void loadLast();
  Q_INVOKABLE void load(QUrl const& url);
  Q_INVOKABLE void storeLast();
  Q_INVOKABLE void store(QUrl const& url);

  Q_INVOKABLE QString databasePath() const;

  void setFinder(std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder);

private:
  std::filesystem::path _database_path;
  std::function<std::optional<ingredient>(boost::uuids::uuid const&)> _finder;
  std::shared_ptr<amount_list_adapter> _adapter;
};

} // namespace gui
} // namespace recipe
