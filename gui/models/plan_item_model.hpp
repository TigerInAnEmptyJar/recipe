#pragma once

#include "plan.h"

#include <QAbstractListModel>

namespace recipe {
namespace gui {

class plan_item_model : public QAbstractListModel
{
  Q_OBJECT

  enum ItemRoles
  {
    title_role = Qt::UserRole + 1,
    image_path_role,
    image_role,
  };

public:
  plan_item_model(plan_item* data,
                  std::function<std::optional<recipe>(boost::uuids::uuid const&)> finder);

  int rowCount(QModelIndex const& parent = QModelIndex()) const override;

  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void addRecipe(QString const& recipe);
  Q_INVOKABLE void deleteRecipe(int index);
  Q_INVOKABLE void addSubscriber(QString const& eater);
  Q_INVOKABLE void removeSubscriber(QString const& eater);

  Q_INVOKABLE QString databasePath() const;

private:
  plan_item* _data;
  std::filesystem::path _database_path;

  std::function<std::optional<recipe>(boost::uuids::uuid const&)> _finder;
};

} // namespace gui
} // namespace recipe