#pragma once

#include "shopping_io.h"

class QJsonObject;

namespace recipe::io {

class shopping_json_io : public shopping_io
{
public:
  void write(shopping_list const& out, std::filesystem::path const& path) const override;

  std::optional<shopping_list> read(std::filesystem::path const& path,
                                    finder_t ingredient_finder) const override;

  std::string serialize(shopping_list const& out) const override;

  std::optional<shopping_list> deserialize(std::string const& text,
                                           finder_t ingredient_finder) const override;
  QJsonObject to_object(shopping_list const& out) const;
  QJsonObject to_object(shopping_day const& day) const;
  std::optional<shopping_list> list_from_object(QJsonObject const& object,
                                                finder_t ingredient_finder) const;
  std::optional<shopping_day> day_from_object(QJsonObject const& object,
                                              finder_t ingredient_finder) const;
};

} // namespace recipe::io
