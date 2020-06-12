#pragma once

#include "ingredient_io.h"

#include <filesystem>
#include <optional>
#include <vector>

class QJsonObject;

namespace recipe {
namespace io {

class ingredient_json_io : public ingredient_io
{
public:
  void write(std::vector<ingredient> const& out, std::filesystem::path const& path) const override;
  std::optional<std::vector<ingredient>> read(std::filesystem::path const& path) const override;
  QJsonObject toJsonObject(ingredient const& i) const;
  ingredient fromJsonObject(QJsonObject const& i) const;
};

} // namespace io
} // namespace recipe
