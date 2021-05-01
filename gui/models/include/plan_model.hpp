#pragma once

#include "plan.h"
#include "shopping_list.h"

#include <QAbstractListModel>

namespace recipe {
namespace gui {

class plan_item_model;

class plan_model : public QAbstractListModel
{
  Q_OBJECT

  enum PlanRoles
  {
    name_role = Qt::UserRole + 1,
    shopping_role,
    subscribers_role,
    item_role,
  };

public:
  Q_PROPERTY(int days READ getDays NOTIFY daysChanged);
  Q_PROPERTY(int meals READ getMeals NOTIFY mealsChanged);
  Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged);

public:
  plan_model(std::function<std::optional<recipe>(boost::uuids::uuid const&)> finder);

  int rowCount(QModelIndex const& parent = QModelIndex()) const override;

  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  Q_INVOKABLE void newPlan(int ndays, int nmeals);
  Q_INVOKABLE void loadLast();
  Q_INVOKABLE void load(QUrl const& url);
  Q_INVOKABLE void store();
  Q_INVOKABLE void storeAs(QUrl const& url);
  Q_INVOKABLE QStringList exportFormats() const;
  Q_INVOKABLE void exportPlan(QUrl const& url, int format);

  Q_INVOKABLE QString databasePath() const;

  Q_INVOKABLE void addRecipe(int index, QString const& id);

  Q_INVOKABLE void addEater(QString const& eater);
  Q_INVOKABLE void removeEater(QString const& eater);
  Q_INVOKABLE QStringList eaters() const;

  Q_INVOKABLE void addSubscriber(int index, QString const& eater);
  Q_INVOKABLE void removeSubscriber(int index, QString const& eater);
  Q_INVOKABLE bool subscribed(int index, QString const& eater);

  Q_INVOKABLE shopping_list generateList() const;

  int getDays() const;
  int getMeals() const;
  QString getTitle() const;
  void setTitle(QString const& title);

Q_SIGNALS:
  void daysChanged(int days);
  void mealsChanged(int meals);
  void titleChanged(QString const& title);

private:
  plan _data;
  std::filesystem::path _database_path;
  std::vector<std::shared_ptr<plan_item_model>> _items;

  std::function<std::optional<recipe>(boost::uuids::uuid const&)> _finder;
};

} // namespace gui
} // namespace recipe
