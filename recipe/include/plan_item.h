#pragma once
#include "recipe.h"

#include <string>

namespace recipe {

/*!
 * \brief The plan_item class
 * This class defines a single meal within a meal-plan.
 * A meal consists of one or more recipes to be cooked and a list of persons that intend to eat the
 * meal.
 * The shopping-before flag determines, whether the cook intends to go shopping before this meal is
 * cooked (used for determining which fresh ingredients need to be bought on which occation).
 */
class plan_item
{
public:
  //! Iterators to iterate over the recipes in this meal.
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

  /*!
   * \brief name-getter
   * \return the name of the meal.
   */
  std::string name() const;

  /*!
   * \brief name-setter
   * \param n the new name of the meal.
   * \return the plan-item.
   */
  plan_item& name(std::string const& n);

  /*!
   * \brief begin
   * \return an iterator to the first element in the recipe-list.
   */
  iterator begin();

  /*!
   * \brief begin
   * \return an iterator to the first element in the recipe-list.
   */
  const_iterator begin() const;

  /*!
   * \brief end
   * \return an iterator beyond the last element in the recipe-list.
   */
  iterator end();

  /*!
   * \brief end
   * \return an iterator beyond the last element in the recipe-list.
   */
  const_iterator end() const;

  /*!
   * \brief add
   * Adds a recipe to the list of recipes.
   * \note This may invalidate used iterators.
   * \param recipe the recipe to add.
   * \return the plan-item.
   */
  plan_item& add(recipe const& recipe);

  /*!
   * \brief remove
   * Removes a recipe from the list.
   * \note This will invalidate used iterators.
   * \param item the recipe to remove from the list.
   */
  void remove(iterator item);

  /*!
   * \brief subscribers
   * Returns a list of names of people that intend to participate in eating the meal.
   * \return the list of eaters/subscribers for the meal.
   */
  std::vector<std::string> const& subscribers() const;

  /*!
   * \brief add
   * Add an eater/subscriber to the meal.
   * \param subscriber the name of the person to subscribe.
   */
  void add(std::string const& subscriber);

  /*!
   * \brief remove
   * Remove a person from the list of subscribers.
   * \param item an iterator to the person to remove from the list of eaters for this meal.
   */
  void remove(std::vector<std::string>::iterator item);

  /*!
   * \brief remove
   * Remove a person from the list of subscribers.
   * \param item the name of the person to remove from the list of eaters for this meal.
   */
  void remove(std::string const& item);

  /*!
   * \brief shoppingBefore-getter
   * \return true if it is intended to go shopping before this meal.
   */
  bool shoppingBefore() const;

  /*!
   * \brief shoppingBefore-setter
   * \param value the new value for the shopping-before flag.
   * \return the plan-item.
   */
  plan_item& shoppingBefore(bool value);

private:
  std::string _name{"New meal"};
  std::vector<recipe> _recipes;
  std::vector<std::string> _subscribers;
  bool _shoppingBefore{false};
};

} // namespace recipe
