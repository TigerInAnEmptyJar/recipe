#pragma once

#include "plan_io.h"

class QJsonObject;

namespace recipe {
namespace io {

class plan_json_io : public plan_io
{
public:
  void write(plan const& out, std::filesystem::path const& path) const override;

  std::optional<plan> read(std::filesystem::path const& path,
                           finder_t recipe_finder) const override;
  QJsonObject toJsonObject(plan_item const& i) const;
  std::optional<plan_item> itemFromJsonObject(QJsonObject const& i, finder_t recipe_finder) const;
  QJsonObject toJsonObject(plan const& i) const;
  std::optional<plan> planFromJsonObject(QJsonObject const& i, finder_t recipe_finder) const;
};

} // namespace io
} // namespace recipe
