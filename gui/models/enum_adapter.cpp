#include "enum_adapter.hpp"

#include "amounted_ingredient.h"
#include "ingredient.h"
#include "recipe.h"

#include <QString>

namespace recipe {
namespace gui {

template <>
enum_adapter<typename ingredient::amount_type>::enum_adapter()
    : _biggest(ingredient::amount_type::piece), _smallest(ingredient::amount_type::volume)
{
  using value_type = decltype(_map)::value_type;

  // clang-format off
  _map.insert(value_type(ingredient::amount_type::volume, QObject::tr("Volume")));
  _map.insert(value_type(ingredient::amount_type::mass,   QObject::tr("Mass")));
  _map.insert(value_type(ingredient::amount_type::piece,  QObject::tr("Piece")));
  // clang-format on
  fillAll();
}

template <>
enum_adapter<typename ingredient::category_t>::enum_adapter()
    : _biggest(ingredient::category_t::canned), _smallest(ingredient::category_t::fruit)
{
  using value_type = decltype(_map)::value_type;

  // clang-format off
  _map.insert(value_type(ingredient::category_t::fruit,     QObject::tr("Fruit")));
  _map.insert(value_type(ingredient::category_t::vegetable, QObject::tr("Vegetable")));
  _map.insert(value_type(ingredient::category_t::meat,      QObject::tr("Meat")));
  _map.insert(value_type(ingredient::category_t::fish,      QObject::tr("Fish")));
  _map.insert(value_type(ingredient::category_t::spices,    QObject::tr("Spices")));
  _map.insert(value_type(ingredient::category_t::diary,     QObject::tr("Diary")));
  _map.insert(value_type(ingredient::category_t::noodles,   QObject::tr("Noodles")));
  _map.insert(value_type(ingredient::category_t::baking,    QObject::tr("Baking")));
  _map.insert(value_type(ingredient::category_t::drink,     QObject::tr("Drink")));
  _map.insert(value_type(ingredient::category_t::frozen,    QObject::tr("Frozen")));
  _map.insert(value_type(ingredient::category_t::canned,    QObject::tr("Canned")));
  // clang-format on
  fillAll();
}

template <>
enum_adapter<typename amounted_ingredient::amount_t>::enum_adapter()
    : _biggest(amounted_ingredient::amount_t::pince),
      _smallest(amounted_ingredient::amount_t::liter)
{
  using value_type = decltype(_map)::value_type;
  // clang-format off
  _map.insert(value_type(amounted_ingredient::amount_t::liter,       QObject::tr("Liter")));
  _map.insert(value_type(amounted_ingredient::amount_t::milliliter,  QObject::tr("Milliliter")));
  _map.insert(value_type(amounted_ingredient::amount_t::cups,        QObject::tr("Cups")));
  _map.insert(value_type(amounted_ingredient::amount_t::grams,       QObject::tr("Grams")));
  _map.insert(value_type(amounted_ingredient::amount_t::kg,          QObject::tr("Kilograms")));
  _map.insert(value_type(amounted_ingredient::amount_t::ounces,      QObject::tr("Ounces")));
  _map.insert(value_type(amounted_ingredient::amount_t::pounds,      QObject::tr("Pounds")));
  _map.insert(value_type(amounted_ingredient::amount_t::piece,       QObject::tr("Pieces")));
  _map.insert(value_type(amounted_ingredient::amount_t::bundle,      QObject::tr("Bundles")));
  _map.insert(value_type(amounted_ingredient::amount_t::can,         QObject::tr("Cans")));
  _map.insert(value_type(amounted_ingredient::amount_t::table_spoon, QObject::tr("Table spoons")));
  _map.insert(value_type(amounted_ingredient::amount_t::tea_spoon,   QObject::tr("Tea spoons")));
  _map.insert(value_type(amounted_ingredient::amount_t::pince,       QObject::tr("Pinces")));
  // clang-format on
  fillAll();
}

template <>
enum_adapter<meal_t>::enum_adapter() : _biggest(meal_t::baking), _smallest(meal_t::fish)
{
  using value_type = decltype(_map)::value_type;
  // clang-format off
  _map.insert(value_type(meal_t::fish,       QObject::tr("Fish")));
  _map.insert(value_type(meal_t::vegetarian, QObject::tr("Vegetarian")));
  _map.insert(value_type(meal_t::vegan,      QObject::tr("Vegan")));
  _map.insert(value_type(meal_t::sweet,      QObject::tr("Sweet")));
  _map.insert(value_type(meal_t::other,      QObject::tr("Other")));
  _map.insert(value_type(meal_t::baking,     QObject::tr("Baking")));
  // clang-format on
  fillAll();
}

} // namespace gui
} // namespace recipe
