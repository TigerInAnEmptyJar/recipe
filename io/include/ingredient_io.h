#pragma once

#include "ingredient.h"

#include <filesystem>
#include <optional>
#include <vector>

namespace recipe {
namespace io {

/*!
 * \brief The ingredient_io class
 * This is the base class for types responsible for writing and reading ingredients from or to file.
 */
class ingredient_io
{
public:
  /*!
   * \brief write a list of ingredients to a file.
   * \param out the list of ingredients to write,
   * \param path the path to write to.
   */
  virtual void write(std::vector<ingredient> const& out,
                     std::filesystem::path const& path) const = 0;

  /*!
   * \brief read ingredients from file
   * \param path the path to read from,
   * \return a list of ingredients, if available.
   */
  virtual std::optional<std::vector<ingredient>> read(std::filesystem::path const& path) const = 0;
};

} // namespace io
} // namespace recipe
