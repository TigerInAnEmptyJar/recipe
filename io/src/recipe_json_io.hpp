#pragma once

#include "recipe_io.h"

class QJsonObject;

namespace recipe {
namespace io {

class recipe_json_io : public recipe_io
{
public:
  void write(std::vector<recipe> const& out, std::filesystem::path const& path) const override;
  void write(std::vector<recipe> const& out, std::ostream & path) const override;

  std::optional<std::vector<recipe>> read(std::filesystem::path const& path,
                                          finder_t ingredient_finder) const override;
  QJsonObject toJsonObject(recipe const& i) const;
  std::optional<recipe> fromJsonObject(QJsonObject const& i, finder_t ingredient_finder) const;
};

} // namespace io
} // namespace recipe
