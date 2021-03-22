#include "shopping_day_generator.h"
#include "shopping_day.h"

namespace recipe::io {

shopping_day shopping_day_generator::generate(std::vector<shopping_day::value_type> internals) const
{
  shopping_day result;
  result._list = std::move(internals);
  return result;
}

shopping_list shopping_day_generator::generate(std::vector<shopping_list::value_type> internals,
                                               boost::uuids::uuid id) const
{
  shopping_list result;
  result._list = std::move(internals);
  result._id = id;
  return result;
}
} // namespace recipe::io
