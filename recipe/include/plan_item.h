#pragma once
#include "recipe.h"

#include <string>

namespace recipe {

/*!
 * \brief The meal_item class
 * The meal_item represents a single recipe, like a side dish or a desert.
 * To make it possible to have several options for different eaters, the
 * individual recipe is equiped with a list of subscribers.
 */
class meal_item
{
public:
  meal_item() = default;
  meal_item(meal_item const&) = default;
  meal_item(meal_item&&) = default;
  ~meal_item() = default;
  meal_item& operator=(meal_item const&) = default;
  meal_item& operator=(meal_item&&) = default;
  bool operator==(meal_item const&) const = default;

  /*!
   * \brief meal_item Constructor
   * \param element the recipe of this meal part
   */
  explicit meal_item(recipe const& element);

  /*!
   * \brief Getter for the recipe of this meal part.
   * \return the recipe of this meal part.
   */
  recipe item() const;

  /*!
   * \brief Setter for the recipe of this meal part.
   * \param element the recipe for this meal part
   * \return this object.
   */
  meal_item& item(recipe const& element);

  /*!
   * \brief Getter for all subscribers to this meal part.
   * \return list of subscribers.
   */
  std::vector<std::string> subscribers() const;

  /*!
   * \brief Subscribe to this meal part.
   * \note will be ignored if the provided subscriber is already subscribed.
   * \param subscriber name of the subscriber,
   * \return this object.
   */
  meal_item& add(std::string const& subscriber);

  /*!
   * \brief Unsubscribe from this meal part.
   * \note will be ignored if the provided subscriber is not subscribed.
   * \param subscriber the name of the subscriber,
   * \return this object.
   */
  meal_item& remove(std::string const& subscriber);

  /*!
   * \brief Unsubscribe from this meal part.
   * \param subscriber the subscriber, as iterator on the subscription list.
   * \return this object.
   */
  meal_item& remove(std::vector<std::string>::iterator const& subscriber);

private:
  recipe _recipe;
  std::vector<std::string> _subscribers;
};

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
  using iterator = std::vector<meal_item>::iterator;
  using const_iterator = std::vector<meal_item>::const_iterator;

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
   * \brief add
   * Adds a recipe to the list of recipes.
   * \note This may invalidate used iterators.
   * \param meal the recipe to add.
   * \return the plan-item.
   */
  plan_item& add(meal_item const& meal);

  /*!
   * \brief remove
   * Removes a recipe from the list.
   * \note This will invalidate used iterators.
   * \param item the recipe to remove from the list.
   */
  void remove(iterator item);

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
  std::vector<meal_item> _recipes;
  bool _shoppingBefore{false};
};

} // namespace recipe
