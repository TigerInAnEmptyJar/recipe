#include "shopping_list.h"

#include <iostream>
namespace {

auto list_from_plan(recipe::plan const& from)
    -> std::vector<std::vector<recipe::amounted_ingredient>>
{
  std::vector<std::vector<recipe::amounted_ingredient>> in_plan;

  auto addToList = [&in_plan](recipe::amounted_ingredient ingredient, std::size_t index) {
    while (in_plan.size() <= index) {
      in_plan.push_back({});
    }
    auto element = std::find_if(
        std::begin(in_plan[index]), std::end(in_plan[index]), [&ingredient](auto item) {
          return item.base_ingredient().id() == ingredient.base_ingredient().id();
        });
    if (element != std::end(in_plan[index])) {
      element->add(ingredient);
    } else {
      in_plan[index].push_back(ingredient);
    }
  };

  int currentSection = -1;
  std::for_each(std::begin(from), std::end(from), [&addToList, &currentSection](auto item) {
    if (item.shoppingBefore() || currentSection < 0) {
      currentSection++;
    }
    std::for_each(std::begin(item), std::end(item), [&addToList, currentSection](auto rec) {
      auto portionsNeeded = rec.subscribers().size();
      if (portionsNeeded == 0) {
        return;
      }
      auto element = rec.item();
      float ingredientFactor =
          static_cast<float>(portionsNeeded) / static_cast<float>(element.servings());
      std::for_each(std::begin(element), std::end(element),
                    [ingredientFactor, &addToList, currentSection](auto ingredient) {
                      auto in{ingredient};
                      in *= ingredientFactor;
                      if (in.base_ingredient().sectioned()) {
                        addToList(in, currentSection);
                      } else {
                        addToList(in, 0);
                      }
                    });
    });
  });
  return in_plan;
}

} // namespace

namespace recipe {

std::string shopping_list::name() const { return _name; }

shopping_list& shopping_list::name(std::string const& n)
{
  _name = n;
  return *this;
}

shopping_list::iterator shopping_list::begin() { return _list.begin(); }

shopping_list::const_iterator shopping_list::begin() const { return _list.begin(); }

shopping_list::iterator shopping_list::end() { return _list.end(); }

shopping_list::const_iterator shopping_list::end() const { return _list.end(); }

boost::uuids::uuid shopping_list::id() const { return _id; }

void shopping_list::update_from_plan(plan const& from)
{
  if (_id != from.id()) {
    return;
  }
  auto in_plan = ::list_from_plan(from);

  while (_list.size() < in_plan.size()) {
    _list.push_back({in_plan[_list.size()]});
  }

  auto listIt = std::begin(_list);
  for (auto planIt = std::begin(in_plan); planIt != std::end(in_plan) && listIt != std::end(_list);
       planIt++, listIt++) {
    listIt->update(*planIt);
  }
  assign_names(from);
}

shopping_list shopping_list::generate(plan const& from)
{
  shopping_list result;

  auto in_plan = ::list_from_plan(from);

  std::transform(std::begin(in_plan), std::end(in_plan), std::back_inserter(result._list),
                 [](auto element) { return shopping_day(element); });
  result.assign_names(from);
  result._id = from.id();

  return result;
}

void shopping_list::assign_names(plan const& from)
{
  auto planIt = std::begin(from);
  if (std::begin(_list) != std::end(_list)) {
    std::begin(_list)->name(planIt->name());
  }
  std::for_each(std::begin(_list), std::end(_list), [&planIt, &from](auto& day) {
    while (planIt != std::end(from) && !planIt->shoppingBefore()) {
      planIt++;
    }
    if (planIt != std::end(from)) {
      day.name(planIt->name());
      planIt++;
    }
  });
}

} // namespace recipe
