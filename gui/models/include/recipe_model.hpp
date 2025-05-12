#pragma once
#include "amount_list_adapter.hpp"
#include "amounted_list_model.hpp"
#include "enum_adapter.hpp"
#include "recipe.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QUrl>

#include <boost/uuid/uuid.hpp>

#include <memory>
#include <optional>

namespace recipe {
class recipe;
class ingredient;
namespace gui {

class recipe_model : public QAbstractListModel
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

  int rowCount(QModelIndex const& parent = QModelIndex()) const override;

  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role) override;
  QHash<int, QByteArray> roleNames() const override;
  Qt::ItemFlags flags(QModelIndex const& index) const override;
  Q_INVOKABLE QStringList meal_types() const;
  Q_INVOKABLE QStringList amount_types() const;
  Q_INVOKABLE QStringList exportFormats() const;
  Q_INVOKABLE void exportRecipe(QUrl const& url, int format, int i);

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

  Q_INVOKABLE void bookRequested(QUrl const& path);
  Q_INVOKABLE QString databasePath() const;

  void setFinder(std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder);
  std::optional<recipe> findRecipe(boost::uuids::uuid const& id) const;

private:
  struct recipe_data
  {
    recipe object;
    bool favorite{false};
    std::shared_ptr<amounted_list_model> ingredient_model;
    recipe_data(recipe input, std::filesystem::path const& database_path);
  };
  std::vector<std::shared_ptr<recipe_data>> _data;
  std::filesystem::path _database_path;
  std::function<std::optional<ingredient>(boost::uuids::uuid const&)> _finder;
  enum_adapter<meal_t> _adapter;
  enum_adapter<amounted_ingredient::amount_t> _amounts_adapter;
};

} // namespace gui
} // namespace recipe
