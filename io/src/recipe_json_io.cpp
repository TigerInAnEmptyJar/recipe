#include "recipe_json_io.hpp"
#include "amounted_json_io.hpp"

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
QString const type_value{"recipes"};
QString const version_key{"version"};
constexpr int const version_value{0};
QString const content_key{"content"};

QString const id_key{"id"};
QString const title_key{"title"};
QString const meal_key{"meal-type"};
QString const servings_key{"servings"};
QString const instruction_key{"instructions"};
QString const time_key{"preparation-time"};
QString const source_key{"source"};
QString const image_key{"image-path"};
QString const cal_key{"calories"};
QString const jou_key{"joules"};
QString const fat_key{"fats"};
QString const prot_key{"proteins"};
QString const ch_key{"carbohydrates"};
QString const ingredients_key{"ingredients"};
QString const tag_key{"tags"};
QString const eater_key{"eaters"};

} // namespace

namespace recipe {
namespace io {

void recipe_json_io::write(std::vector<recipe> const& out, std::filesystem::path const& path) const
{
  QJsonArray array;
  for (auto& element : out) {
    array.append(toJsonObject(element));
  }
  QJsonObject object;
  object.insert(type_key, type_value);
  object.insert(version_key, version_value);
  object.insert(content_key, array);
  QJsonDocument document;
  document.setObject(object);

  std::ofstream output(path);
  output << document.toJson().toStdString() << std::flush;
}

std::optional<std::vector<recipe>> recipe_json_io::read(
    std::filesystem::path const& path,
    std::function<std::optional<ingredient>(boost::uuids::uuid)> ingredient_finder) const
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

  std::vector<recipe> output;
  for (auto element : content) {
    auto object = fromJsonObject(element.toObject(), ingredient_finder);
    if (object.has_value()) {
      output.push_back(*object);
    }
  }

  return output;
}

QJsonObject recipe_json_io::toJsonObject(recipe const& i) const
{
  QJsonObject out;

  out.insert(title_key, QString::fromStdString(i.title()));
  out.insert(meal_key, static_cast<int>(i.meal_type()));
  out.insert(servings_key, i.servings());
  out.insert(instruction_key, QString::fromStdString(i.instructions()));
  out.insert(time_key, static_cast<qint64>(i.preparation_time().count()));
  out.insert(source_key, QString::fromStdString(i.source()));
  out.insert(image_key, QString::fromStdString(i.image_path().native()));
  out.insert(cal_key, i.calories());
  out.insert(jou_key, i.joules());
  out.insert(fat_key, i.g_fat());
  out.insert(prot_key, i.g_proteins());
  out.insert(ch_key, i.g_carbohydrates());
  out.insert(id_key, QString::fromStdString(boost::uuids::to_string(i.id())));

  amounted_json_io io;
  QJsonArray array;
  for (auto element : i) {
    array.append(io.toJsonObject(element));
  }
  out.insert(ingredients_key, array);

  QJsonArray tagArray;
  for (auto element : i.tagList()) {
    tagArray.append(QString::fromStdString(element));
  }
  out.insert(tag_key, tagArray);

  QJsonArray eaterArray;
  for (auto element : i.eaterList()) {
    eaterArray.append(QString::fromStdString(element));
  }
  out.insert(eater_key, eaterArray);

  return out;
}

std::optional<recipe> recipe_json_io::fromJsonObject(
    QJsonObject const& i,
    std::function<std::optional<ingredient>(boost::uuids::uuid)> ingredient_finder) const
{
  recipe out;

  if (i.contains(title_key)) {
    out.title(i[title_key].toString().toStdString());
  }
  if (i.contains(meal_key)) {
    out.meal_type(static_cast<meal_t>(i[meal_key].toInt()));
  }
  if (i.contains(servings_key)) {
    out.servings(i[servings_key].toInt());
  }
  if (i.contains(instruction_key)) {
    out.instructions(i[instruction_key].toString().toStdString());
  }
  if (i.contains(time_key)) {
    out.preparation_time(std::chrono::minutes{i[time_key].toInt()});
  }
  if (i.contains(source_key)) {
    out.source(i[source_key].toString().toStdString());
  }
  if (i.contains(image_key)) {
    out.image_path(i[image_key].toString().toStdString());
  }
  if (i.contains(cal_key)) {
    out.calories(i[cal_key].toInt());
  }
  if (i.contains(jou_key)) {
    out.joules(i[jou_key].toInt());
  }
  if (i.contains(fat_key)) {
    out.g_fat(i[fat_key].toInt());
  }
  if (i.contains(prot_key)) {
    out.g_proteins(i[prot_key].toInt());
  }
  if (i.contains(ch_key)) {
    out.g_carbohydrates(i[ch_key].toInt());
  }
  if (i.contains(id_key)) {
    boost::uuids::string_generator gen;
    auto id = gen(i[id_key].toString().toStdString());
    if (id.is_nil()) {
      id = boost::uuids::random_generator{}();
    }
    out.id(id);
  }

  amounted_json_io io;
  if (i.contains(ingredients_key)) {
    auto o = i[ingredients_key];
    if (o.isArray()) {
      auto array = o.toArray();
      for (auto element : array) {
        if (element.isObject()) {
          auto object = io.fromJsonObject(element.toObject(), ingredient_finder);
          if (object.has_value()) {
            out.add(*object);
          }
        }
      }
    }
  }

  if (i.contains(tag_key)) {
    auto o = i[tag_key];
    if (o.isArray()) {
      auto array = o.toArray();
      for (auto element : array) {
        out.addTag(element.toString().toStdString());
      }
    }
  }

  if (i.contains(eater_key)) {
    auto o = i[eater_key];
    if (o.isArray()) {
      auto array = o.toArray();
      for (auto element : array) {
        out.addEater(element.toString().toStdString());
      }
    }
  }

  return out;
}

} // namespace io
} // namespace recipe
