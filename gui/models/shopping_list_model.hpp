#pragma once
#include "shopping_list.h"

#include <QAbstractListModel>

#include <boost/uuid/uuid.hpp>

#include <memory>
#include <optional>

namespace recipe {
class ingredient;
namespace gui {
class shopping_day_model;

class shopping_list_model : public QAbstractListModel
{
  Q_OBJECT

public:
  enum Roles
  {
    title_role = Qt::UserRole + 1,
    day_role,
  };

  using QAbstractListModel::QAbstractListModel;

  int rowCount(QModelIndex const& parent = QModelIndex()) const override;

  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  Q_INVOKABLE void loadLast();
  Q_INVOKABLE void load(QUrl const& url);
  Q_INVOKABLE void storeLast();
  Q_INVOKABLE void store(QUrl const& url);

  Q_INVOKABLE QString databasePath() const;

  void setFinder(std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder);

private:
  shopping_list _data;
  std::vector<std::shared_ptr<shopping_day_model>> _days;
  std::filesystem::path _database_path;
  std::function<std::optional<ingredient>(boost::uuids::uuid const&)> _finder;
};

} // namespace gui
} // namespace recipe
