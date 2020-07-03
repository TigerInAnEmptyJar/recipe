#pragma once
#include "recipe.h"

#include <string>

namespace recipe {

class plan_item
{
public:
  using iterator = std::vector<recipe>::iterator;
  using const_iterator = std::vector<recipe>::const_iterator;

public:
  plan_item(std::string const& item_name);
  plan_item() = default;
  plan_item(plan_item const&) = default;
  plan_item(plan_item&&) = default;
  plan_item& operator=(plan_item const&) = default;
  plan_item& operator=(plan_item&&) = default;
  ~plan_item() = default;
  bool operator==(plan_item const& other) const = default;

  std::string name() const;
  void name(std::string const& n);

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  void add(recipe const& recipe);
  void remove(iterator item);

  std::vector<std::string> const& subscribers() const;
  void add(std::string const& subscriber);
  void remove(std::vector<std::string>::iterator item);

  bool shoppingBefore() const;
  void shoppingBefore(bool value);

private:
  std::string _name{"New meal"};
  std::vector<recipe> _recipes;
  std::vector<std::string> _subscribers;
  bool _shoppingBefore{false};
};

} // namespace recipe
