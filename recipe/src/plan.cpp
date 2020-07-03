#include "plan.h"

namespace recipe {

plan::plan(std::string const& plan_name, size_t days, size_t meals)
    : _name(plan_name), _items(days * meals), _days(days), _meals(meals)
{
}

bool plan::operator==(plan const& other) const
{
  if (!(_name == other.name() && _days == other.days() && _meals == other.meals())) {
    return false;
  }
  size_t count = 0;
  for (auto const& item : other) {
    if (item != _items[count++]) {
      return false;
    }
  }
  return true;
}

std::string plan::name() const { return _name; }

void plan::name(std::string const& n) { _name = n; }

plan::iterator plan::begin() { return _items.begin(); }

plan::const_iterator plan::begin() const { return _items.begin(); }

plan::iterator plan::end() { return _items.end(); }

plan::const_iterator plan::end() const { return _items.end(); }

size_t plan::days() const { return _days; }

size_t plan::meals() const { return _meals; }

} // namespace recipe
