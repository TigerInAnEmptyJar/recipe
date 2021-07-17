#pragma once

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include <boost/uuid/uuid.hpp>

#include <memory>
#include <optional>

namespace recipe {
class recipe;
class ingredient;
namespace gui {

class recipe_model : public QSortFilterProxyModel
{
  Q_OBJECT

public:
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
    id_role,
    favorite_role,
  };

public:
  recipe_model();

  Q_INVOKABLE QStringList meal_types() const;
  Q_INVOKABLE QStringList amount_types() const;

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
  bool lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const override;

private:
  std::shared_ptr<QAbstractListModel> _model;
};

} // namespace gui
} // namespace recipe
