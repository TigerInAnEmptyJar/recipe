#pragma once

#include "amounted_io.h"

class QJsonObject;

namespace recipe {
namespace io {

class amounted_json_io : public amounted_io
{
public:
  void write(std::vector<amounted_ingredient> const& out,
             std::filesystem::path const& path) const override;

  std::optional<std::vector<amounted_ingredient>> read(std::filesystem::path const& path,
                                                       finder_t ingredient_finder) const override;

  QJsonObject toJsonObject(amounted_ingredient const& i) const;
  std::optional<amounted_ingredient> fromJsonObject(QJsonObject const& i,
                                                    finder_t ingredient_finder) const;
};

} // namespace io
} // namespace recipe
