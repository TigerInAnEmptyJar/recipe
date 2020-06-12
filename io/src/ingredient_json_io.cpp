#include "ingredient_json_io.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fstream>

namespace {
QString const type_key{"file-type"};
QString const type_value{"ingredients"};
QString const version_key{"version"};
constexpr int const version_value{0};
QString const content_key{"content"};

QString const id_key{"id"};
QString const name_key{"name"};
QString const category_key{"category"};
QString const default_key{"default-amount"};
QString const sectioned_key{"sectioned"};
QString const image_key{"image-path"};
} // namespace

namespace recipe {
namespace io {

void ingredient_json_io::write(std::vector<ingredient> const& out,
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

std::optional<std::vector<ingredient>> ingredient_json_io::read(
    std::filesystem::path const& path) const
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

  std::vector<ingredient> output;
  for (auto element : content) {
    output.push_back(fromJsonObject(element.toObject()));
  }
  return output;
}

QJsonObject ingredient_json_io::toJsonObject(ingredient const& i) const
{
  QJsonObject out;

  out.insert(name_key, QString::fromStdString(i.name()));
  out.insert(category_key, static_cast<int>(i.category()));
  out.insert(default_key, static_cast<int>(i.default_amount()));
  out.insert(sectioned_key, i.sectioned());
  out.insert(image_key, QString::fromStdString(i.image_path().native()));
  out.insert(id_key, QString::fromStdString(boost::uuids::to_string(i.id())));

  return out;
}

ingredient ingredient_json_io::fromJsonObject(QJsonObject const& i) const
{
  recipe::ingredient out;

  if (i.contains(name_key)) {
    out.name(i[name_key].toString().toStdString());
  }
  if (i.contains(category_key)) {
    out.category(static_cast<recipe::ingredient::category_t>(i[category_key].toInt()));
  }
  if (i.contains(default_key)) {
    out.default_amount(static_cast<recipe::ingredient::amount_type>(i[default_key].toInt()));
  }
  if (i.contains(sectioned_key)) {
    out.sectioned(i[sectioned_key].toBool());
  }
  if (i.contains(image_key)) {
    out.image_path(i[image_key].toString().toStdString());
  }
  if (i.contains(id_key)) {
    boost::uuids::string_generator gen;
    out.id(gen(i[id_key].toString().toStdString()));
  }

  return out;
}

} // namespace io
} // namespace recipe
