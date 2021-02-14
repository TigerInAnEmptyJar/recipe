#include "shopping_day.h"

#include <vector>

namespace recipe {

shopping_day::shopping_day(std::vector<amounted_ingredient> ingredients)
{
  std::transform(
      std::begin(ingredients), std::end(ingredients), std::back_inserter(_list), [](auto item) {
        return std::make_pair(item, std::make_pair(amounted_ingredient::amount_t::grams, 0.f));
      });
}

void shopping_day::update(std::vector<amounted_ingredient> ingredients)
{
  std::for_each(std::begin(_list), std::end(_list), [&ingredients](auto& item) {
    auto element = std::find_if(std::begin(ingredients), std::end(ingredients), [&item](auto el) {
      return el.base_ingredient().id() == item.first.base_ingredient().id();
    });
    if (element == std::end(ingredients)) {
      return;
    }
    item.first = *element;
  });
  _list.erase(std::remove_if(std::begin(_list), std::end(_list),
                             [&ingredients](auto& item) {
                               auto element =
                                   std::find_if(std::begin(ingredients), std::end(ingredients),
                                                [&item](auto el) {
                                                  return el.base_ingredient().id() ==
                                                         item.first.base_ingredient().id();
                                                });
                               return element == std::end(ingredients);
                             }),
              std::end(_list));
}

shopping_day::iterator shopping_day::begin() { return _list.begin(); }

shopping_day::const_iterator shopping_day::begin() const { return _list.begin(); }

shopping_day::iterator shopping_day::end() { return _list.end(); }

shopping_day::const_iterator shopping_day::end() const { return _list.end(); }

std::string shopping_day::name() const { return _name; }

shopping_day& shopping_day::name(std::string const& n)
{
  _name = n;
  return *this;
}

void shopping_day::available(boost::uuids::uuid ingredient, float value,
                             amounted_ingredient::amount_t amount)
{
  auto item = std::find_if(std::begin(_list), std::end(_list), [ingredient](auto element) {
    return element.first.base_ingredient().id() == ingredient;
  });
  if (item == std::end(_list)) {
    return;
  }
  item->second = std::make_pair(amount, value);
}

} // namespace recipe
