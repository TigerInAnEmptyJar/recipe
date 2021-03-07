#pragma once

#include "shopping_list.h"

namespace recipe::io {

class shopping_day_generator
{
public:
  shopping_day generate(std::vector<shopping_day::value_type> internals) const;
  shopping_list generate(std::vector<shopping_list::value_type> internals) const;
};

} // namespace recipe::io
