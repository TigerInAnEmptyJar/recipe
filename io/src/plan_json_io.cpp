#include "plan_json_io.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fstream>

namespace {
QString const type_key{"file-type"};
QString const type_value{"plan"};
QString const version_key{"version"};
constexpr int const version_value{1};
constexpr std::array<int, 2> supported_versions{0, 1};
QString const content_key{"content"};

QString const key_item_name{"item-name"};
QString const key_item_id{"recipe-id"};
QString const key_item_recipes{"recipe"};
QString const key_item_full_recipes{"full-recipe"};
QString const key_item_subscriber{"subscribers"};
QString const key_item_before{"doShopBefore"};
QString const key_plan_name{"plan-name"};
QString const key_plan_id{"id"};
QString const key_plan_days{"days"};
QString const key_plan_meals{"meals"};
QString const key_plan_items{"items"};
QString const key_plan_eaters{"eaters"};
} // namespace

namespace recipe {
namespace io {

void plan_json_io::write(plan const& out, std::filesystem::path const& path) const
{
  QJsonArray array;
  array.append(toJsonObject<version_value>(out));

  QJsonObject object;
  object.insert(type_key, type_value);
  object.insert(version_key, version_value);
  object.insert(content_key, array);
  QJsonDocument document;
  document.setObject(object);

  std::ofstream output(path);
  output << document.toJson().toStdString() << std::flush;
}

std::optional<plan> plan_json_io::read(std::filesystem::path const& path,
                                       finder_t recipe_finder) const
{
  std::string file_content;
  std::ifstream input(path);
  while (!input.eof()) {
    char line[10000];
    input.getline(line, 10000);
    file_content.append(line);
  }
  auto document = QJsonDocument::fromJson(QByteArray::fromStdString(file_content));

  if (!document.isObject()) {
    return {};
  }

  auto object = document.object();

  if (!object.contains(type_key) || object[type_key].toString() != type_value) {
    return {};
  }
  auto version = version_value;
  if (!object.contains(version_key)) {
    return {};
  }
  version = object[version_key].toInt();
  if (std::find(std::begin(supported_versions), std::end(supported_versions), version) ==
      std::end(supported_versions)) {
    return {};
  }
  if (!object.contains(content_key) || !object[content_key].isArray()) {
    return {};
  }
  auto content = object[content_key].toArray();

  auto reader =
      [version,
       this]() -> std::function<std::optional<plan>(QJsonObject const& i, finder_t recipe_finder)> {
    if (version == supported_versions[0]) {
      return [this](QJsonObject const& i, finder_t recipe_finder) {
        return planFromJsonObject<0>(i, recipe_finder);
      };
    }
    if (version == supported_versions[1]) {
      return [this](QJsonObject const& i, finder_t recipe_finder) {
        return planFromJsonObject<1>(i, recipe_finder);
      };
    }
    return [this](QJsonObject const&, finder_t) { return std::optional<plan>(); };
  }();
  std::vector<plan> output;
  for (auto element : content) {
    auto object = reader(element.toObject(), recipe_finder);
    if (object.has_value()) {
      output.push_back(*object);
    }
  }

  if (output.size() != 1) {
    return {};
  }
  return output[0];
}

template <int version>
QJsonObject plan_json_io::toJsonObject(plan_item const& i) const
{
  QJsonObject object;
  QJsonArray recipes;
  QJsonArray subscribers;
  for (auto it = i.begin(); it != i.end(); ++it) {
    auto id = QString::fromStdString(boost::uuids::to_string(it->item().id()));
    if constexpr (version == 0) {
      recipes.append(id);
      QJsonArray subscribers;
      for (auto const& sub : it->subscribers()) {
        if (!subscribers.contains(QString::fromStdString(sub))) {
          subscribers.append(QString::fromStdString(sub));
        }
      }
    } else if constexpr (version == 1) {
      QJsonObject recipeObject;
      QJsonArray subscribers;
      for (auto const& sub : it->subscribers()) {
        subscribers.append(QString::fromStdString(sub));
      }
      recipeObject.insert(key_item_subscriber, subscribers);
      recipeObject.insert(key_item_id, id);
      recipes.append(recipeObject);
    }
  }
  QJsonArray full_recipes;
  for (auto it = i.begin_full(); it != i.end_full(); it++) {
    full_recipes.append(QString::fromStdString(boost::uuids::to_string(it->id())));
  }
  if constexpr (version == 0) {
    object.insert(key_item_subscriber, subscribers);
  }

  object.insert(key_item_recipes, recipes);
  object.insert(key_item_full_recipes, full_recipes);
  object.insert(key_item_name, QString::fromStdString(i.name()));
  object.insert(key_item_before, i.shoppingBefore());

  return object;
}

template <int version>
std::optional<plan_item> plan_json_io::itemFromJsonObject(QJsonObject const& i,
                                                          finder_t recipe_finder) const
{
  plan_item item;

  if (i.contains(key_item_name)) {
    item.name(i[key_item_name].toString().toStdString());
  } else {
    return {};
  }

  if (i.contains(key_item_before)) {
    item.shoppingBefore(i[key_item_before].toBool());
  }

  if (i.contains(key_item_recipes) && i[key_item_recipes].isArray()) {
    QJsonArray recipes = i[key_item_recipes].toArray();
    boost::uuids::string_generator gen;
    for (auto rec : recipes) {
      if constexpr (version == 0) {
        auto id = gen(rec.toString().toStdString());
        auto recipe = recipe_finder(id);
        if (recipe.has_value()) {
          meal_item meal{*recipe};
          if (i.contains(key_item_subscriber) && i[key_item_subscriber].isArray()) {
            QJsonArray subscribers = i[key_item_subscriber].toArray();
            for (auto sub : subscribers) {
              meal.add(sub.toString().toStdString());
            }
          }
          item.add(meal);
        }
      } else if (version == 1) {
        QJsonObject recObject = rec.toObject();
        if (!recObject.contains(key_item_id)) {
          continue;
        }
        auto id = gen(recObject[key_item_id].toString().toStdString());
        auto recipe = recipe_finder(id);
        if (recipe.has_value()) {
          meal_item meal{*recipe};
          if (recObject.contains(key_item_subscriber) && recObject[key_item_subscriber].isArray()) {
            QJsonArray subscribers = recObject[key_item_subscriber].toArray();
            for (auto sub : subscribers) {
              meal.add(sub.toString().toStdString());
            }
          }
          item.add(meal);
        }
      }
    }
  }

  if (i.contains(key_item_full_recipes) && i[key_item_full_recipes].isArray()) {
    QJsonArray recipes = i[key_item_full_recipes].toArray();
    boost::uuids::string_generator gen;
    for (auto rec : recipes) {
      auto id = gen(rec.toString().toStdString());
      auto recipe = recipe_finder(id);
      if (recipe.has_value()) {
        item.addFullRecipe(*recipe);
      }
    }
  }

  if (i.contains(key_item_subscriber) && i[key_item_subscriber].isArray()) {
    QJsonArray subscribers = i[key_item_subscriber].toArray();
    for (auto sub : subscribers) {
      for (auto& rec : item) {
        rec.add(sub.toString().toStdString());
      }
    }
  }

  return item;
}

template <int version>
QJsonObject plan_json_io::toJsonObject(plan const& i) const
{
  QJsonObject object;

  object.insert(key_plan_name, QString::fromStdString(i.name()));
  object.insert(key_plan_days, static_cast<int>(i.days()));
  object.insert(key_plan_meals, static_cast<int>(i.meals()));
  object.insert(key_plan_id, QString::fromStdString(boost::uuids::to_string(i.id())));
  QJsonArray eaters;
  for (auto const& item : i.eaterList()) {
    eaters.append(QString::fromStdString(item));
  }
  object.insert(key_plan_eaters, eaters);

  QJsonArray items;
  for (auto const& item : i) {
    items.append(toJsonObject<version>(item));
  }
  object.insert(key_plan_items, items);

  return object;
}

template <int version>
std::optional<plan> plan_json_io::planFromJsonObject(QJsonObject const& i,
                                                     finder_t recipe_finder) const
{
  if (!(i.contains(key_plan_name) && i.contains(key_plan_days) && i.contains(key_plan_meals))) {
    return {};
  }

  plan object{i[key_plan_name].toString().toStdString(),
              static_cast<size_t>(i[key_plan_days].toInt()),
              static_cast<size_t>(i[key_plan_meals].toInt())};

  if (i.contains(key_plan_id)) {
    boost::uuids::string_generator gen;
    auto id = gen(i[key_plan_id].toString().toStdString());
    if (id.is_nil()) {
      id = boost::uuids::random_generator{}();
    }
    object.id(id);
  }

  QJsonArray items = i[key_plan_items].toArray();
  if (items.size() != object.days() * object.meals()) {
    return {};
  }

  if (i.contains(key_plan_eaters)) {
    QJsonArray eaters = i[key_plan_eaters].toArray();
    for (auto const& item : eaters) {
      object.addEater(item.toString().toStdString());
    }
  }

  int count = 0;
  for (auto& item : object) {
    auto element = itemFromJsonObject<version>(items.at(count++).toObject(), recipe_finder);
    if (element.has_value()) {
      item = *element;
    }
  }

  return object;
}

template QJsonObject plan_json_io::toJsonObject<0>(plan_item const& i) const;
template QJsonObject plan_json_io::toJsonObject<1>(plan_item const& i) const;
template QJsonObject plan_json_io::toJsonObject<1>(plan const& i) const;
template std::optional<plan_item> plan_json_io::itemFromJsonObject<0>(QJsonObject const& i,
                                                                      finder_t recipe_finder) const;
template std::optional<plan_item> plan_json_io::itemFromJsonObject<1>(QJsonObject const& i,
                                                                      finder_t recipe_finder) const;
} // namespace io
} // namespace recipe
