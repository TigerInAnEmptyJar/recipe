#pragma once

#include "recipe.h"

#include "amounted_list_model.hpp"

#include <QAbstractListModel>

namespace recipe {
namespace gui {

class recipe_model : public QAbstractListModel
{
  Q_OBJECT

  enum RecipeRoles
  {
    title_role = Qt::UserRole + 1,
    type_role,
    servings_role,
    time_role,
    ingredient_role,
    instructions_role,
    image_path_role,
    image_role,
    fat_role,
    protein_role,
    carbo_role,
    calories_role,
    joules_role,
    source_role,
    tag_role,
    eater_role,
  };

public:
  using QAbstractListModel::QAbstractListModel;

  int rowCount(QModelIndex const& parent = QModelIndex()) const override;

  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE QStringList meal_types() const;
  Q_INVOKABLE QStringList amount_types() const;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  Q_INVOKABLE void addItem();
  Q_INVOKABLE void deleteItem(int index);
  Q_INVOKABLE void addTag(int index, QString const& tag);
  Q_INVOKABLE void removeTag(int index, QString const& tag);
  Q_INVOKABLE void addEater(int index, QString const& eater);
  Q_INVOKABLE void removeEater(int index, QString const& eater);
  Q_INVOKABLE void addIngredient(int index, QString const& id);

  Q_INVOKABLE void loadLast();
  Q_INVOKABLE void load(QUrl const& url);
  Q_INVOKABLE void storeLast();
  Q_INVOKABLE void store(QUrl const& url);

  Q_INVOKABLE QString databasePath() const;

  void setFinder(std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder);
  std::optional<recipe> findRecipe(boost::uuids::uuid const& id) const;

private:
  std::vector<std::pair<recipe, std::shared_ptr<amounted_list_model>>> _data;
  std::filesystem::path _database_path;
  std::function<std::optional<ingredient>(boost::uuids::uuid const&)> _finder;
};

} // namespace gui
} // namespace recipe
