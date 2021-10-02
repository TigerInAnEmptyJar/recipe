#include "plan_item.h"

namespace recipe {

meal_item::meal_item(recipe const& element) : _recipe(element) {}

recipe meal_item::item() const { return _recipe; }

meal_item& meal_item::item(recipe const& element)
{
  _recipe = element;
  return *this;
}

std::vector<std::string> meal_item::subscribers() const { return _subscribers; }

meal_item& meal_item::add(std::string const& subscriber)
{
  if (std::find(std::begin(_subscribers), std::end(_subscribers), subscriber) ==
      std::end(_subscribers)) {
    _subscribers.push_back(subscriber);
  }
  return *this;
}

meal_item& meal_item::remove(std::string const& subscriber)
{
  auto it = std::find(std::begin(_subscribers), std::end(_subscribers), subscriber);
  if (it != std::end(_subscribers)) {
    _subscribers.erase(it);
  }
  return *this;
}

meal_item& meal_item::remove(std::vector<std::string>::iterator const& subscriber)
{
  _subscribers.erase(subscriber);
  return *this;
}

plan_item::plan_item(std::string const& item_name) : _name(item_name) {}

std::string plan_item::name() const { return _name; }

plan_item& plan_item::name(std::string const& n)
{
  _name = n;
  return *this;
}

plan_item::iterator plan_item::begin() { return _recipes.begin(); }

plan_item::const_iterator plan_item::begin() const { return _recipes.begin(); }

plan_item::iterator plan_item::end() { return _recipes.end(); }

plan_item::const_iterator plan_item::end() const { return _recipes.end(); }

plan_item& plan_item::add(recipe const& recipe)
{
  _recipes.push_back(meal_item{recipe});
  return *this;
}

plan_item& plan_item::add(meal_item const& meal)
{
  _recipes.push_back(meal);
  return *this;
}

void plan_item::remove(iterator item)
{
  if (item != _recipes.end()) {
    _recipes.erase(item);
  }
}

bool plan_item::shoppingBefore() const { return _shoppingBefore; }

plan_item& plan_item::shoppingBefore(bool value)
{
  _shoppingBefore = value;
  return *this;
}

} // namespace recipe
