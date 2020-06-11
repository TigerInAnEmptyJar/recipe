#pragma once

#include "ingredient.h"

#include <map>

namespace recipe {

/*!
 * \brief The amounted_ingredient class
 * This class hosts an ingredient along with the amount needed from it for a recipe or within
 * a weekly plan.
 */
class amounted_ingredient
{
public:
  /*!
   * \brief The amount_t enum
   * This shows possible amount types.
   */
  enum class amount_t
  {
    // Volumes
    liter,
    milliliter,
    cups,
    // Masses
    grams,
    kg,
    ounces,
    pounds,
    // Pieces
    piece,
    bundle,
    can,
  };

  using iterator = std::map<amount_t, float>::iterator;
  using const_iterator = std::map<amount_t, float>::const_iterator;

public:
  amounted_ingredient(amounted_ingredient const&) = default;
  amounted_ingredient(amounted_ingredient&&) = default;
  amounted_ingredient& operator=(amounted_ingredient const&) = default;
  amounted_ingredient& operator=(amounted_ingredient&&) = default;
  ~amounted_ingredient() = default;

  /*!
   * \brief Comparison operator
   * \param other the object to compare to
   * \return true if the two objects are equal
   */
  bool operator==(amounted_ingredient const& other) const;

  /*!
   * \brief Explicit constructor
   * \param base the ingredient that is base of this object.
   */
  explicit amounted_ingredient(ingredient const& base);

  /*!
   * \brief base_ingredient
   * \return the ingredient this object is based on.
   */
  ingredient const& base_ingredient() const;

  /*!
   * \brief begin
   * \return the begin of the list of amounts in this object
   */
  iterator begin();

  /*!
   * \brief end
   * \return the end of the list of amounts in this object
   */
  iterator end();

  /*!
   * \brief begin
   * \return the begin of the list of amounts in this object
   */
  const_iterator begin() const;

  /*!
   * \brief end
   * \return the end of the list of amounts in this object
   */
  const_iterator end() const;

  /*!
   * \brief adds an amount to the ingredient
   * Where possible, this will be added to a convertable amount item.
   * \param amount the amount type
   * \param value the value of the amount
   */
  void add(amount_t amount, float value);

  /*!
   * \brief add the amounts of one amounted ingredient to this object
   * Works only if both amounted ingredients have the same id.
   * \param object the amounted ingredient to add.
   */
  void add(amounted_ingredient const& object);

  /*!
   * \brief remove an amount from the list of amounts.
   * \param item the item to remove
   * \return true if operation was successful.
   */
  bool remove(const_iterator item);

private:
  ingredient _base_ingredient;
  std::map<amount_t, float> _value;
};

} // namespace recipe
