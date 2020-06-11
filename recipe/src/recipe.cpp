#include "recipe.h"

#include <algorithm>
#include <numeric>

namespace recipe {

recipe::recipe(std::string const& t) : _title(t) {}

std::string recipe::title() const { return _title; }

void recipe::title(std::string const& t) { _title = t; }

meal_t recipe::meal_type() const { return _meal_type; }

void recipe::meal_type(meal_t t) { _meal_type = t; }

uint8_t recipe::servings() const { return _servings; }

void recipe::servings(uint8_t s) { _servings = s; }

std::string recipe::instructions() const { return _instructions; }

void recipe::instructions(std::string const& i) { _instructions = i; }

std::chrono::minutes recipe::preparation_time() const { return _time; }

void recipe::preparation_time(std::chrono::minutes time) { _time = time; }

std::string recipe::source() const { return _source; }

void recipe::source(std::string const& s) { _source = s; }

std::filesystem::__cxx11::path recipe::image_path() const { return _image_path; }

void recipe::image_path(std::filesystem::__cxx11::path const& path) { _image_path = path; }

uint8_t recipe::calories() const { return _calories; }

void recipe::calories(uint8_t c) { _calories = c; }

uint8_t recipe::joules() const { return _joules; }

void recipe::joules(uint8_t c) { _joules = c; }

uint8_t recipe::g_fat() const { return _g_fat; }

void recipe::g_fat(uint8_t c) { _g_fat = c; }

uint8_t recipe::g_proteins() const { return _g_protein; }

void recipe::g_proteins(uint8_t c) { _g_protein = c; }

uint8_t recipe::g_carbohydrates() const { return _g_carbohydrates; }

void recipe::g_carbohydrates(uint8_t c) { _g_carbohydrates = c; }

recipe::const_iterator recipe::begin() const { return _ingredients.begin(); }

recipe::iterator recipe::begin() { return _ingredients.begin(); }

recipe::const_iterator recipe::end() const { return _ingredients.end(); }

recipe::iterator recipe::end() { return _ingredients.end(); }

void recipe::add(amounted_ingredient const& ingredient)
{
  auto item =
      std::find_if(std::begin(_ingredients), std::end(_ingredients), [ingredient](auto element) {
        return element.base_ingredient().id() == ingredient.base_ingredient().id();
      });
  if (item != std::end(_ingredients)) {
    item->add(ingredient);
    return;
  }
  _ingredients.push_back(ingredient);
}

void recipe::remove(recipe::const_iterator item)
{
  if (item == _ingredients.end()) {
    return;
  }
  auto element = std::find(std::begin(_ingredients), std::end(_ingredients), *item);
  if (element == _ingredients.end()) {
    return;
  }

  if (element == item) {
    _ingredients.erase(item);
    return;
  }
}

std::string recipe::tags() const
{
  if (_tags.size() == 0) {
    return {};
  }
  return std::accumulate(
             std::begin(_tags), std::end(_tags), std::string{},
             [](std::string const& s1, std::string const& s2) { return s1 + ", " + s2; })
      .substr(2);
}

std::vector<std::string> recipe::tagList() const { return _tags; }

void recipe::addTag(std::string const& tag)
{
  if (std::find(std::begin(_tags), std::end(_tags), tag) == std::end(_tags)) {
    _tags.push_back(tag);
  }
}

void recipe::removeTag(std::string const& tag)
{
  auto item = std::find(std::begin(_tags), std::end(_tags), tag);
  if (item != std::end(_tags)) {
    _tags.erase(item);
  }
}

std::string recipe::eaters() const
{
  if (_eaters.size() == 0) {
    return {};
  }
  if (_eaters.size() == 1) {
    return _eaters[0];
  }
  return std::accumulate(
             std::begin(_eaters), std::end(_eaters), std::string{},
             [](std::string const& s1, std::string const& s2) { return s1 + ", " + s2; })
      .substr(2);
}

std::vector<std::string> recipe::eaterList() const { return _eaters; }

void recipe::addEater(std::string const& eater)
{
  if (std::find(std::begin(_eaters), std::end(_eaters), eater) == std::end(_eaters)) {
    _eaters.push_back(eater);
  }
}

void recipe::removeEater(std::string const& eater)
{
  auto item = std::find(std::begin(_eaters), std::end(_eaters), eater);
  if (item != std::end(_eaters)) {
    _eaters.erase(item);
  }
}

boost::uuids::uuid recipe::id() const { return _id; }

void recipe::id(boost::uuids::uuid i) { _id = i; }

} // namespace recipe
