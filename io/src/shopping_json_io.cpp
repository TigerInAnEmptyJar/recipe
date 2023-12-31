#include "shopping_json_io.h"
#include "amounted_json_io.hpp"
#include "shopping_day_generator.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fstream>

namespace {
QString const type_key{"file-type"};
QString const type_value{"shopping-list"};
QString const version_key{"version"};
constexpr int const version_value{0};
QString const content_key{"content"};

QString const key_list_name{"list-name"};
QString const key_list_id{"id"};
QString const key_days_item{"days"};

QString const key_day_name{"day-name"};
QString const key_ingredients{"ingredients"};
QString const key_ingredient{"ingredient"};
QString const key_at_home_unit{"unit"};
QString const key_at_home_value{"value"};
QString const key_at_home_all{"has-all"};
} // namespace

namespace recipe::io {

void shopping_json_io::write(shopping_list const& out, std::filesystem::path const& path) const
{
  auto result = serialize(out);

  std::ofstream output(path);
  output << result << std::flush;
}

std::optional<shopping_list> shopping_json_io::read(std::filesystem::path const& path,
                                                    finder_t ingredient_finder) const
{
  std::string file_content;
  std::ifstream input(path);
  while (!input.eof()) {
    char line[10000];
    input.getline(line, 10000);
    file_content.append(line);
  }
  return deserialize(file_content, ingredient_finder);
}

std::string shopping_json_io::serialize(shopping_list const& out) const
{
  QJsonArray array;
  QJsonObject object;
  object.insert(type_key, type_value);
  object.insert(version_key, version_value);
  object.insert(content_key, array);
  object.insert(content_key, to_object(out));
  return QJsonDocument(object).toJson().toStdString();
}

std::optional<shopping_list> shopping_json_io::deserialize(std::string const& text,
                                                           finder_t ingredient_finder) const
{
  auto doc = QJsonDocument::fromJson(QByteArray::fromStdString(text));
  if (!doc.isObject() || !doc.object().contains(content_key) ||
      !doc.object()[content_key].isObject()) {
    return {};
  }
  return list_from_object(doc.object()[content_key].toObject(), ingredient_finder);
}

QJsonObject shopping_json_io::to_object(shopping_list const& out) const
{
  QJsonObject item;
  item.insert(key_list_name, QString::fromStdString(out.name()));
  item.insert(key_list_id, QString::fromStdString(boost::uuids::to_string(out.id())));
  QJsonArray days;
  for (auto const& element : out) {
    days.append(to_object(element));
  }
  item.insert(key_days_item, days);
  return item;
}

QJsonObject shopping_json_io::to_object(shopping_day const& day) const
{
  QJsonObject item;
  item.insert(key_day_name, QString::fromStdString(day.name()));
  QJsonArray ingredients;
  io::amounted_json_io io;
  for (auto const& element : day) {
    QJsonObject ing;
    ing.insert(key_ingredient, io.toJsonObject(std::get<0>(element)));
    ing.insert(key_at_home_unit, static_cast<uint16_t>(std::get<1>(element)));
    ing.insert(key_at_home_value, std::get<2>(element));
    ing.insert(key_at_home_all, std::get<3>(element));
    ingredients.append(ing);
  }
  item.insert(key_ingredients, ingredients);
  return item;
}

std::optional<shopping_list> shopping_json_io::list_from_object(QJsonObject const& object,
                                                                finder_t ingredient_finder) const
{
  if (!(object.contains(key_days_item) && object[key_days_item].isArray()) ||
      !object.contains(key_list_name)) {
    return {};
  }
  std::vector<shopping_list::value_type> shoppingDays;
  for (auto const& element : object[key_days_item].toArray()) {
    if (!element.isObject()) {
      continue;
    }
    auto day = day_from_object(element.toObject(), ingredient_finder);
    if (day.has_value()) {
      shoppingDays.push_back(*day);
    }
  }
  boost::uuids::uuid id;
  if (object.contains(key_list_id)) {
    boost::uuids::string_generator gen;
    id = gen(object[key_list_id].toString().toStdString());
    if (id.is_nil()) {
      id = boost::uuids::random_generator{}();
    }
  }

  shopping_day_generator generator;
  auto list = generator.generate(shoppingDays, id);
  list.name(object[key_list_name].toString().toStdString());
  return list;
}

std::optional<shopping_day> shopping_json_io::day_from_object(QJsonObject const& object,
                                                              finder_t ingredient_finder) const
{
  if (!(object.contains(key_ingredients) && object[key_ingredients].isArray()) ||
      !object.contains(key_day_name)) {
    return {};
  }

  auto name = object[key_day_name].toString().toStdString();

  std::vector<shopping_day::value_type> items;
  io::amounted_json_io io;
  for (auto element : object[key_ingredients].toArray()) {
    if (!element.isObject()) {
      continue;
    }
    auto item = element.toObject();

    if (!(item.contains(key_ingredient) && item[key_ingredient].isObject()) ||
        !item.contains(key_at_home_unit) || !item.contains(key_at_home_value)) {
      continue;
    }
    auto ingredient = io.fromJsonObject(item[key_ingredient].toObject(), ingredient_finder);
    if (!ingredient.has_value()) {
      continue;
    }
    bool has_all = false;
    if (item.contains(key_at_home_all)) {
      has_all = item[key_at_home_all].toBool();
    }
    items.push_back(std::make_tuple(
        *ingredient, static_cast<amounted_ingredient::amount_t>(item[key_at_home_unit].toInt()),
        static_cast<float>(item[key_at_home_value].toDouble()), has_all));
  }
  shopping_day_generator generator;
  auto day = generator.generate(items);
  day.name(name);
  return day;
}

} // namespace recipe::io
