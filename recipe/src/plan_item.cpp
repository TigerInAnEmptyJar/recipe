#include "plan_item.h"

namespace recipe {

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
  _recipes.push_back(recipe);
  return *this;
}

void plan_item::remove(iterator item)
{
  if (item != _recipes.end()) {
    _recipes.erase(item);
  }
}

std::vector<std::string> const& plan_item::subscribers() const { return _subscribers; }

void plan_item::add(std::string const& subscriber) { _subscribers.push_back(subscriber); }

void plan_item::remove(std::vector<std::string>::iterator item)
{
  if (item != _subscribers.end()) {
    _subscribers.erase(item);
  }
}

void plan_item::remove(std::string const& item)
{
  auto element = std::find(_subscribers.begin(), _subscribers.end(), item);
  if (element != _subscribers.end()) {
    _subscribers.erase(element);
  }
}

bool plan_item::shoppingBefore() const { return _shoppingBefore; }

plan_item& plan_item::shoppingBefore(bool value)
{
  _shoppingBefore = value;
  return *this;
}

} // namespace recipe
