#pragma once

#include "amounted_ingredient.h"

#include <vector>

namespace recipe {
namespace io {
class shopping_day_generator;
}

/*!
 * \brief The shopping_day class
 * This class contains a shopping list for a single shopping event.
 * It doesn't only contain a list of ingredients, which will be needed for the meal-plan this list
 * was generated from, but for each ingredient, an amount can be specified, which is found at home
 * already to prevent buying too much.
 */
class shopping_day
{
public:
  //! Aliases and iterators for convenience.
  using value_type = std::tuple<amounted_ingredient, amounted_ingredient::amount_t, float, bool>;
  using iterator = std::vector<value_type>::iterator;
  using const_iterator = std::vector<value_type>::const_iterator;

  shopping_day(shopping_day const&) = default;
  shopping_day(shopping_day&&) = default;
  ~shopping_day() = default;
  shopping_day& operator=(shopping_day const&) = default;
  shopping_day& operator=(shopping_day&&) = default;
  bool operator==(shopping_day const& other) const = default;

  /*!
   * \brief begin
   * \return an iterator to the first element of the ingredient list.
   */
  iterator begin();

  /*!
   * \brief begin
   * \return an iterator to the first element of the ingredient list.
   */
  const_iterator begin() const;

  /*!
   * \brief end
   * \return an iterator beyond the last element of the ingredient list.
   */
  iterator end();

  /*!
   * \brief end
   * \return an iterator beyond the last element of the ingredient list.
   */
  const_iterator end() const;

  /*!
   * \brief name-getter
   * \return the name of the shopping-event (by default: derived from the name of the meal before
   *         which it will happen).
   */
  std::string name() const;

  /*!
   * \brief name-setter
   * \param n the new name of the shopping-event
   * \return the shopping-event.
   */
  shopping_day& name(std::string const& n);

  /*!
   * \brief available
   * This method sets the amount which is already available at home for a specified ingredient.
   * \param ingredient the id of an ingredient to specify the amount,
   * \param value the numeric value of the amount found at home,
   * \param amount the unit.
   */
  void available(boost::uuids::uuid ingredient, float value, amounted_ingredient::amount_t amount);

private:
  shopping_day() = default;
  shopping_day(std::vector<amounted_ingredient> ingredients);

  void update(std::vector<amounted_ingredient> ingredients);

  std::vector<value_type> _list;
  std::string _name;

  friend class shopping_list;
  friend class ::recipe::io::shopping_day_generator;
};

} // namespace recipe
