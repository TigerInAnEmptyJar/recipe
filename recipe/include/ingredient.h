#pragma once

#include <boost/uuid/uuid.hpp>

#include <filesystem>
#include <string>

namespace recipe {

/*!
 * \brief The ingredient class
 * This class defines a single ingredient containing the following properties:
 * - Name: the name of the ingredient
 * - Category: the type of ingredient. This defines both where to find the ingredient in the shop as
 *     well as whether it has to be cooled or not.
 * - Default amount type: In a recipe, an ingredient comes with an amount. The amount can be defined
 *     either in volume, in weight or in pieces. This property defines which of these types shall be
 *     the default for the ingredient.
 * - Sectioned: this flag defines where the ingredient shall appear on sectioned shopping lists.
 * - Image: an image of the ingredient.
 */
class ingredient
{
public:
  enum class category_t
  {
    fruit,
    vegetable,
    meat,
    fish,
    spices,
    diary,
    noodles,
    baking,
    drink,
    frozen,
    canned,
  };

  enum class amount_type
  {
    volume,
    mass,
    piece,
  };

public:
  ingredient(std::string const& name, category_t cat, amount_type amount, bool sect,
             std::filesystem::path const& path);
  ingredient() = default;
  ingredient(ingredient const&) = default;
  ingredient(ingredient&&) = default;
  ingredient& operator=(ingredient const&) = default;
  ingredient& operator=(ingredient&&) = default;
  ~ingredient() = default;
  bool operator==(ingredient const& other) const = default;

  /*!
   * \brief name
   * \return the name of the ingredient
   */
  std::string name() const;

  /*!
   * \brief category
   * \return the category of the ingredient
   */
  category_t category() const;

  /*!
   * \brief default_amount
   * \return the amount type which should be default for this ingredient
   */
  amount_type default_amount() const;

  /*!
   * \brief sectioned
   * \return true if the ingredient sticks to the same section as the corresponding recipe
   */
  bool sectioned() const;

  /*!
   * \brief image_path
   * \return returns the path to the image of the ingredient relative to the config path of the
   * program.
   */
  std::filesystem::path image_path() const;

  /*!
   * \brief name
   * Setter for the name of the ingredient
   * \param n the new name of the ingredient
   */
  void name(std::string const& n);

  /*!
   * \brief category
   * Setter for the category of the ingredient
   * \param c the new category of the ingredient
   */
  void category(category_t c);

  /*!
   * \brief default_amount
   * Setter for the default amount for the ingredient
   * \param a the new default amount of the ingredient
   */
  void default_amount(amount_type a);

  /*!
   * \brief sectioned
   * Setter for the sectioned property of the ingredient
   * \param s the new value of the sectioned flag
   */
  void sectioned(bool s);

  /*!
   * \brief image_path
   * Setter for the image path of the ingredient
   * \param p the new image path
   */
  void image_path(std::filesystem::path const& p);

  /*!
   * \brief getter for the id of the ingredient
   * \return the id of the ingredient.
   */
  boost::uuids::uuid id() const;

  /*!
   * \brief setter for the id of the ingredient
   * \param i the new id of the ingredient.
   */
  void id(boost::uuids::uuid i);

private:
  boost::uuids::uuid generateId();

  std::string _name{"New ingredient"};
  category_t _category{category_t::fruit};
  amount_type _amount{amount_type::volume};
  bool _sectioned{true};
  std::filesystem::path _image_path;
  boost::uuids::uuid _id{generateId()};
};

} // namespace recipe
