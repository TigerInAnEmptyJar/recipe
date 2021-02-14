#include "plan.h"

#include <iterator>
#include <sstream>

namespace recipe {

plan::plan(std::string const& plan_name, std::size_t days, std::size_t meals)
    : _name(plan_name), _items(days * meals), _days(days), _meals(meals)
{
}

bool plan::operator==(plan const& other) const
{
  if (!(_name == other.name() && _days == other.days() && _meals == other.meals())) {
    return false;
  }
  std::size_t count = 0;
  for (auto const& item : other) {
    if (item != _items[count++]) {
      return false;
    }
  }
  if (other._eaters != _eaters) {
    return false;
  }
  return true;
}

std::string plan::name() const { return _name; }

void plan::name(std::string const& n) { _name = n; }

plan::iterator plan::begin() { return _items.begin(); }

plan::const_iterator plan::begin() const { return _items.begin(); }

plan::iterator plan::end() { return _items.end(); }

plan::const_iterator plan::end() const { return _items.end(); }

std::size_t plan::days() const { return _days; }

std::size_t plan::meals() const { return _meals; }

std::string plan::eaters() const
{
  std::stringstream out;
  std::copy(std::begin(_eaters), std::end(_eaters), std::ostream_iterator<std::string>(out, ", "));
  return out.str();
}

std::vector<std::string> plan::eaterList() const { return _eaters; }

plan& plan::addEater(std::string const& eater)
{
  if (std::find(std::begin(_eaters), std::end(_eaters), eater) == std::end(_eaters)) {
    _eaters.push_back(eater);
  }
  return *this;
}

void plan::removeEater(std::string const& eater)
{
  auto item = std::find(std::begin(_eaters), std::end(_eaters), eater);
  if (item != std::end(_eaters)) {
    _eaters.erase(item);
  }
}

} // namespace recipe
