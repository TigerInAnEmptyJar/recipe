#pragma once

#include "ingredient.h"
#include "recipe.h"

#include <filesystem>
#include <functional>
#include <optional>
#include <vector>

namespace recipe {
namespace io {

/*!
 * \brief The recipe_io class
 * This is the base class for types responsible for writing and reading recipes from or to file.
 */
class recipe_io
{
public:
  //! The function-type for providing access to the stored ingredients.
  using finder_t = std::function<std::optional<ingredient>(boost::uuids::uuid)>;

  /*!
   * \brief write a list of recipes to a file.
   * \param out the list of recipes to write,
   * \param path the path to write to.
   */
  virtual void write(std::vector<recipe> const& out, std::filesystem::path const& path) const = 0;

    /*!
   * \brief write a list of recipes to a file.
   * \param out the list of recipes to write,
   * \param path the stream to write to.
   */
  virtual void write(std::vector<recipe> const& out, std::ostream & path) const = 0;

  /*!
   * \brief read recipes from file
   * \param path the path to read from,
   * \param ingredient_finder a functor that will provide an ingredient for a provided id,
   * \return a list of recipes, if available.
   */
  virtual std::optional<std::vector<recipe>> read(std::filesystem::path const& path,
                                                  finder_t ingredient_finder) const = 0;
};

} // namespace io
} // namespace recipe
