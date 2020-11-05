#include "recipe_model.hpp"

#include "amount_list_adapter.hpp"
#include "amounted_list_model.hpp"
#include "enum_adapter.hpp"

#include "io_provider.h"
#include "recipe.h"

#include <QImage>
#include <QSettings>
#include <QUrl>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>

#include <iostream>

namespace {
QString const recipeFile = "last_recipe_file";
}

namespace recipe {
namespace gui {
namespace recipes {
class data_model : public QAbstractListModel
{
  Q_OBJECT

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
  bool lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const;

private:
  struct recipe_data
  {
    recipe object;
    std::shared_ptr<amounted_list_model> ingredient_model;
    recipe_data(recipe input, std::filesystem::path const& database_path);
  };
  std::vector<std::shared_ptr<recipe_data>> _data;
  std::filesystem::path _database_path;
  std::function<std::optional<ingredient>(boost::uuids::uuid const&)> _finder;
  enum_adapter<meal_t> _adapter;
  enum_adapter<amounted_ingredient::amount_t> _amounts_adapter;
};

} // namespace recipes

template <>
class amount_list_adapter_impl<recipe> : public amount_list_adapter
{
public:
  amount_list_adapter_impl(recipe& obj) : _object(obj) {}
  size_t size() override { return std::distance(std::begin(_object), std::end(_object)); }
  amounted_ingredient& at(size_t i) override { return *(_object.begin() + i); }
  amounted_ingredient const& at(size_t i) const override { return *(_object.begin() + i); }
  bool singleAmount() const override { return true; }
  bool removeAt(size_t i) override
  {
    if (i >= size()) {
      return false;
    }
    _object.remove(_object.begin() + i);
    return true;
  };

private:
  recipe& _object;
};

recipe_model::recipe_model()
    : QSortFilterProxyModel(), _model(std::make_shared<recipes::data_model>())
{
  setSourceModel(_model.get());
}

QStringList recipe_model::meal_types() const
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    return model->meal_types();
  }
  return {};
}

QStringList recipe_model::amount_types() const
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    return model->amount_types();
  }
  return {};
}

void recipe_model::addItem()
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->addItem();
  }
}

void recipe_model::deleteItem(int index)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->deleteItem(mapToSource(this->index(index, 0)).row());
  }
}

void recipe_model::addTag(int index, QString const& tag)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->addTag(mapToSource(this->index(index, 0)).row(), tag);
  }
}

void recipe_model::removeTag(int index, QString const& tag)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->removeTag(mapToSource(this->index(index, 0)).row(), tag);
  }
}

void recipe_model::addEater(int index, QString const& eater)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->addEater(mapToSource(this->index(index, 0)).row(), eater);
  }
}

void recipe_model::removeEater(int index, QString const& eater)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->removeEater(mapToSource(this->index(index, 0)).row(), eater);
  }
}

void recipe_model::addIngredient(int index, QString const& id)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->addIngredient(mapToSource(this->index(index, 0)).row(), id);
  }
}

void recipe_model::loadLast()
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->loadLast();
  }
}

void recipe_model::load(QUrl const& url)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->load(url);
  }
}

void recipe_model::storeLast()
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->storeLast();
  }
}

void recipe_model::store(QUrl const& url)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    model->store(url);
  }
}

QString recipe_model::databasePath() const
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    return model->databasePath();
  }
  return {};
}

void recipe_model::setFinder(
    std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder)
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    return model->setFinder(finder);
  }
}

std::optional<recipe> recipe_model::findRecipe(boost::uuids::uuid const& id) const
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    return model->findRecipe(id);
  }
  return {};
}

bool recipe_model::lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const
{
  if (auto model = std::dynamic_pointer_cast<recipes::data_model>(_model)) {
    return model->lessThan(lhs, rhs);
  }
  return false;
}

namespace recipes {
data_model::recipe_data::recipe_data(recipe input, std::filesystem::path const& database_path)
    : object(input)
{
  auto adapter = std::make_shared<amount_list_adapter_impl<recipe>>(object);
  ingredient_model = std::make_shared<amounted_list_model>(adapter, database_path);
}

int data_model::rowCount(QModelIndex const&) const { return _data.size(); }

QVariant data_model::data(QModelIndex const& index, int role) const
{
  if (!index.isValid() || static_cast<size_t>(index.row()) >= _data.size()) {
    return {};
  }

  switch (role) {
  case recipe_model::RecipeRoles::title_role:
    return QString::fromStdString(_data[index.row()]->object.title());
  case recipe_model::RecipeRoles::type_role: {
    return _adapter.to_string(_data[index.row()]->object.meal_type());
  }
  case recipe_model::RecipeRoles::servings_role:
    return _data[index.row()]->object.servings();
  case recipe_model::RecipeRoles::time_role:
    return static_cast<quint64>(_data[index.row()]->object.preparation_time().count());
  case recipe_model::RecipeRoles::ingredient_role:
    return QVariant::fromValue(_data[index.row()]->ingredient_model.get());
  case recipe_model::RecipeRoles::instructions_role:
    return QString::fromStdString(_data[index.row()]->object.instructions());
  case recipe_model::RecipeRoles::image_path_role: {
    auto path = _data[index.row()]->object.image_path();
    if (path.empty()) {
      return QVariant();
    }
    return QString::fromStdString(path.native());
  }
  case recipe_model::RecipeRoles::image_role: {
    auto path = _data[index.row()]->object.image_path();
    if (path.empty()) {
      return QVariant();
    }
    if (path.is_absolute()) {
      return "file:://" + QString::fromStdString(path.native());
    }
    return "file://" + QString::fromStdString((_database_path / path).native());
  }
  case recipe_model::RecipeRoles::fat_role:
    return _data[index.row()]->object.g_fat();
  case recipe_model::RecipeRoles::protein_role:
    return _data[index.row()]->object.g_proteins();
  case recipe_model::RecipeRoles::carbo_role:
    return _data[index.row()]->object.g_carbohydrates();
  case recipe_model::RecipeRoles::calories_role:
    return _data[index.row()]->object.calories();
  case recipe_model::RecipeRoles::joules_role:
    return _data[index.row()]->object.joules();
  case recipe_model::RecipeRoles::source_role:
    return QString::fromStdString(_data[index.row()]->object.source());
  case recipe_model::RecipeRoles::tag_role: {
    QStringList tags;
    for (auto tag : _data[index.row()]->object.tagList()) {
      tags.append(QString::fromStdString(tag));
    }
    return tags;
  }
  case recipe_model::RecipeRoles::eater_role: {
    QStringList eaters;
    for (auto eater : _data[index.row()]->object.eaterList()) {
      eaters.append(QString::fromStdString(eater));
    }
    return eaters;
  }
  }
  return {};
}

bool data_model::lessThan(QModelIndex const& lhs, QModelIndex const& rhs) const
{
  if (!lhs.isValid() || !rhs.isValid() || static_cast<size_t>(lhs.row()) > _data.size() ||
      static_cast<size_t>(rhs.row()) > _data.size()) {
    return false;
  }
  auto& left = _data[lhs.row()];
  auto& right = _data[rhs.row()];
  if (left->object.meal_type() != right->object.meal_type()) {
    return left->object.meal_type() < right->object.meal_type();
  }
  return left->object.title() < right->object.title();
}

QHash<int, QByteArray> data_model::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(recipe_model::RecipeRoles::title_role, "title");
  roles.insert(recipe_model::RecipeRoles::type_role, "meal_type");
  roles.insert(recipe_model::RecipeRoles::servings_role, "servings");
  roles.insert(recipe_model::RecipeRoles::time_role, "preparation_time");
  roles.insert(recipe_model::RecipeRoles::ingredient_role, "ingredient");
  roles.insert(recipe_model::RecipeRoles::instructions_role, "instructions");
  roles.insert(recipe_model::RecipeRoles::image_path_role, "image_path");
  roles.insert(recipe_model::RecipeRoles::image_role, "image");
  roles.insert(recipe_model::RecipeRoles::fat_role, "fat");
  roles.insert(recipe_model::RecipeRoles::protein_role, "protein");
  roles.insert(recipe_model::RecipeRoles::carbo_role, "carbohydrates");
  roles.insert(recipe_model::RecipeRoles::calories_role, "calories");
  roles.insert(recipe_model::RecipeRoles::joules_role, "joules");
  roles.insert(recipe_model::RecipeRoles::source_role, "source");
  roles.insert(recipe_model::RecipeRoles::tag_role, "tags");
  roles.insert(recipe_model::RecipeRoles::eater_role, "eaters");
  return roles;
}

QStringList data_model::meal_types() const { return _adapter.all(); }

QStringList data_model::amount_types() const { return _amounts_adapter.all(); }

Qt::ItemFlags data_model::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool data_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return false;
  }
  switch (role) {
  case recipe_model::RecipeRoles::title_role:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position]->object.title(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::type_role:
    if (auto item = _adapter.to_enum(value)) {
      _data[position]->object.meal_type(*item);
      dataChanged(index, index, {role});
      return true;
    }
    return false;
  case recipe_model::RecipeRoles::servings_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position]->object.servings(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::time_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position]->object.preparation_time(std::chrono::minutes{value.toInt()});
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::instructions_role:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position]->object.instructions(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::image_path_role: {
    if (value.type() != QVariant::String) {
      return false;
    }
    auto val = value.toString().toStdString();
    if (val.starts_with("file://")) {
      val = val.substr(7);
    }
    auto image_path = std::filesystem::path{val};
    try {
      if (std::filesystem::exists(image_path)) {
        _data[position]->object.image_path(std::filesystem::relative(image_path, _database_path));
        dataChanged(index, index, {role, recipe_model::RecipeRoles::image_role});
        return true;
      }
    } catch (std::filesystem::filesystem_error const& e) {
      std::cout << "Caught exception while accessing filesystem: " << e.what() << std::endl;
    }
    return false;
  }
  case recipe_model::RecipeRoles::image_role:
    break;
  case recipe_model::RecipeRoles::fat_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position]->object.g_fat(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::protein_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position]->object.g_proteins(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::carbo_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position]->object.g_carbohydrates(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::calories_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position]->object.calories(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::joules_role:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position]->object.joules(value.toInt());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::source_role:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position]->object.source(value.toString().toStdString());
    dataChanged(index, index, {role});
    return true;
  case recipe_model::RecipeRoles::ingredient_role:
    break;
  case recipe_model::RecipeRoles::tag_role:
  case recipe_model::RecipeRoles::eater_role:
    break;
  }
  return false;
}

void data_model::addItem()
{
  beginInsertRows({}, _data.size(), _data.size());
  _data.push_back(std::make_shared<recipe_data>(recipe{}, _database_path));
  _data.back()->object.id(boost::uuids::random_generator{}());
  endInsertRows();
}

void data_model::deleteItem(int index)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }
  beginRemoveRows({}, index, index);
  _data.erase(_data.begin() + index);
  endRemoveRows();
}

void data_model::addTag(int index, QString const& tag)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  _data[index]->object.addTag(tag.toStdString());
  dataChanged(this->index(index), this->index(index), {recipe_model::RecipeRoles::tag_role});
}

void data_model::removeTag(int index, QString const& tag)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  _data[index]->object.removeTag(tag.toStdString());
  dataChanged(this->index(index), this->index(index), {recipe_model::RecipeRoles::tag_role});
}

void data_model::addEater(int index, QString const& eater)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  _data[index]->object.addEater(eater.toStdString());
  dataChanged(this->index(index), this->index(index), {recipe_model::RecipeRoles::eater_role});
}

void data_model::removeEater(int index, QString const& eater)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  _data[index]->object.removeEater(eater.toStdString());
  dataChanged(this->index(index), this->index(index), {recipe_model::RecipeRoles::eater_role});
}

void data_model::addIngredient(int index, QString const& id)
{
  if (static_cast<size_t>(index) >= _data.size() || index < 0) {
    return;
  }

  boost::uuids::string_generator gen;
  auto ingredient = _finder(gen(id.toStdString()));
  if (ingredient.has_value()) {
    _data[index]->ingredient_model->startModelChange();
    amounted_ingredient element{*ingredient};
    switch (ingredient->default_amount()) {
    case ingredient::amount_type::mass:
      element.add(amounted_ingredient::amount_t::grams, 100);
      break;
    case ingredient::amount_type::volume:
      element.add(amounted_ingredient::amount_t::milliliter, 100);
      break;
    case ingredient::amount_type::piece:
      element.add(amounted_ingredient::amount_t::piece, 1);
      break;
    }
    _data[index]->object.add(element);
    _data[index]->ingredient_model->endModelChange();
    dataChanged(this->index(index), this->index(index),
                {recipe_model::RecipeRoles::ingredient_role});
  }
}

void data_model::loadLast()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::recipeFile).toString().toStdString()};
  _database_path = path.parent_path();
  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();

  auto data = provider.recipe(provider.installed_recipe().begin()->first)->read(path, _finder);
  if (!data.has_value()) {
    return;
  }

  beginResetModel();
  _data.clear();
  std::for_each(std::begin(*data), std::end(*data), [this](auto const& item) {
    _data.push_back(std::make_shared<recipe_data>(item, _database_path));
  });
  endResetModel();
}

void data_model::load(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();

  if (!std::filesystem::exists(path)) {
    std::cout << "Loading not possible: url doesn't exist " << path.native() << std::endl;
    return;
  }
  io::io_provider provider;
  provider.setup();
  auto data = provider.recipe(provider.installed_recipe().begin()->first)->read(path, _finder);
  if (!data.has_value()) {
    return;
  }
  QSettings settings;
  settings.setValue(::recipeFile, url.path());
  _database_path = path.parent_path();

  beginResetModel();
  _data.clear();
  std::for_each(std::begin(*data), std::end(*data), [this](auto const& item) {
    _data.push_back(std::make_shared<recipe_data>(item, _database_path));
  });
  endResetModel();
}

void data_model::storeLast()
{
  QSettings settings;
  auto path = std::filesystem::path{settings.value(::recipeFile).toString().toStdString()};
  if (!std::filesystem::exists(path)) {
    std::cout << "Save to existing file not possible: url doesn't exist " << path.native()
              << std::endl;
    return;
  }
  _database_path = path.parent_path();
  io::io_provider provider;
  provider.setup();
  std::vector<recipe> out;
  std::transform(std::begin(_data), std::end(_data), std::back_inserter(out),
                 [](auto const& item) { return item->object; });
  provider.recipe(provider.installed_recipe().begin()->first)->write(out, path);
}

void data_model::store(QUrl const& url)
{
  std::filesystem::path path = url.path().toStdString();
  io::io_provider provider;
  provider.setup();
  std::vector<recipe> out;
  std::transform(std::begin(_data), std::end(_data), std::back_inserter(out),
                 [](auto const& item) { return item->object; });
  provider.recipe(provider.installed_recipe().begin()->first)->write(out, path);
  QSettings settings;
  settings.setValue(::recipeFile, url.path());
}

QString data_model::databasePath() const { return QString::fromStdString(_database_path.native()); }

void data_model::setFinder(
    std::function<std::optional<ingredient>(boost::uuids::uuid const&)> finder)
{
  _finder = finder;
}

std::optional<recipe> data_model::findRecipe(boost::uuids::uuid const& id) const
{
  auto element = std::find_if(std::begin(_data), std::end(_data),
                              [id](auto item) { return item->object.id() == id; });
  if (element != std::end(_data)) {
    return (*element)->object;
  }
  return {};
}
} // namespace recipes
} // namespace gui
} // namespace recipe
#include "recipe_model.moc"
