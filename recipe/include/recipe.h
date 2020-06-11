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
  using iterator = std::vector<amounted_ingredient>::iterator;
  using const_iterator = std::vector<amounted_ingredient>::const_iterator;

public:
  recipe() = default;
  recipe(recipe const&) = default;
  recipe(recipe&&) = default;
  recipe& operator=(recipe const&) = default;
  recipe& operator=(recipe&&) = default;
  ~recipe() = default;

  /*!
   * \brief Constructor for recipe
   * \param t title for the new recipe
   */
  recipe(std::string const& t);

  std::string title() const;
  void title(std::string const& t);

  meal_t meal_type() const;
  void meal_type(meal_t t);

  uint8_t servings() const;
  void servings(uint8_t s);

  std::string instructions() const;
  void instructions(std::string const& i);

  std::chrono::minutes preparation_time() const;
  void preparation_time(std::chrono::minutes time);

  std::string source() const;
  void source(std::string const& s);

  std::filesystem::path image_path() const;
  void image_path(std::filesystem::path const& path);

  uint8_t calories() const;
  void calories(uint8_t c);

  uint8_t joules() const;
  void joules(uint8_t c);

  uint8_t g_fat() const;
  void g_fat(uint8_t c);

  uint8_t g_proteins() const;
  void g_proteins(uint8_t c);

  uint8_t g_carbohydrates() const;
  void g_carbohydrates(uint8_t c);

  const_iterator begin() const;
  iterator begin();

  const_iterator end() const;
  iterator end();

  void add(amounted_ingredient const& ingredient);
  void remove(const_iterator item);

  std::string tags() const;
  std::vector<std::string> tagList() const;
  void addTag(std::string const& tag);
  void removeTag(std::string const& tag);

  std::string eaters() const;
  std::vector<std::string> eaterList() const;
  void addEater(std::string const& eater);
  void removeEater(std::string const& eater);

  boost::uuids::uuid id() const;
  void id(boost::uuids::uuid i);

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
  uint8_t _calories;
  uint8_t _joules;
  uint8_t _g_fat;
  uint8_t _g_protein;
  uint8_t _g_carbohydrates;
  boost::uuids::uuid _id;
};

} // namespace recipe
