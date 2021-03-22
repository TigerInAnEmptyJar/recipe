#pragma once

#include "shopping_io.h"

namespace recipe::io {

class shopping_tex_writer : public shopping_io
{
public:
  void write(shopping_list const& out, std::filesystem::path const& path) const override;
  std::optional<shopping_list> read(std::filesystem::path const& path,
                                    finder_t ingredient_finder) const override;
  std::string serialize(shopping_list const& out) const override;
  std::optional<shopping_list> deserialize(std::string const& text,
                                           finder_t ingredient_finder) const override;
};

} // namespace recipe::io
