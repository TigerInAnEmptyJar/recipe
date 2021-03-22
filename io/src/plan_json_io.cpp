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
constexpr int const version_value{0};
QString const content_key{"content"};

QString const key_item_name{"item-name"};
QString const key_item_recipes{"recipe"};
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
  array.append(toJsonObject(out));

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
  if (!object.contains(version_key) || object[version_key].toInt() != version_value) {
    return {};
  }
  if (!object.contains(content_key) || !object[content_key].isArray()) {
    return {};
  }
  auto content = object[content_key].toArray();

  std::vector<plan> output;
  for (auto element : content) {
    auto object = planFromJsonObject(element.toObject(), recipe_finder);
    if (object.has_value()) {
      output.push_back(*object);
    }
  }

  if (output.size() != 1) {
    return {};
  }
  return output[0];
}

QJsonObject plan_json_io::toJsonObject(plan_item const& i) const
{
  QJsonArray subscribers;
  for (auto const& sub : i.subscribers()) {
    subscribers.append(QString::fromStdString(sub));
  }
  QJsonArray recipes;
  for (auto it = i.begin(); it != i.end(); ++it) {
    recipes.append(QString::fromStdString(boost::uuids::to_string(it->id())));
  }

  QJsonObject object;

  object.insert(key_item_recipes, recipes);
  object.insert(key_item_subscriber, subscribers);
  object.insert(key_item_name, QString::fromStdString(i.name()));
  object.insert(key_item_before, i.shoppingBefore());

  return object;
}

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

  if (i.contains(key_item_subscriber) && i[key_item_subscriber].isArray()) {
    QJsonArray subscribers = i[key_item_subscriber].toArray();
    for (auto sub : subscribers) {
      item.add(sub.toString().toStdString());
    }
  }

  if (i.contains(key_item_recipes) && i[key_item_recipes].isArray()) {
    QJsonArray recipes = i[key_item_recipes].toArray();
    boost::uuids::string_generator gen;
    for (auto rec : recipes) {
      auto id = gen(rec.toString().toStdString());
      auto recipe = recipe_finder(id);
      if (recipe.has_value()) {
        item.add(*recipe);
      }
    }
  }

  return item;
}

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
    items.append(toJsonObject(item));
  }
  object.insert(key_plan_items, items);

  return object;
}

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
    auto element = itemFromJsonObject(items.at(count++).toObject(), recipe_finder);
    if (element.has_value()) {
      item = *element;
    }
  }

  return object;
}

} // namespace io
} // namespace recipe
