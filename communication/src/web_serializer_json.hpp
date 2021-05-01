#pragma once

#include "web_serializer.h"

namespace recipe::communication {

class web_serializer_json : public web_serializer
{
public:
  std::string serialize(recipe const& input) const override;
  std::string serialize(ingredient const & input) const override;
  std::string serialize(plan const& input) const override;
  std::string serialize(shopping_list const& input) const override;
};

}
