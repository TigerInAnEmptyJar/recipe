#include "amounted_ingredient.h"

#include <algorithm>
#include <optional>

namespace {
bool canConvert(recipe::amounted_ingredient::amount_t from,
                recipe::amounted_ingredient::amount_t to)
{
  using amount_t = recipe::amounted_ingredient::amount_t;
  if ((from == amount_t::liter || from == amount_t::milliliter || from == amount_t::cups) &&
      (to == amount_t::liter || to == amount_t::milliliter || to == amount_t::cups)) {
    return true;
  }
  if ((from == amount_t::grams || from == amount_t::kg || from == amount_t::pounds ||
       from == amount_t::ounces) &&
      (to == amount_t::grams || to == amount_t::kg || to == amount_t::pounds ||
       to == amount_t::ounces)) {
    return true;
  }
  return false;
}

std::optional<std::pair<recipe::amounted_ingredient::amount_t, float>> convert(
    recipe::amounted_ingredient::amount_t from, recipe::amounted_ingredient::amount_t to,
    float value)
{
  if (from == to) {
    return std::make_pair(from, value);
  }
  using amount_t = recipe::amounted_ingredient::amount_t;
  switch (from) {
  case amount_t::liter:
    if (to == amount_t::milliliter) {
      return std::make_pair(to, value * 1000);
    } else {
      return std::make_pair(to, value * 4.23);
    }
  case amount_t::milliliter:
    if (to == amount_t::liter) {
      return std::make_pair(to, value / 1000.);
    } else {
      return std::make_pair(to, value * 0.00423);
    }
  case amount_t::cups:
    if (to == amount_t::milliliter) {
      return std::make_pair(to, value * 236.59);
    } else {
      return std::make_pair(to, value * 0.23658);
    }
    break;
  case amount_t::grams:
    switch (to) {
    case amount_t::kg:
      return std::make_pair(to, value / 1000.);
    case amount_t::ounces:
      return std::make_pair(to, value * 0.035274);
    case amount_t::pounds:
      return std::make_pair(to, value * 0.002204);
    default:
      break;
    }
    break;
  case amount_t::kg:
    switch (to) {
    case amount_t::grams:
      return std::make_pair(to, value * 1000.);
    case amount_t::ounces:
      return std::make_pair(to, value * 35.274);
    case amount_t::pounds:
      return std::make_pair(to, value * 2.204);
    default:
      break;
    }
    break;
  case amount_t::pounds:
    switch (to) {
    case amount_t::kg:
      return std::make_pair(to, value * 0.45359);
    case amount_t::grams:
      return std::make_pair(to, value * 453.59);
    case amount_t::ounces:
      return std::make_pair(to, value * 16);
    default:
      break;
    }
    break;
  case amount_t::ounces:
    switch (to) {
    case amount_t::kg:
      return std::make_pair(to, value * 0.02835);
    case amount_t::grams:
      return std::make_pair(to, value * 28.35);
    case amount_t::pounds:
      return std::make_pair(to, value * 0.035274);
    default:
      break;
    }
    break;
  default:
    break;
  }
  return {};
}
} // namespace

namespace recipe {

bool amounted_ingredient::operator==(amounted_ingredient const& other) const
{
  if (_base_ingredient.id() != other.base_ingredient().id()) {
    return false;
  }
  for (auto element : other) {
    if (std::find(std::begin(_value), std::end(_value), element) == std::end(_value)) {
      return false;
    }
  }
  return true;
}

amounted_ingredient::amounted_ingredient(recipe::ingredient const& base) : _base_ingredient(base) {}

const ingredient& amounted_ingredient::base_ingredient() const { return _base_ingredient; }

amounted_ingredient::iterator amounted_ingredient::begin() { return _value.begin(); }

amounted_ingredient::iterator amounted_ingredient::end() { return _value.end(); }

amounted_ingredient::const_iterator amounted_ingredient::begin() const { return _value.begin(); }

amounted_ingredient::const_iterator amounted_ingredient::end() const { return _value.end(); }

void amounted_ingredient::add(amounted_ingredient::amount_t amount, float value)
{
  auto element = _value.find(amount);
  if (element != _value.end()) {
    element->second += value;
    return;
  }
  auto first = std::find_if(std::begin(_value), std::end(_value),
                            [amount](auto item) { return ::canConvert(amount, item.first); });
  if (first == std::end(_value)) {
    _value.insert(std::make_pair(amount, value));
    return;
  }
  auto converted = ::convert(amount, first->first, value);
  if (converted.has_value()) {
    first->second += converted->second;
    return;
  }
  _value.insert(std::make_pair(amount, value));
}

void amounted_ingredient::add(const amounted_ingredient& object)
{
  if (_base_ingredient.id() != object.base_ingredient().id()) {
    return;
  }
  for (auto element : object) {
    add(element.first, element.second);
  }
}

bool amounted_ingredient::remove(amounted_ingredient::const_iterator item)
{
  if (item == _value.end()) {
    return false;
  }
  auto element = _value.find(item->first);
  if (element == _value.end()) {
    return false;
  }

  if (element == item) {
    _value.erase(item);
    return true;
  }
  element->second = element->second - item->second;
  return true;
}

} // namespace recipe
