#include "amounted_json_io.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fstream>

namespace {
QString const type_key{"file-type"};
QString const type_value{"shoppinglist"};
QString const version_key{"version"};
constexpr int const version_value{0};
QString const content_key{"content"};

QString const id_key{"id"};
QString const value_key{"value"};
QString const amount_key{"unit"};
QString const values_key{"values"};

QJsonObject toJson(recipe::amounted_ingredient const& i)
{
  QJsonObject out;

  out.insert(id_key, QString::fromStdString(boost::uuids::to_string(i.base_ingredient().id())));

  QJsonArray array;

  for (auto element : i) {
    QJsonObject object;
    object.insert(value_key, element.second);
    object.insert(amount_key, static_cast<int>(element.first));
    array.append(object);
  }

  out.insert(values_key, array);

  return out;
}

std::optional<recipe::amounted_ingredient> fromJson(
    QJsonObject const& i,
    std::function<std::optional<recipe::ingredient>(boost::uuids::uuid)> ingredient_finder)
{
  if (!i.contains(id_key)) {
    return {};
  }
  boost::uuids::string_generator gen;
  auto ingredient = ingredient_finder(gen(i[id_key].toString().toStdString()));
  if (!ingredient.has_value()) {
    return {};
  }
  recipe::amounted_ingredient out{*ingredient};

  if (i.contains(values_key)) {
    auto o = i[values_key];
    if (o.isArray()) {
      auto array = o.toArray();
      for (auto element : array) {
        if (element.isObject()) {
          auto object = element.toObject();
          if (object.contains(value_key) && object.contains(amount_key)) {
            out.add(static_cast<recipe::amounted_ingredient::amount_t>(object[amount_key].toInt()),
                    object[value_key].toDouble());
          }
        }
      }
    }
  }

  return out;
}
} // namespace

namespace recipe {
namespace io {

void amounted_json_io::write(std::vector<amounted_ingredient> const& out,
                             std::filesystem::path const& path) const
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

std::optional<std::vector<amounted_ingredient>> amounted_json_io::read(
    std::filesystem::path const& path,
    std::function<std::optional<recipe::ingredient>(boost::uuids::uuid)> ingredient_finder) const
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

  std::vector<amounted_ingredient> output;
  for (auto element : content) {
    auto object = fromJsonObject(element.toObject(), ingredient_finder);
    if (object.has_value()) {
      output.push_back(*object);
    }
  }
  return output;
}

QJsonObject amounted_json_io::toJsonObject(amounted_ingredient const& i) const
{
  QJsonObject out;

  out.insert(id_key, QString::fromStdString(boost::uuids::to_string(i.base_ingredient().id())));

  QJsonArray array;

  for (auto element : i) {
    QJsonObject object;
    object.insert(value_key, element.second);
    object.insert(amount_key, static_cast<int>(element.first));
    array.append(object);
  }

  out.insert(values_key, array);

  return out;
}

std::optional<amounted_ingredient> amounted_json_io::fromJsonObject(
    QJsonObject const& i,
    std::function<std::optional<recipe::ingredient>(boost::uuids::uuid)> ingredient_finder) const
{
  if (!i.contains(id_key)) {
    return {};
  }
  boost::uuids::string_generator gen;
  auto ingredient = ingredient_finder(gen(i[id_key].toString().toStdString()));
  if (!ingredient.has_value()) {
    return {};
  }
  recipe::amounted_ingredient out{*ingredient};

  if (i.contains(values_key)) {
    auto o = i[values_key];
    if (o.isArray()) {
      auto array = o.toArray();
      for (auto element : array) {
        if (element.isObject()) {
          auto object = element.toObject();
          if (object.contains(value_key) && object.contains(amount_key)) {
            out.add(static_cast<recipe::amounted_ingredient::amount_t>(object[amount_key].toInt()),
                    object[value_key].toDouble());
          }
        }
      }
    }
  }

  return out;
}
} // namespace io
} // namespace recipe
