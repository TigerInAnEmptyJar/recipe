#pragma once

#include "amounted_ingredient.h"

#include <boost/uuid/uuid.hpp>

#include <vector>

namespace recipe {

/*!
 * \brief The meal_t enum
 * This enum contains the most general types of meals.
 */
enum class meal_t
{
  fish,
  vegetarian,
  vegan,
  sweet,
  other,
  baking,
};

/*!
 * \brief The recipe class
 * This class represents a recipe. It contains the following properties:
 * - Title: The name of the recipe
 * - Number of servings: The number of servings to prepare with the given amount of ingredients
 * - Preparation time: The estimated time it takes to prepare the meal for the given number of
 *   servings
 * - Nutrition: This comes in several numbers: Carbohydrates, fats, proteins come in grams, while
 *   energy will be given both in Joules as well as calories.
 * - Source: Here the author of the recipe can provide a hint to where the recipe was taken from.
 * - Meal-type: Here the basic type of the meal can be specified, whether it contains meat or fish,
 *   whether it is vegetarian or vegan or whether it is a sweet dish/dessert.
 * - Tags: This is a list of strings that can further specify the type of meal the recipe describes.
 *   Along with the meal type, this is a property that can be used for filtering.
 * - Potential Eaters: Like tags, this is a list of strings, each one defining a person that would
 *   eat this dish. This is a property that can be filtered for.
 * - Ingredients: This is a list of items that is needed to prepare the meal.
 * - Instructions: The instructions are a list of strings that describe how to create the meal out
 *   of the ingredients.
 * - Image: A picture of the prepared meal.
 */
class recipe
{
public:
  //! Iterators to iterate over ingredients.
  using iterator = std::vector<amounted_ingredient>::iterator;
  using const_iterator = std::vector<amounted_ingredient>::const_iterator;

public:
  recipe() = default;
  recipe(recipe const&) = default;
  recipe(recipe&&) = default;
  recipe& operator=(recipe const&) = default;
  recipe& operator=(recipe&&) = default;
  ~recipe() = default;
  bool operator==(recipe const& other) const = default;

  /*!
   * \brief Constructor for recipe
   * \param t title for the new recipe
   */
  recipe(std::string const& t);

  /*!
   * \brief getter for the title of the recipe
   * \return the title of the recipe
   */
  std::string title() const;

  /*!
   * \brief setter for the title of the recipe
   * \param t the new title
   * \return a reference to the changed recipe
   */
  recipe& title(std::string const& t);

  /*!
   * \brief getter for the meal_type of the recipe
   * \return the meal type
   */
  meal_t meal_type() const;

  /*!
   * \brief setter for the meal_type of the recipe
   * \param t the new meal-type
   * \return a reference to the changed recipe
   */
  recipe& meal_type(meal_t t);

  /*!
   * \brief getter for the number of servings of the recipe description
   * \return the number of servings the listed ingredients are counted for
   */
  uint8_t servings() const;

  /*!
   * \brief setter for the number of servings of the recipe description
   * \param s the new number of servings
   * \return a reference to the changed recipe
   */
  recipe& servings(uint8_t s);

  /*!
   * \brief getter for the recipe's instructions
   * \return the recipe's instructions
   */
  std::string instructions() const;

  /*!
   * \brief setter for the recipe's instructions
   * \param i the new instructions
   * \return a reference to the changed recipe
   */
  recipe& instructions(std::string const& i);

  /*!
   * \brief getter for the preparation_time of the recipe
   * \return the estimated preparation time
   */
  std::chrono::minutes preparation_time() const;

  /*!
   * \brief setter for the estimated preparation_time of the recipe
   * \param time the new preparation time
   * \return a reference to the changed recipe
   */
  recipe& preparation_time(std::chrono::minutes time);

  /*!
   * \brief getter for the source of the recipe
   * In this property the source of the recipe is stored. This can either be a book, a website or
   * something like "I came up with it myself!"
   * \return the source of the recipe
   */
  std::string source() const;

  /*!
   * \brief setter for the source of the recipe
   * In this property the source of the recipe is stored. This can either be a book, a website or
   * something like "I came up with it myself!"
   * \param s the new source of the recipe
   * \return a reference to the changed recipe
   */
  recipe& source(std::string const& s);

  /*!
   * \brief getter for the image_path of the recipe
   * \return the image path of the recipe
   */
  std::filesystem::path image_path() const;

  /*!
   * \brief setter for the image_path of the recipe
   * \param path the new path to the image for the recipe
   * \return a reference to the changed recipe
   */
  recipe& image_path(std::filesystem::path const& path);

  /*!
   * \brief getter for the calories one serving of the recipe has
   * \return the calories for one serving
   */
  uint16_t calories() const;

  /*!
   * \brief setter for the calories one serving of the recipe has
   * \param c the new calories for one serving
   * \return a reference to the changed recipe
   */
  recipe& calories(uint16_t c);

  /*!
   * \brief getter for the kilo-joules one serving of the recipe has
   * \return the kilo-joules of the recipe
   */
  uint16_t joules() const;

  /*!
   * \brief setter for the kilo-joules one serving of the recipe has
   * \param c the new kilo-joules value
   * \return a reference to the changed recipe
   */
  recipe& joules(uint16_t c);

  /*!
   * \brief getter for the amount of fat[g] one serving of the recipe contains
   * \return the amount of fat per serving
   */
  uint8_t g_fat() const;

  /*!
   * \brief setter for the amount of fat[g] one serving of the recipe contains
   * \param c the new amount of fat per serving
   * \return a reference to the changed recipe
   */
  recipe& g_fat(uint8_t c);

  /*!
   * \brief getter for the amount of proteins[g] one serving of the recipe contains
   * \return the amount of proteins per serving
   */
  uint8_t g_proteins() const;

  /*!
   * \brief getter for the amount of proteins[g] one serving of the recipe contains
   * \param c the new amount of proteins per serving
   * \return a reference to the changed recipe
   */
  recipe& g_proteins(uint8_t c);

  /*!
   * \brief getter for the amount of carbohydrates[g] one serving of the recipe contains
   * \return the amount of carbohydrates per serving
   */
  uint8_t g_carbohydrates() const;

  /*!
   * \brief getter for the amount of carbohydrates[g] one serving of the recipe contains
   * \param c the new amount of carbohydrates per serving
   * \return a reference to the changed recipe
   */
  recipe& g_carbohydrates(uint8_t c);

  /*!
   * \brief begin: iterate over the ingredients
   * \return a const iterator over the ingredients
   */
  const_iterator begin() const;

  /*!
   * \brief begin: iterate over the ingredients
   * \return an iterator over the ingredients
   */
  iterator begin();

  /*!
   * \brief end: iterate over the ingredients
   * \return an iterator over the ingredients
   */
  const_iterator end() const;

  /*!
   * \brief end: iterate over the ingredients
   * \return an iterator over the ingredients
   */
  iterator end();

  /*!
   * \brief add an ingredient
   * if an ingredient with this id is already stored for the recipe, the amount will be added to the
   * stored ingredient
   * \param ingredient the ingredient to add
   */
  recipe& add(amounted_ingredient const& ingredient);

  /*!
   * \brief remove an ingredient from the list
   * If the ingredient is in the list, the provided ingredient is removed
   * \param item an iterator to the ingredient to remove
   */
  void remove(const_iterator item);

  /*!
   * \brief getter for a concattenated string of tags
   * \return the tags for this recipe
   */
  std::string tags() const;

  /*!
   * \brief getter for the list of tags
   * \return the list of tags
   */
  std::vector<std::string> tagList() const;

  /*!
   * \brief add a Tag
   * \param tag the new tag
   * \return a reference to the changed recipe
   */
  recipe& addTag(std::string const& tag);

  /*!
   * \brief remove a Tag
   * if the tag is not present in the recipe, it is not removed
   * \param tag tag to remove
   */
  void removeTag(std::string const& tag);

  /*!
   * \brief getter for a concattenated string of potential eaters
   * \return the potential eaters for this recipe
   */
  std::string eaters() const;

  /*!
   * \brief getter for the list of potential eaters
   * \return the list of potential eaters
   */
  std::vector<std::string> eaterList() const;

  /*!
   * \brief add an Eater
   * \param eater the new eater
   * \return a reference to the changed recipe
   */
  recipe& addEater(std::string const& eater);

  /*!
   * \brief remove an Eater
   * if the eater is not stored in the list of potential eaters, it is not removed
   * \param eater the eater to remove
   */
  void removeEater(std::string const& eater);

  /*!
   * \brief getter for the id of the recipe
   * \return the id of the recipe
   */
  boost::uuids::uuid id() const;

  /*!
   * \brief setter for the id of the recipe
   * \param i the new id
   * \return a reference to the changed recipe
   */
  recipe& id(boost::uuids::uuid i);

private:
  std::string _title;
  meal_t _meal_type;
  uint8_t _servings;
  std::vector<amounted_ingredient> _ingredients;
  std::string _instructions;
  std::chrono::minutes _time;
  std::string _source;
  std::vector<std::string> _tags;
  std::vector<std::string> _eaters;
  std::filesystem::path _image_path;
  uint16_t _calories;
  uint16_t _joules;
  uint8_t _g_fat;
  uint8_t _g_protein;
  uint8_t _g_carbohydrates;
  boost::uuids::uuid _id;
};

} // namespace recipe
