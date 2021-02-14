#pragma once

#include "plan.h"
#include "shopping_day.h"

#include <vector>

namespace recipe {

/*!
 * \brief The shopping_list class
 * This class represents the list of shopping events for a single meal-plan. Since there might be
 * defined several meals before which shopping is possible, this list here hosts one item for each
 * of these opportunities.
 */
class shopping_list
{
public:
  //! Convenience aliases.
  using value_type = shopping_day;
  using iterator = std::vector<value_type>::iterator;
  using const_iterator = std::vector<value_type>::const_iterator;

  shopping_list(shopping_list const&) = default;
  shopping_list(shopping_list&&) = default;
  ~shopping_list() = default;
  shopping_list& operator=(shopping_list const&) = default;
  shopping_list& operator=(shopping_list&&) = default;

  /*!
   * \brief name-getter
   * \return the name of the shopping-list.
   */
  std::string name() const;

  /*!
   * \brief name-setter
   * \param n the new name of the shopping-list.
   * \return the shopping-list.
   */
  shopping_list& name(std::string const& n);

  /*!
   * \brief begin
   * \return an iterator to the first element of the list of shopping-events.
   */
  iterator begin();

  /*!
   * \brief begin
   * \return an iterator to the first element of the list of shopping-events.
   */
  const_iterator begin() const;

  /*!
   * \brief end
   * \return an iterator beyond the last element of the list of shopping-events.
   */
  iterator end();

  /*!
   * \brief end
   * \return an iterator beyond the last element of the list of shopping-events.
   */
  const_iterator end() const;

  /*!
   * \brief update_from_plan
   * This method applies changes made in a meal-plan to the shopping-list. In order not to loose
   * already provided availability-data (\see shopping_day::available), the list can be updated
   * instead of generating it anew.
   * Ingredients common to plan and list will keep their values-at-home.
   * \param from the meal-plan to update from.
   */
  void update_from_plan(plan const& from);

  /*!
   * \brief generate
   * Since constructors (apart from copy- and move-constructors) are deactivated, we here have the
   * one and only factory function. It needs a plan to be generated.
   * \param from the plan to generate the shopping-list from.
   * \return the created shopping-list.
   */
  static shopping_list generate(plan const& from);

private:
  shopping_list() = default;
  void assign_names(plan const& from);

  std::vector<value_type> _list;
  std::string _name;
};

} // namespace recipe
