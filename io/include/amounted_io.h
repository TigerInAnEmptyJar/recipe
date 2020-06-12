#pragma once

#include "amounted_ingredient.h"

#include <filesystem>
#include <functional>
#include <optional>
#include <vector>

namespace recipe {
namespace io {

/*!
 * \brief The amounted_io class
 * This is the base class for types responsible for writing and reading amounted ingredients from
 * or to file.
 */
class amounted_io
{
public:
  using finder_t = std::function<std::optional<ingredient>(boost::uuids::uuid)>;

  /*!
   * \brief write a list of amounted ingredients to a file.
   * \param out the list of amounted ingredients to write,
   * \param path the path to write to,
   */
  virtual void write(std::vector<amounted_ingredient> const& out,
                     std::filesystem::path const& path) const = 0;

  /*!
   * \brief read amounted ingredients from file
   * \param path the path to read from,
   * \param ingredient_finder a functor that returns an ingredient for a provided id.
   * \return a list of amounted ingredients, if available.
   */
  virtual std::optional<std::vector<amounted_ingredient>> read(
      std::filesystem::path const& path, finder_t ingredient_finder) const = 0;
};

} // namespace io
} // namespace recipe
