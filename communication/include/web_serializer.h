#pragma once

#include "recipe.h"
#include "shopping_list.h"

namespace recipe::communication {

class web_serializer
{
public:
  virtual std::string serialize(recipe const& input) const = 0;
  virtual std::string serialize(ingredient const & input) const = 0;
  virtual std::string serialize(plan const& input) const=0;
  virtual std::string serialize(shopping_list const& input) const= 0;
};

std::shared_ptr<web_serializer> create_web_serializer();

}
