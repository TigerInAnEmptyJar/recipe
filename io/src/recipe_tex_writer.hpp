#pragma once

#include "recipe.h"
#include "recipe_io.h"

#include <filesystem>

namespace recipe {
namespace io {

class recipe_tex_writer : public recipe_io
{
public:
  void write(std::vector<recipe> const& out, std::filesystem::path const& path) const override;
  void write(std::vector<recipe> const& out, std::ostream & path) const override;
  std::optional<std::vector<recipe>> read(std::filesystem::path const& path,
                                          finder_t ingredient_finder) const override;
  void write(recipe const& out, std::ostream& output,
             std::filesystem::path const& databasePath) const;
};

} // namespace io
} // namespace recipe
