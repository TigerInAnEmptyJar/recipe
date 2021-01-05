#pragma once
#include "plan_item.h"

#include <set>

namespace recipe {

class plan
{
public:
  using iterator = std::vector<plan_item>::iterator;
  using const_iterator = std::vector<plan_item>::const_iterator;

public:
  plan(std::string const& item_name, size_t days, size_t meals);
  plan(plan const&) = default;
  plan(plan&&) = default;
  plan& operator=(plan const&) = default;
  plan& operator=(plan&&) = default;
  ~plan() = default;
  bool operator==(plan const& other) const;

  std::string name() const;
  void name(std::string const& n);

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  size_t days() const;
  size_t meals() const;

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

private:
  std::string _name{"New plan"};
  std::vector<plan_item> _items;
  size_t _days;
  size_t _meals;
  std::vector<std::string> _eaters;
};

} // namespace recipe
