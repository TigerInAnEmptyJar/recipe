#pragma once
#include "plan_item.h"

#include <set>

namespace recipe {

/*!
 * \brief The plan class
 * Defines the meals in a dedicated amount of days.
 */
class plan
{
public:
  //! Declare iterators to access the list of meals.
  using iterator = std::vector<plan_item>::iterator;
  using const_iterator = std::vector<plan_item>::const_iterator;

public:
  plan(std::string const& item_name, std::size_t days, std::size_t meals);
  plan(plan const&) = default;
  plan(plan&&) = default;
  plan& operator=(plan const&) = default;
  plan& operator=(plan&&) = default;
  ~plan() = default;
  bool operator==(plan const& other) const;

  /*!
   * \brief name-getter
   * \return the name of the plan.
   */
  std::string name() const;

  /*!
   * \brief name-setter
   * \param n the new name of the plan.
   */
  void name(std::string const& n);

  /*!
   * \brief begin
   * \return an iterator to the first meal of the plan.
   */
  iterator begin();

  /*!
   * \brief begin
   * \return an iterator to the first meal of the plan.
   */
  const_iterator begin() const;

  /*!
   * \brief end
   * \return an iterator behind the last meal of the plan.
   */
  iterator end();

  /*!
   * \brief end
   * \return an iterator behind the last meal of the plan.
   */
  const_iterator end() const;

  /*!
   * \brief days
   * \return the number of days the plan is designed for.
   */
  std::size_t days() const;

  /*!
   * \brief meals
   * \return the number of meals per day for this plan.
   */
  std::size_t meals() const;

  /*!
   * \brief getter for a concattenated string of potential eaters/subscribers
   * \return the potential eaters for this plan
   */
  std::string eaters() const;

  /*!
   * \brief getter for the list of potential eaters/subscribers
   * \return the list of potential eaters
   */
  std::vector<std::string> eaterList() const;

  /*!
   * \brief add an Eater/Subscriber
   * \param eater the new eater
   * \return a reference to the changed recipe
   */
  plan& addEater(std::string const& eater);

  /*!
   * \brief remove an Eater/Subscriber
   * if the eater is not stored in the list of potential eaters, it is not removed
   * \param eater the eater to remove
   */
  void removeEater(std::string const& eater);

  boost::uuids::uuid id() const;
  plan& id(boost::uuids::uuid const& id);

private:
  std::string _name{"New plan"};
  std::vector<plan_item> _items;
  std::size_t _days;
  std::size_t _meals;
  std::vector<std::string> _eaters;
  boost::uuids::uuid _id;
};

} // namespace recipe
